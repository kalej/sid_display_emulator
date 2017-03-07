#include <avr/io.h>
#include <avr/builtins.h>
#include <util/delay.h>
#include <string.h>
#include "ssd1306.h"
#include "parallel.h"
#include "i2c.h"
#include "pins.h"
#include "divutils.h"

#define LINE_SIZE 64
typedef struct
{
	uint8_t data[LINE_SIZE];
	uint8_t changed[(LINE_SIZE+4)/5];
} DispLine_T;

void line_init(DispLine_T* line)
{
	memset(line->data, 0, sizeof(line->data));
	memset(line->changed, 1, sizeof(line->changed));
}

typedef struct
{
	DispLine_T lines[2];
	uint8_t line_idx;
	uint8_t col_idx;
} Display_T;

void display_init(Display_T* display)
{
	line_init(&display->lines[0]);
	line_init(&display->lines[1]);
	display->line_idx = 0;
	display->col_idx = 0;
}

Display_T displays[2] __attribute__ ((section (".noinit")));

int main( void )
{
	uint8_t cd, data, address, bank, upd_line, upd_symbol;
	uint8_t symbol[6];
	Display_T* disp;
	DispLine_T* line;

	parallel_init();
	symbol[5] = 0;
	display_init(&displays[0]);
	display_init(&displays[1]);

	ssd1306_init();
	sei();
	BUSY_HIGH;
	upd_line = 0;
	upd_symbol = 0;

	while( 1 ) {
		i2c_poll();

		if ( parallel_count() > 1 )
		{
			parallel_read(&cd);
			parallel_read(&data);

			address = cd & 1;
			if (cd & CD_MASK)
			{
				//command
				if ( data & 0x80 )
				{
					bank = (data & 0x40)?1:0;
					displays[address].line_idx = bank;
					displays[address].col_idx = data & 0x3F;
				}
			}
			else
			{
				//data
				disp = &displays[address];
				line = &disp->lines[disp->line_idx];
				if ( line->data[disp->col_idx] != data )
				{
					line->changed[DIV5(disp->col_idx)] = 1;
				}
				line->data[disp->col_idx] = data;
				disp->col_idx++;
			}
		}

		disp = &displays[upd_line >> 1];
		line = &disp->lines[upd_line & 1];
		if ( line->changed[upd_symbol] )
		{
			memcpy(symbol, line->data + upd_symbol*5, 5);
			if ( !ssd1306_put_data_at(upd_symbol*6 + ((upd_line&0x02)?0:60), (upd_line&1) + 1, symbol, 6) )
			{
				line->changed[upd_symbol] = 0;
			}
		}
		upd_symbol +=1;
		if ( upd_symbol == 10 )//sizeof(line->data)/5 )
		{
			upd_symbol = 0;
			upd_line = (upd_line + 1) & 3;
		}
	}
	return 0;
}

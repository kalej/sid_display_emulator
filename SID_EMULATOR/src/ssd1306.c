/*
 * ssd1306.c
 *
 *  Created on: 5 мар. 2017 г.
 *      Author: Tech2
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "i2c.h"
#include "ssd1306.h"
#include "readers.h"

uint8_t EEMEM ssd1306init[] =
{
	SSD1306_DEFAULT_ADDRESS,
	0x00,
	SSD1306_DISPLAYOFF,
	0x80,
	SSD1306_SETDISPLAYCLOCKDIV,
	0x80,
	0x80,
	SSD1306_SETMULTIPLEX,
	0x3F,
	0x80,
	SSD1306_SETDISPLAYOFFSET,
	0x00,
	0x80,
	SSD1306_SETSTARTLINE | 0x00,
	0x80,
	SSD1306_CHARGEPUMP,
	0x14,
	0x80,
	SSD1306_MEMORYMODE,
	0x00,
	0x80,
	//SSD1306_SEGREMAP | 0x1,
	SSD1306_SEGREMAP | 0x0,
	0x80,
	SSD1306_COMSCANDEC,//for 128_64
	//SSD1306_COMSCANINC,//for 128_32
	0x80,
	SSD1306_SETCOMPINS,
	//0x12,
	0x02,
	0x80,
	SSD1306_SETCONTRAST,
	0xCF,
	0x80,
	SSD1306_SETPRECHARGE,
	0xF1,
	0x80,
	SSD1306_SETVCOMDETECT,
	0x40,
	0x80,
	SSD1306_DISPLAYALLON_RESUME,
	0x80,
	SSD1306_NORMALDISPLAY,
	0x80,
	SSD1306_DISPLAYON,
	0x80,
	SSD1306_COLUMNADDR,
	0x00,
	0x7F,
	0x80,
	SSD1306_PAGEADDR,
	0x00,
	0x03
};

uint8_t ssd1306_init()
{
	i2c_init_400();
	return i2c_put(ssd1306init, sizeof(ssd1306init), eeprom_reader);
}

uint8_t ssd1306_set_pointer(uint8_t x, uint8_t y)
{
	uint8_t data[9];

	if ( i2c_get_available() < 10 )
		return -1;

	data[0] = SSD1306_DEFAULT_ADDRESS;
	data[1] = 0x00;
	data[2] = SSD1306_PAGEADDR;
	data[3] = y;
	data[4] = 8;
	data[5] = 0x80;
	data[6] = SSD1306_COLUMNADDR;
	data[7] = x;
	data[8] = 127;

	return i2c_put(data, sizeof(data), ram_reader);
}

uint8_t ssd1306_put_data(uint8_t* what, uint8_t len)
{
	uint8_t data[MAX_DISP_DATA_LEN + 2];
	uint8_t* data_ptr;
	uint8_t i;

	if (len > MAX_DISP_DATA_LEN)
		return -1;

	if ( i2c_get_available() < len + 3 )
		return -1;

	data_ptr = data + 2;
	data[0] = SSD1306_DEFAULT_ADDRESS;
	data[1] = 0x40;

	for ( i = 0; i < len; i ++)
	{
		*data_ptr = what[i];
		data_ptr++;
	}

	return i2c_put(data, len + 2, ram_reader);
}

uint8_t ssd1306_put_data_at(uint8_t x, uint8_t y, uint8_t* what, uint8_t len)
{

	if (len > MAX_DISP_DATA_LEN)
		return -1;

	if ( i2c_get_available() < len + 10 + 3 )
		return -1;

	return ssd1306_set_pointer(x, y) | ssd1306_put_data(what, len);
}

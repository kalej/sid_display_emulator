/*
 * parallel.c
 *
 *  Created on: 10 февр. 2017 г.
 *      Author: Tech2
 */

#include <stdint.h>
#include "parallel.h"
#include "pins.h"

volatile ParallelBuffer_T pb __attribute__ ((section (".noinit")));
volatile uint8_t cd_address __attribute__ ((section (".noinit")));
volatile uint8_t data __attribute__ ((section (".noinit")));
volatile uint8_t edge_counter __attribute__ ((section (".noinit")));

ISR(CS_INT_VECTOR)
{
	if ( READ_CS == 0 ) //i.e. CS low, the beginning
	{
		edge_counter = 0;
		STB_INT_ENABLE;
		SET_STB_INT_ON_FALLING;
	}
	else //i.e. CS high, the end
	{
		STB_INT_DISABLE;
		if ( edge_counter == 2 )
		{
			BUSY_LOW;

			pb.data[pb.writer_idx] = cd_address;
			pb.writer_idx = (pb.writer_idx + 1) & PARALLEL_BUFFER_SIZE_MASK;
			pb.count++;

			pb.data[pb.writer_idx] = data;
			pb.writer_idx = (pb.writer_idx + 1) & PARALLEL_BUFFER_SIZE_MASK;
			pb.count++;

			BUSY_HIGH;
		}
	}
}

ISR(STB_INT_VECTOR)
{
	if ( READ_STB == 0 )
	{
		cd_address = READ_CD | READ_DATA;
		SET_STB_INT_ON_RISING;
	}
	else
	{
		if ( edge_counter != 0 )
		{
			data |= DATA_PIN & DATA_MASK;
		}
		else
		{
			data = DATA_PIN << 4;
		}
		edge_counter++;
	}
}

void parallel_init( void )
{
	pb.count = 0;
	pb.reader_idx = 0;
	pb.writer_idx = 0;

	SET_BUSY_OUT;
	BUSY_LOW;

	SET_DATA_IN;
	DATA_NO_PULL_UP;

	SET_CD_IN;
	CD_NO_PULL_UP;

	SET_STB_IN;
	STB_NO_PULL_UP;

	SET_CS_IN;
	CS_NO_PULL_UP;

	SET_CS_INT_ON_CHANGE;
	STB_INT_DISABLE;
	CS_INT_ENABLE;
}

uint16_t parallel_count( void )
{
	return pb.count;
}

uint8_t parallel_read( uint8_t* out )
{
	if (pb.count == 0)
		return 0;

	CS_INT_DISABLE;
	STB_INT_DISABLE;
	pb.count--;
	STB_INT_ENABLE;
	CS_INT_ENABLE;

	*out = pb.data[pb.reader_idx];
	pb.reader_idx = (pb.reader_idx + 1) & PARALLEL_BUFFER_SIZE_MASK;
	return 1;
}

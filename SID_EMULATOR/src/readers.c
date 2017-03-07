/*
 * readers.c
 *
 *  Created on: 7 мар. 2017 г.
 *      Author: Tech2
 */
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "readers.h"

uint8_t eeprom_read(uint8_t* data)
{
	return eeprom_read_byte(data);
}

uint8_t pgm_read(uint8_t* data)
{
	return pgm_read_byte(data);
}

uint8_t ram_read(uint8_t* data)
{
	return *data;
}

ByteReader_T eeprom_reader = &eeprom_read;
ByteReader_T pgm_reader = &pgm_read;
ByteReader_T ram_reader = &ram_read;

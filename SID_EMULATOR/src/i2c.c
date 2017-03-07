/*
 * i2c.c
 *
 *  Created on: 28 февр. 2017 г.
 *      Author: Tech2
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "i2c.h"
#include "pins.h"

typedef struct
{
	uint8_t data[I2C_BUFFER_SIZE];
	uint16_t reader_idx;
	uint16_t writer_idx;
	uint16_t count;
} I2CBuffer_T;

volatile I2CBuffer_T ib  __attribute__ ((section (".noinit")));

#define TW_START					1
#define TW_REP_START				2
#define TW_MT_SLA_ACK				3
#define TW_MT_SLA_NACK				4
#define TW_MT_DATA_ACK				5
#define TW_MT_DATA_NACK				6
#define TW_ARB_LOST					7
#define TW_MR_SLA_ACK				8
#define TW_MR_SLA_NACK				9
#define TW_MR_DATA_ACK				10
#define TW_MR_DATA_NACK				11
#define TW_ST_SLA_ACK				21
#define TW_ST_ARB_LOST_SLA_ACK		22
#define TW_ST_DATA_ACK				23
#define TW_ST_DATA_NACK				24
#define TW_ST_LAST_DATA				25
#define TW_SR_SLA_ACK				12
#define TW_SR_ARB_LOST_SLA_ACK		13
#define TW_SR_GCALL_ACK				14
#define TW_SR_ARB_LOST_GCALL_ACK	15
#define TW_SR_DATA_ACK				16
#define TW_SR_DATA_NACK				17
#define TW_SR_GCALL_DATA_ACK		18
#define TW_SR_GCALL_DATA_NACK		19
#define TW_SR_STOP					20
#define TW_NO_INFO					31
#define TW_BUS_ERROR				0

#define TWI_STATUS (((uint8_t)TWSR) >> 3)

uint8_t is_sending;

void i2c_poll(void)
{
	static uint16_t idx;
	static uint8_t msg_len;
	static uint8_t msg_rest;

	if ( !is_sending && ib.count )
	{
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		is_sending = 1;
	}

	if ( is_sending && !(TWCR & (1<<TWINT)) )
		return;

	BLUE_LED_ON;

	switch (TWI_STATUS){
    case TW_START:                   // on start or repeated start condition
    case TW_REP_START:
    	msg_rest = ib.data[ib.reader_idx];
    	msg_len = msg_rest;
    	idx = (ib.reader_idx + 1) & I2C_IDX_MASK;
    	TWDR = ib.data[idx];
    	idx = (idx + 1) & I2C_IDX_MASK;
    	msg_rest--;
    	TWCR = (1<<TWEN)|(1<<TWINT);
    	break;
    case TW_MT_SLA_ACK:
    case TW_MT_DATA_ACK:
    	if (msg_rest && ib.count)
    	{
    		TWDR = ib.data[idx];
			idx = (idx + 1) & I2C_IDX_MASK;
			msg_rest--;
			TWCR = (1<<TWEN)|(1<<TWINT);
    	}
    	else
    	{
    		ib.reader_idx = idx;
    		ib.count -= msg_len + 1;
    		TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
    		while(TWCR & (1<<TWSTO));
    		is_sending = 0;
    	}
    	break;
    case TW_MT_SLA_NACK:
    case TW_MT_DATA_NACK:
    case TW_BUS_ERROR:
    default:
    	TWCR = 0;
    	TWCR = (1<<TWEN)|(1<<TWINT);
  }
  BLUE_LED_OFF;
}

uint8_t i2c_init_400(void)
{
  ib.count = 0;
  ib.writer_idx = 0;
  ib.reader_idx = 0;

  TWBR = 12;
  //TWSR = 0;
  //TWDR = 0xFF;
  //TWCR = (1<<TWEN);
  is_sending = 0;
  return 0;
}

uint16_t i2c_get_available(void)
{
	return I2C_BUFFER_SIZE - ib.count;
}

uint8_t i2c_put(uint8_t* data, uint8_t len, ByteReader_T reader)
{
	uint8_t i;
	if ( i2c_get_available() < len + 1 )
		return 1;

	ib.data[ib.writer_idx] = len;
	ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;

	for( i = 0; i < len; i++ )
	{
		ib.data[ib.writer_idx] = reader(&data[i]);
		ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;
	}
	ib.count += len + 1;
	return 0;
}
/*
uint8_t i2c_put_from_eeprom(uint8_t* data, uint8_t len)
{
	uint8_t i;
	if ( i2c_get_available() < len + 1 )
		return 1;

	ib.data[ib.writer_idx] = len;
	ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;

	for( i = 0; i < len; i++ )
	{
		ib.data[ib.writer_idx] = eeprom_read_byte(data + i);
		ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;
	}
	ib.count += len + 1;
	return 0;
}

uint8_t i2c_put_from_progmem(uint8_t* data, uint8_t len)
{
	uint8_t i;
	if ( i2c_get_available() < len + 1 )
		return 1;

	ib.data[ib.writer_idx] = len;
	ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;

	for( i = 0; i < len; i++ )
	{
		ib.data[ib.writer_idx] = pgm_read_byte(data + i);
		ib.writer_idx = (ib.writer_idx + 1) & I2C_IDX_MASK;
	}
	ib.count += len + 1;
	return 0;
}
*/

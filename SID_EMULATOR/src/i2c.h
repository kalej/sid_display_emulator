/*
 * i2c.h
 *
 *  Created on: 28 февр. 2017 г.
 *      Author: Tech2
 */

#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include "readers.h"

#define I2C_BUFFER_SIZE 256
#define I2C_IDX_MASK (I2C_BUFFER_SIZE - 1)


uint8_t i2c_init_400(void);
uint16_t i2c_get_available(void);
uint8_t i2c_put(uint8_t* data, uint8_t len, ByteReader_T reader);
void i2c_poll(void);
#endif /* I2C_H_ */

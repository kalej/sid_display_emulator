/*
 * divutils.h
 *
 *  Created on: 5 мар. 2017 г.
 *      Author: Tech2
 */

#ifndef DIVUTILS_H_
#define DIVUTILS_H_

#include <avr/pgmspace.h>

extern const uint8_t div5tbl[];
extern const uint8_t mod5tbl[];

#define DIV5(x) (pgm_read_byte(div5tbl + x))
#define MOD5(x) (pgm_read_byte(mod5tbl + x))

#endif /* DIVUTILS_H_ */

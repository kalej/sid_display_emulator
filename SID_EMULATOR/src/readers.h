/*
 * readers.h
 *
 *  Created on: 7 мар. 2017 г.
 *      Author: Tech2
 */

#ifndef SRC_READERS_H_
#define SRC_READERS_H_

typedef uint8_t (*ByteReader_T)(uint8_t*);
extern ByteReader_T eeprom_reader;
extern ByteReader_T pgm_reader;
extern ByteReader_T ram_reader;

#endif /* SRC_READERS_H_ */

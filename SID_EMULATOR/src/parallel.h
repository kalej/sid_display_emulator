/*
 * parallel.h
 *
 *  Created on: 10 февр. 2017 г.
 *      Author: Tech2
 */

#ifndef PARALLEL_H_
#define PARALLEL_H_

#include <avr/interrupt.h>

//definitions of interrupt detect types for INT0
#define INT0_DETECT_MASK ((1<<ISC01)|(1<<ISC00))
#define INT0_DETECT_AT_LOW ((0<<ISC01)|(0<<ISC00))
#define INT0_DETECT_ON_CHANGE ((0<<ISC01)|(1<<ISC00))
#define INT0_DETECT_ON_FALLING ((1<<ISC01)|(0<<ISC00))
#define INT0_DETECT_ON_RISING ((1<<ISC01)|(1<<ISC00))

#define SET_INT0_AT_LOW {EICRA = (EICRA & ~INT0_DETECT_MASK) | INT0_DETECT_AT_LOW;}
#define SET_INT0_ON_CHANGE {EICRA = (EICRA & ~INT0_DETECT_MASK) | INT0_DETECT_ON_CHANGE;}
#define SET_INT0_ON_RISING {EICRA = (EICRA & ~INT0_DETECT_MASK) | INT0_DETECT_ON_RISING;}
#define SET_INT0_ON_FALLING {EICRA = (EICRA & ~INT0_DETECT_MASK) | INT0_DETECT_ON_FALLING;}

//definitions of interrupt detect types for INT1
#define INT1_DETECT_MASK ((1<<ISC11)|(1<<ISC10))
#define INT1_DETECT_AT_LOW ((0<<ISC11)|(0<<ISC10))
#define INT1_DETECT_ON_CHANGE ((0<<ISC11)|(1<<ISC10))
#define INT1_DETECT_ON_FALLING ((1<<ISC11)|(0<<ISC10))
#define INT1_DETECT_ON_RISING ((1<<ISC11)|(1<<ISC10))

#define SET_INT1_AT_LOW {EICRA = (EICRA & ~INT1_DETECT_MASK) | INT1_DETECT_AT_LOW;}
#define SET_INT1_ON_CHANGE {EICRA = (EICRA & ~INT1_DETECT_MASK) | INT1_DETECT_ON_CHANGE;}
#define SET_INT1_ON_RISING {EICRA = (EICRA & ~INT1_DETECT_MASK) | INT1_DETECT_ON_RISING;}
#define SET_INT1_ON_FALLING {EICRA = (EICRA & ~INT1_DETECT_MASK) | INT1_DETECT_ON_FALLING;}

//definitions of interrupt masks for INT0
#define INT0_MASK (1<<INT0)
#define ENABLE_INT0 {EIMSK |= INT0_MASK;}
#define DISABLE_INT0 {EIMSK &= ~INT0_MASK;}

//definitions of interrupt masks for INT1
#define INT1_MASK (1<<INT1)
#define ENABLE_INT1 {EIMSK |= INT1_MASK;}
#define DISABLE_INT1 {EIMSK &= ~INT1_MASK;}

//define aliases
#define CS_INT_VECTOR INT0_vect
#define STB_INT_VECTOR INT1_vect

#define SET_CS_INT_AT_LOW SET_INT0_AT_LOW
#define SET_CS_INT_ON_CHANGE SET_INT0_ON_CHANGE
#define SET_CS_INT_ON_RISING SET_INT0_ON_RISING
#define SET_CS_INT_ON_FALLING SET_INT0_ON_FALLING

#define SET_STB_INT_AT_LOW SET_INT1_AT_LOW
#define SET_STB_INT_ON_CHANGE SET_INT1_ON_CHANGE
#define SET_STB_INT_ON_RISING SET_INT1_ON_RISING
#define SET_STB_INT_ON_FALLING SET_INT1_ON_FALLING

#define CS_INT_ENABLE ENABLE_INT0
#define CS_INT_DISABLE DISABLE_INT0

#define STB_INT_ENABLE ENABLE_INT1
#define STB_INT_DISABLE DISABLE_INT1

#define PARALLEL_BUFFER_SIZE_PWR 8
#define PARALLEL_BUFFER_SIZE (1<<PARALLEL_BUFFER_SIZE_PWR)
#define PARALLEL_BUFFER_SIZE_MASK ((PARALLEL_BUFFER_SIZE) - 1)

typedef struct
{
	uint8_t data[PARALLEL_BUFFER_SIZE];
	uint16_t reader_idx;
	uint16_t writer_idx;
	uint16_t count;

} ParallelBuffer_T;

void parallel_init( void );
uint16_t parallel_count( void );
uint8_t parallel_read( uint8_t* out );
#endif /* PARALLEL_H_ */

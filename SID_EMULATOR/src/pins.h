/*
 * pin_def.h
 *
 *  Created on: 10 февр. 2017 г.
 *      Author: Tech2
 */

#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>

// DATA - PC3:PC0
#define DATA_DDR (DDRC)
#define DATA_MASK ((1 << PORTC0)|(1 << PORTC1)|(1 << PORTC2)|(1 << PORTC3))
#define DATA_PIN (PINC)
#define DATA_PORT (PORTC)

#define SET_DATA_IN {DATA_DDR &= ~DATA_MASK;}
#define READ_DATA (DATA_PIN & DATA_MASK)
#define DATA_PULL_UP {DATA_PORT |= DATA_MASK;}
#define DATA_NO_PULL_UP {DATA_PORT &= ~DATA_MASK;}

// C, /D - PB4
#define CD_DDR (DDRB)
#define CD_PIN (PINB)
#define CD_MASK (1 << PORTB4)
#define CD_PORT (PORTB)

#define SET_CD_IN {CD_DDR &= ~CD_MASK;}
#define READ_CD (CD_PIN & CD_MASK)
#define CD_PULL_UP {CD_PORT |= CD_MASK;}
#define CD_NO_PULL_UP {CD_PORT &= ~CD_MASK;}

// BUSY - PB3
#define BUSY_DDR (DDRB)
#define BUSY_PORT (PORTB)
#define BUSY_PIN (PINB)
#define BUSY_MASK (1 << PORTB3)

#define SET_BUSY_OUT {BUSY_DDR |= BUSY_MASK;}
#define BUSY_LOW {BUSY_PORT &= ~BUSY_MASK;}
#define BUSY_HIGH {BUSY_PORT |= BUSY_MASK;}
#define READ_BUSY (BUSY_PIN & BUSY_MASK)

// /STB - PD3, INT1
#define STB_DDR (DDRD)
#define STB_PIN (PIND)
#define STB_MASK (1 << PORTD3)
#define STB_PORT (PORTD)

#define SET_STB_IN {STB_DDR &= ~STB_MASK;}
#define READ_STB (STB_PIN & STB_MASK)
#define STB_PULL_UP {STB_PORT |= STB_MASK;}
#define STB_NO_PULL_UP {STB_PORT &= ~STB_MASK;}

// /CS - PD2, INT0
#define CS_DDR (DDRD)
#define CS_PIN (PIND)
#define CS_MASK (1 << PORTD2)
#define CS_PORT (DDRD)

#define SET_CS_IN {CS_DDR &= ~ CS_MASK;}
#define READ_CS (CS_PIN & CS_MASK)
#define CS_PULL_UP {CS_PORT |= CS_MASK;}
#define CS_NO_PULL_UP {CS_PORT &= ~CS_MASK;}

//BLUE LED - PB5
#define BLUE_LED_DDR (DDRB)
#define BLUE_LED_PORT (PORTB)
#define BLUE_LED_MASK (1 << PORTB5)

#define SET_BLUE_LED_OUT {BLUE_LED_DDR |= BLUE_LED_MASK;}
#define BLUE_LED_ON {BLUE_LED_PORT |= BLUE_LED_MASK;}
#define BLUE_LED_OFF {BLUE_LED_PORT &= ~BLUE_LED_MASK;}

#endif /* PINS_H_ */

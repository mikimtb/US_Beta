/* 
 * File:   US_range_finder.h
 * Author: Miroslav Bozic
 *
 * Created on October 31, 2016, 3:56 PM
 */

#ifndef US_RANGE_FINDER_H
#define	US_RANGE_FINDER_H

#include <16F887.h>
#device adc=10

#FUSES NOWDT                 	// No Watch Dog Timer
#FUSES INTRC                    // Internal RC Osc, no CLKOUT
#FUSES NOBROWNOUT            	// No brownout reset
#FUSES NOIESO                	// Internal External Switch Over mode disabled
#FUSES NOLVP                 	// No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(int=8000000)

/* Global defines */
#define TRIG PIN_B0
#define ECHO PIN_B1

/* Global variables */
int8 trigger = 0;

#endif	/* US_RANGE_FINDER_H */


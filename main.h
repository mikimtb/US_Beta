/* 
 * File:   US_range_finder.h
 * Author: Miroslav Bozic
 *
 * Created on October 31, 2016, 3:56 PM
 */

#ifndef MAIN_H
#define	MAIN_H

/* MCU definition */
#define DEBUG   TRUE

#ifdef DEBUG
#include <16F887.h>
#device adc=10

#FUSES NOWDT                 	// No Watch Dog Timer
#FUSES INTRC                    // Internal RC Osc, no CLKOUT
#FUSES NOBROWNOUT            	// No brownout reset
#FUSES NOIESO                	// Internal External Switch Over mode disabled
#FUSES NOLVP                 	// No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#else
#include <16F684.h>
#device adc=10

#FUSES NOWDT                 	// No Watch Dog Timer
#FUSES INTRC_IO                 // Internal RC Osc, no CLKOUT, pins are used as IO
#FUSES NOBROWNOUT            	// No brownout reset
#FUSES NOIESO                	// Internal External Switch Over mode disabled
#FUSES NOMCLR                   // MCLR pin is used as IO
#endif

#use delay(int=8000000)

/* Global defines */
#define TRIG PIN_B0
#define ECHO PIN_B2

#endif	/* US_RANGE_FINDER_H */


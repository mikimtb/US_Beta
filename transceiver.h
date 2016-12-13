/* 
 * File:   tranceiver.h
 * Author: Miroslav
 *
 * Created on November 4, 2016, 10:32 PM
 */

#ifndef TRANCEIVER_H
#define	TRANCEIVER_H

// Private function prototypes
void timer_tick(void);
void gpio_isr_handler(void);
void comparator_isr_handler(void);

// Public function prototypes
void transceiver_init(void);
void transceiver_ready(void);
void transceiver_transmit(int8 pulse_no);
void transceiver_listen(void);
void transceiver_event_handler(void);
void transceiver_wait(void);
void transceiver_trigger(void);
void transceiver_timeout(void);
void transceiver_echo_above(void);
void transceiver_echo_below(void);

#endif	/* TRANCEIVER_H */


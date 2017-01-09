#include "gpio.h"

/* PORTB address */
#ifdef DEBUG
#byte PORTB = 0x06
#else
#byte PORTA = 0x05
#endif

/**
 * Function initialize gpio pins for Trigger and Echo
 * Trigger - Input, Low to High edge on gpio raise gpio_isr_handler
 * Echo -  Output, the length of highe pulse on this gpio represent the echo 
 */
void gpio_init()
{
#ifdef DEBUG    
    set_tris_b(get_tris_b() & 0b11111011);          // Set RB2 and RB3 as output because 
                                                    // the pins are used as ECHO and TEST
#else
    set_tris_a(get_tris_a() & 0b11011111);          // Set RA5 as output because 
                                                    // the pin is used as ECHO
#endif
}

/**
 * Function enable INT_RB0 interrupts in order to catch trigger
 */
void gpio_trigger_enable()
{
    char b;
#ifdef DEBUG   
    b = PORTB;                              // Clear mismatch condition
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB0);
#else
    b = PORTA;                              // Clear mismatch condition
    clear_interrupt(INT_RA);
    enable_interrupts(INT_RA0);
#endif
}



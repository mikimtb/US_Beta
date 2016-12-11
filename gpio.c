#include "gpio.h"

/* PORTB address */
#byte PORTB = 0x06

/**
 * Function initialize gpio pins for Trigger and Echo
 * Trigger - Input, Low to High edge on gpio raise gpio_isr_handler
 * Echo -  Output, the length of highe pulse on this gpio represent the echo 
 */
void gpio_init()
{
    char b;
    
    set_tris_b(get_tris_b() & 0b11110011);          // Set RB2 and RB3 as output because 
                                                    // the pins are used as ECHO and TEST
    b = PORTB;                                      // Clear mismatch condition
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB0);
}



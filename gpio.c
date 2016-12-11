#include "gpio.h"

/**
 * Function initialize gpio pins for Trigger and Echo
 * Trigger - Input, Low to High edge on gpio raise gpio_isr_handler
 * Echo -  Output, the length of highe pulse on this gpio represent the echo 
 */
void gpio_init()
{
    char b;
    
    set_tris_b(get_tris_b() & 0b11111011);          // Set RB1 as output becasue 
                                                    // the pin is used as ECHO
    b = PORTB;                                      // Clear mismatch condition
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB0);
}



#include "gpio.h"

extern int8 trigger;
/**
 * Function initialize gpio pins for Trigger and Echo
 * Trigger - Input, Low to High edge on gpio raise gpio_isr_handler
 * Echo -  Output, the length of highe pulse on this gpio represent the echo 
 */
void gpio_init()
{
    char b;
    
    set_tris_b(get_tris_b() & 0b11111101);          // Set RB1 as output becasue 
                                                    // the pin is used as ECHO
    b = PORTB;                                      // Clear mismatch condition
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB0);
}

/**
 * GPIO interrupt handler. Low to High edge event handler raise this function
 */
#INT_RB
void gpio_isr_handler()
{
    char b;
    
    if(input(TRIG))
        trigger = 1;
    
    b = PORTB;                                      // Clear mismatch condition
}

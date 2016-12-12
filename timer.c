#include "timer.h"

// Register definitions
/* INTCON register address */
#byte INTCON = 0x0b
#bit  T0IF = INTCON.2                       // Timer 0 overflow flag

//Private variables
extern int timeout;

/**
 * Function initialize the timer
 */
void timer_init(void)
{
    // Set timer0 overflow to 2048us
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8);                                  
}   

/**
 * Function start timer and initialize timeout time value
 * @param tout - 1 - 255, the number represent timeout in milliseconds
 */
void timer_start(int tout)
{
    timeout = tout;
    set_timer0(10);                                 // Set timer0 value to 10 in
                                                    // order to produce overflow
                                                    // on every 1ms
    T0IF = 0;
    enable_interrupts(INT_RTCC);
}

/**
 * Function disable timer interrupt and prevent timer tick to happen
 */
void timer_stop()
{
    disable_interrupts(INT_RTCC);
}

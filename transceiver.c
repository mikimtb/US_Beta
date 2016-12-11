#include "transceiver.h"
#include "pwm.h"
#include "gpio.h"
#include "gpio.c"
#include "timer.h"
#include "timer.c"
#include "main.h"

//Type definitions
typedef void (*t_fptr)(void);

t_fptr event = transceiver_wait;

//Private variables
int timeout = 0;

// Private functions

/**
 * GPIO interrupt handler. Low to High edge event handler raise this function
 */
#INT_RB
void gpio_isr_handler()
{
    char b;
    
    if(input(TRIG))
        event = transceiver_trigger;
    
    b = PORTB;                                      // Clear mismatch condition
}

/**
 * Timer tick event handler
 */
#INT_RTCC
void timer_tick(void)
{
    set_timer0(10);
    timeout--;
    if (timeout == 0)
    {
        event = transceiver_timeout;
        disable_interrupts(INT_RTCC);
    }
    //output_toggle(PIN_B2);
    T0IF = 0;
}

//
void transceiver_init()
{
    gpio_init();
    timer_init();
}

/**
 * Function execute transceiver events
 */
void transceiver_event_handler()
{
    (*event)();
}

/**
 * Function going to endless loop and wait for trigger
 */
void transceiver_wait()
{
    return;
}

/**
 * Function produce pulse_no pulses on PWM output pins
 * @param pulse_no - Number of pulses that will be produced
 */
void transceiver_transmit(int8 pulse_no)
{
    pwm_start();
    /* Transmit pulse_no pulses */
    TMR2IF = 0;
    while (pulse_no > 0)
    {
        if(TMR2IF)
        {
            pulse_no--;
            TMR2IF = 0; 
        }
    }
    pwm_stop();
    delay_us(100);
    //pwm_high_z();
}
/**
 * Function switch transceiver in listen mode
 * @return TRUE if echo is detected, FALSE if echo is not detected in Nms
 */
short transceiver_listen()
{
    /* Switch P1A - RC2 to high Z mode */
    set_tris_c(get_tris_c() | 0b00000100);
    /* Switch P1B - RD5 to logic zero */
    output_low(P1B);
    
    /**
     * Add listener code here
     */
    
    return 0;
}

/**
 * Function indicate trigger that starts new acquisition 
 */
void transceiver_trigger()
{
    output_high(ECHO);
    event = transceiver_wait;
    
    timer_start(1);
}

/**
 * Function indicate timeout condition
 */
void transceiver_timeout()
{
    char b;
    
    output_low(ECHO);
    event = transceiver_wait;
    // Enable trigger interrupt again in order to catch another trigger
    b = PORTB;                                      // Clear mismatch condition
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB0);
}

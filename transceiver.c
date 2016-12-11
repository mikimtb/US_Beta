#include "transceiver.h"
#include "pwm.h"
#include "gpio.h"
#include "gpio.c"
#include "timer.h"
#include "timer.c"
#include "comparator.h"
#include "comparator.c"

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

/**
 * Comparator change event handler
 * ECHO connected to the C2IN-, Vref connected internally to C2IN+
 * Comparator output is inverted
 * R2IN+ < R2IN- = HIGH
 * R2IN+ > R2IN- = LOW
 */
#INT_COMP2
void comparator_isr_handler()
{
    if (C2OUT == TRUE)
        event = transceiver_echo_above;
    else
        event = transceiver_echo_below;
    
    clear_interrupt(INT_COMP2);
}
//
void transceiver_init()
{
    comparator_init();
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

/**
 * Callback function that is called when echo goes above threshold voltage
 */
void transceiver_echo_above()
{
    output_high(PIN_B3);
    event = transceiver_wait;
}

/**
 * Callback function that is called when echo goes below threshold voltage
 */
void transceiver_echo_below()
{
    output_low(PIN_B3);
    event = transceiver_wait;
}
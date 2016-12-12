#include "transceiver.h"
#include "pwm.h"
#include "pwm.c"
#include "gpio.h"
#include "gpio.c"
#include "timer.h"
#include "timer.c"
#include "comparator.h"
#include "comparator.c"
#include "main.h"

//Type definitions
typedef void (*t_fptr)(void);

t_fptr event = transceiver_wait;

typedef enum
{
    WAIT_FOR_TRIGGER = 0,
    TRANSMIT = 1,
    LISTEN_2MS = 2,
    LISTEN_28MS = 3
} e_state_machine;

e_state_machine state = WAIT_FOR_TRIGGER;
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
    {
        event = transceiver_trigger;
        disable_interrupts(INT_RB);
    }
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
    pwm_init();
    comparator_init();
    gpio_init();
    timer_init();
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
    
    output_high(ECHO);                      // Transmission is done set ECHO to start listening
    delay_us(100);                          // Wait 100us to suppress ringing of transducer
}
/**
 * Function switch transceiver in listen mode
 */
void transceiver_listen()
{
    /* Switch P1A - RC2 to high Z mode */
    set_tris_c(get_tris_c() | 0b00000100);
    /* Switch P1B - RD5 to logic zero */
    output_low(P1B);
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
 * Function indicate trigger that starts new acquisition 
 */
void transceiver_trigger()
{
    timer_start(2);                         // Start timeout to detect objects that are closer than 34cm
    state = LISTEN_2MS;                     // Set state machine state

    transceiver_transmit(7);                // Transmit 7 pulses
    transceiver_listen();                   // Switch the transceiver to listening mode
    
    comparator_enable();                    // Enable comparator module
    
    event = transceiver_wait;
    
}

/**
 * Function indicate timeout condition
 */
void transceiver_timeout()
{
    output_low(ECHO);                       // Timeout exception reset ECHO
    comparator_disable();                   // Disable comparator because timeout exception is caught
    
    // Enable trigger interrupt again in order to catch another trigger
    gpio_trigger_enable();
    
    state = WAIT_FOR_TRIGGER;
    
    event = transceiver_wait;
}

/**
 * Callback function that is called when echo goes above threshold voltage
 */
void transceiver_echo_above()
{
    if (state == LISTEN_28MS)
    {
        output_low(ECHO);
        // The echo is detected so all modules but trigger detection should be disabled
        timer_stop();
        comparator_disable();
        gpio_trigger_enable();
        
        state = WAIT_FOR_TRIGGER;
        
    }
    event = transceiver_wait;
}

/**
 * Callback function that is called when echo goes below threshold voltage
 */
void transceiver_echo_below()
{
    if(state == LISTEN_2MS)
    {
        timer_start(28);                        // Obstacle is not detected in dead zone of 34cm
                                                // so timer tick should be prevented and timeout
                                                // should be set to 28ms 
        state = LISTEN_28MS;
        setup_vref(VREF_HIGH | VREF_2_65625V);  // Change threshold to catch echo
    }
    else if(state == LISTEN_28MS)
    {
        output_low(ECHO);
        // The echo is detected so all modules but trigger detection should be disabled
        timer_stop();
        comparator_disable();
        gpio_trigger_enable();
        
        state = WAIT_FOR_TRIGGER;
    }
    event = transceiver_wait;
}
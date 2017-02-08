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
    LISTEN_3MS = 3,
    LISTEN_4MS = 4,
    LISTEN_9MS = 5,
    RESET_MCU = 6
} e_state_machine;

e_state_machine state = WAIT_FOR_TRIGGER;
//Private variables
int timeout = 0;

// Private functions

/**
 * GPIO interrupt handler. Low to High edge event handler raise this function
 */
#ifdef DEBUG
#INT_RB
#else
#INT_RA
#endif
void gpio_isr_handler()
{
    char b;
    
    if(input(TRIG))
    {
        event = transceiver_trigger;
#ifdef DEBUG
        disable_interrupts(INT_RB);
#else
        disable_interrupts(INT_RA);
#endif
    }
#ifdef DEBUG
    b = PORTB;                                      // Clear mismatch condition
#else
    b = PORTA;                                      // Clear mismatch condition
#endif
}

/**
 * Timer tick event handler
 */
#INT_RTCC
void timer_tick(void)
{
    disable_interrupts(GLOBAL);
    set_timer0(10);
    timeout--;
    if (timeout == 0)
    {
        disable_interrupts(INT_RTCC);
        event = transceiver_timeout;
    }
    T0IF = 0;
    enable_interrupts(GLOBAL);
}

/**
 * Comparator change event handler
 * ECHO connected to the C2IN-, Vref is connected internally to C2IN+
 * Comparator output is inverted
 * R2IN+ < R2IN- = HIGH
 * R2IN+ > R2IN- = LOW
 */
#INT_COMP2
void comparator_isr_handler()
{
    disable_interrupts(GLOBAL);
    if (state != LISTEN_2MS && state != WAIT_FOR_TRIGGER && state != TRANSMIT)
    {
        output_low(ECHO);
        // The echo is detected so all modules but trigger 
        // detection should be disabled
        timer_stop();
        comparator_disable_int();
        comparator_disable_module();
        gpio_trigger_enable();

        state = RESET_MCU;
    }
   
    clear_interrupt(INT_COMP2);
    enable_interrupts(GLOBAL);
}

//
void transceiver_init()
{
    pwm_init();
    gpio_init();
    timer_init();
}

/**
 * Reset all flags after initialization is done
 */
void transceiver_ready()
{
    output_low(ECHO);
    timer_stop();
    comparator_disable_int();
    gpio_trigger_enable();
        
    state = WAIT_FOR_TRIGGER;
        
    event = transceiver_wait;
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
}
/**
 * Function switch transceiver in listen mode
 */
void transceiver_listen()
{
#ifdef DEBUG
    /* Switch P1A - RC2 to high Z mode */
    set_tris_c(get_tris_c() | 0b00000100);
#else
    /* Switch P1A - RC5 to high Z mode */
    set_tris_c(get_tris_c() | 0b00100000);
#endif
    /* Switch P1B to logic zero */
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
    disable_interrupts(GLOBAL);
    timer_start(2);                         // Start timeout to detect objects that are closer than 34cm
    state = LISTEN_2MS;                     // Set state machine state

    transceiver_transmit(7);                // Transmit 7 pulses
    transceiver_listen();                   // Switch the transceiver to listening mode
    
    event = transceiver_wait;
    enable_interrupts(GLOBAL);
}

/**
 * Function indicate timeout condition
 */
void transceiver_timeout()
{
    disable_interrupts(GLOBAL);
    switch (state)
    {
        // Enable comparator module to start listening
        case LISTEN_2MS :
            timer_start(1);             // Wait one milliseconds
            delay_us(300);              // Wait for 300us to increase dead zone to 39cm     
            
            comparator_init();
            comparator_enable_int();

                                 

            state = LISTEN_3MS;
            break;
        // Reduce threshold after 3ms from transmission to detect far objects
        case LISTEN_3MS :
            timer_start(1);
            
            comparator_set_vref(VREF_3_125V);
            
            state = LISTEN_4MS;
            break;
        // Reduce threshold after 4ms from transmission to detect far objects
        case LISTEN_4MS :
            timer_start(5);
            
            comparator_set_vref(VREF_2_96875V);
            
            state = LISTEN_9MS;
            break;
        // Timeout there is no echo signal so stop listening    
        case LISTEN_9MS :
            output_low(ECHO);
            // The echo is detected so all modules but trigger detection should be disabled
            timer_stop();
            comparator_disable_int();
            comparator_disable_module();
            gpio_trigger_enable();
        
            state = RESET_MCU;
            break;
    }
        
    event = transceiver_wait;
    
    enable_interrupts(GLOBAL);
}
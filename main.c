#include "main.h"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
    /* Initialize PWM module*/
    transceiver_init();
    delay_ms(60);
    transceiver_ready();
    enable_interrupts(global);                  // Enable all interrupts
    
    while(TRUE)
    {
        transceiver_event_handler();
        /*output_toggle(ECHO);
        output_toggle(TRIG);
        delay_ms(100);*/
        /*if (C2OUT == TRUE)
        {
        output_low(DEBUG2);
        //event = transceiver_echo_above;
        }
        else
        {
        output_high(DEBUG2);
        //event = transceiver_echo_below;
        }*/
    }
}

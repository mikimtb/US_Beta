#include "main.h"
#include "transceiver.h"
#include "transceiver.c"
 long j = 0;
void main()
{
   
    /* Initialize PWM module*/
    transceiver_init();
    delay_ms(60);
    transceiver_ready();
    enable_interrupts(global);                  // Enable all interrupts
    
    //set_tris_d(get_tris_d() | 0b10000000);
    
    while(TRUE)
    {
        if (state == WAIT_FOR_TRIGGER)
        {
            delay_ms(50);
            j=0;
            transceiver_trigger();
        }
        transceiver_event_handler();
        //damp_ringing();
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

#include "main.h"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
   
    /* Initialize PWM module*/
    transceiver_init();
    delay_ms(1);
    transceiver_ready();
    enable_interrupts(global);                  // Enable all interrupts
   
    while(TRUE)
    {
#ifdef INTERNAL_TRIGGER
        if (state == WAIT_FOR_TRIGGER)
        {
            delay_ms(50);
            transceiver_trigger();
        }
#endif
        if (state == RESET_MCU)
        {
            delay_us(500);
            reset_cpu();
        }
        transceiver_event_handler();
       
    }
}

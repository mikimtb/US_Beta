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
    }
}

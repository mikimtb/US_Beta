#include "main.h"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
    /* Initialize PWM module*/
    transceiver_init();
    enable_interrupts(global);                  // Enable all interrupts
    
    while(TRUE)
    {
        transceiver_event_handler();
    }
}

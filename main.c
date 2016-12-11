#include "main.h"
#include "pwm.h"
#include "pwm.c"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
    /* Initialize PWM module*/
    pwm_init();
    transceiver_init();
    enable_interrupts(global);                  // Enable all interrupts
    
    while(TRUE)
    {
        transceiver_event_handler();
    }
}

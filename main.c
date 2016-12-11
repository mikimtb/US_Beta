#include "main.h"
#include "gpio.h"
#include "gpio.c"
#include "pwm.h"
#include "pwm.c"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
    /* Initialize PWM module*/
    pwm_init();
    gpio_init();
    enable_interrupts(global);                  // Enable all interrupts
    
    while(TRUE)
    {
        if (trigger)
        {
            output_toggle(ECHO);
            trigger = 0;
            /*transceiver_transmit(7);
            transceiver_listen();*/
            //delay_ms(1000);
        }
    }
}

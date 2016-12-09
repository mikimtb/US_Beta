#include "main.h"
#include "pwm.h"
#include "pwm.c"
#include "transceiver.h"
#include "transceiver.c"

void main()
{
    /* Initialize PWM module*/
    pwm_init();
    
    while(TRUE)
    {
        output_toggle(PIN_B0);
        transceiver_transmit(7);
        transceiver_listen();
        delay_ms(1000);
    }
}

#include "transceiver.h"
#include "pwm.h"

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
    delay_us(100);
    //pwm_high_z();
}
/**
 * Function switch transceiver in listen mode
 * @return TRUE if echo is detected, FALSE if echo is not detected in Nms
 */
short transceiver_listen()
{
    /* Switch P1A - RC2 to high Z mode */
    set_tris_c(get_tris_c() | 0b00000100);
    /* Switch P1B - RD5 to logic zero */
    output_low(P1B);
    
    /**
     * Add listener code here
     */
    
    return 0;
}

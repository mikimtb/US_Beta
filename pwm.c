#include "pwm.h"

// Global defines
#ifdef DEBUG
#define P1A     PIN_C2
#define P1B     PIN_D5
#else
#define P1A     PIN_C5
#define P1B     PIN_C4
#endif

// Registers definitions
/* Peripheral interrupt register */
#byte PIR1=0x0c
#bit TMR2IF=PIR1.1
/* Timer 2 control register */
#byte T2CON=0x12
#bit TMR2ON=T2CON.2
/* Timer 2 register */
#byte TMR2=0x11

/**
 * Function initialize PWM pair deadband delay
 * @param deadband  0-127 the number represent PWM dead band delay time
 */
void pwm_set_deadband(int8 deadband)
{  
    #byte PWM1CON = 0x9B 
    if(deadband > 127) 
        deadband = 127; 
    PWM1CON = deadband;
}
/**
 * Function initialize PWM interface to produce 40KHz PWM
 * The module is set to operate in HALF_BRIDGE_MODE
 * The PWM pins pair are set to be complementary on to another
 */
void pwm_init()
{
    //setup_ccp1(CCP_PWM_HALF_BRIDGE | CCP_PWM_H_H);
    pwm_high_z();
    setup_timer_2(T2_DIV_BY_1,49,1);                                    //25.0 us overflow, 25.0 us interrupt
    //TMR2ON = 0;
    //TMR2 = 0;
    set_pwm1_duty((int16)98);                                           // Set PWM Duty cycle to 50%
    pwm_set_deadband(1); 
}
/**
 * Function enable PWM outputs
 */
void pwm_start()
{
    /* Prepare timer and enable outputs */
    //setup_ccp1(CCP_PWM_HALF_BRIDGE | CCP_PWM_H_H);
    //TMR2 = 0;
    TMR2IF = 0;
    //TMR2ON = 1;
    while (!TMR2IF);
    setup_ccp1(CCP_PWM_HALF_BRIDGE | CCP_PWM_H_H);
    //output_drive(P1A);
    //output_drive(P1B);
}
/**
 * Function disable tris register and pwm operation
 */
void pwm_stop()
{
    setup_ccp1(CCP_OFF);
    output_low(P1A);
    output_low(P1B);
    //TMR2ON = 0;
    
}
/**
 * Function setting PWM output pins in high Z state
 */
void pwm_high_z()
{
#ifdef DEBUG
    set_tris_c(get_tris_c() | 0b00000100);
    set_tris_d(get_tris_d() | 0b00100000);
#else
    set_tris_c(get_tris_c() | 0b00110000);
#endif
}
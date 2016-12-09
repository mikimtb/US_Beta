/* 
 * File:   pwm.h
 * Author: Miroslav Bozic
 *
 * Created on October 31, 2016, 7:42 PM
 */

#ifndef PWM_H
#define	PWM_H

#define P1A     PIN_C2
#define P1B     PIN_D5

/* Peripheral interrupt register */
#byte PIR1=0x0c
#bit TMR2IF=PIR1.1
/* Timer 2 control register */
#byte T2CON=0x12
#bit TMR2ON=T2CON.2
/* Timer 2 register */
#byte TMR2=0x11

/*Function prototypes*/
void pwm_set_deadband(int8 deadband);
void pwm_init(void);
void pwm_start(void);
void pwm_stop(void);
void pwm_high_z(void);



#endif	/* PWM_H */


/* 
 * File:   pwm.h
 * Author: Miroslav Bozic
 *
 * Created on October 31, 2016, 7:42 PM
 */

#ifndef PWM_H
#define	PWM_H

/*Function prototypes*/
void pwm_set_deadband(int8 deadband);
void pwm_init(void);
void pwm_start(void);
void pwm_stop(void);
void pwm_high_z(void);



#endif	/* PWM_H */


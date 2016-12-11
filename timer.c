#include "timer.h"

/**
 * Function initialize the timer
 */
void timer_init(void);

/**
 * Timer tick event handler
 */
#INT_RTCC
void timer_tick(void);

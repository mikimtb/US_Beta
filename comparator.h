/* 
 * File:   comparator.h
 * Author: Miroslav Bozic
 *
 * Created on December 11, 2016, 11:12 PM
 */

#ifndef COMPARATOR_H
#define	COMPARATOR_H

void comparator_init(void);
void comparator_set_vref(int vref);
void comparator_disable_module();
void comparator_enable_int(void);
void comparator_disable_int(void);

#endif	/* COMPARATOR_H */


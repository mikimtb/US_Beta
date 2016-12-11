#include "comparator.h"

void comparator_init()
{
    setup_comparator(CP1_B3_VREF | CP2_A0_VREF | CP2_INVERT);
    setup_vref(8);                                  // Sets internal Vref to 2.5V
    
    clear_interrupt(INT_COMP2);                     // Clear interrupt flag and enable 
    enable_interrupts(INT_COMP2);                   // Comparator 2 interrupt on change
}

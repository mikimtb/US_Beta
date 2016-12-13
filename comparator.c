#include "comparator.h"

// Enumerations
enum
{
    VREF_2_5V = 8,
    VREF_2_65625V = 9,
    VREF_2_8125V = 10,
    VREF_2_96875V = 11,
    VREF_3_125V = 12,
    VREF_3_28125V = 13,
    VREF_3_4375V = 14,
    VREF_3_59375V = 15
};

/**
 * Function initialize comparator 2 module to be used for echo detection
 */
void comparator_init()
{
    setup_comparator(CP1_B3_VREF | CP2_A0_VREF | CP2_INVERT);
    setup_vref(VREF_HIGH | VREF_3_59375V);              // Sets internal Vref to 3.6V
}

/**
 * Function enables comparator 2 module and set threshold voltage to 3.6V
 */
void comparator_enable()
{
    clear_interrupt(INT_COMP2);                     // Clear interrupt flag and enable 
    enable_interrupts(INT_COMP2);                   // Comparator 2 interrupt on change
}

/**
 * Function disable comparator 2 module
 */
void comparator_disable()
{
    clear_interrupt(INT_COMP2);
    disable_interrupts(INT_COMP2);
}
#include "comparator.h"
#include "gpio.h"

/** Enumerations for reference voltage selection
 * Vref             VR3:VR0         CVref
 * 2.5              8               HIGH
 * 2.65625          9               HIGH
 * 2.708333333      13              LOW
 * 2.8125           10              HIGH
 * 2.916666667      14              LOW
 * 2.96875          11              HIGH    
 * 3.125            12              HIGH    
 * 3.28125          13              HIGH          
 * 3.4375           14              HIGH     
 * 3.59375          15              HIGH        
 */
enum
{
    VREF_2_5V = 1,
    VREF_2_65625V = 2,
    VREF_2_7083V = 3,
    VREF_2_8125V = 4,
    VREF_2_9167 = 5,
    VREF_2_96875V = 6,
    VREF_3_125V = 7,
    VREF_3_28125V = 8,
    VREF_3_4375V = 9,
    VREF_3_59375V = 10
};

/**
 * Function initialize comparator 2 module to be used for echo detection
 */
void comparator_init()
{
#ifdef DEBUG
    setup_comparator(CP1_B3_VREF | CP2_A0_VREF | CP2_INVERT);
#else
    setup_comparator(A1_VR_C1_VR | COMP_INVERT);
#endif
    comparator_set_vref(VREF_3_28125V);
}

/**
 * Function set selected reference voltage of internal reference generator
 * @param vref  1 - 10, enum describe the available voltages that can be selected
 */
void comparator_set_vref(int vref)
{
    switch(vref)
    {
        case VREF_2_5V :
            setup_vref(VREF_HIGH | 8);
            break;
        case VREF_2_65625V :
            setup_vref(VREF_HIGH | 9);
            break;
        case VREF_2_7083V :
            setup_vref(VREF_LOW | 13);
            break;
        case VREF_2_8125V :
            setup_vref(VREF_HIGH | 10);
            break;
        case VREF_2_9167 :
            setup_vref(VREF_LOW | 14);
            break;
        case VREF_2_96875V :
            setup_vref(VREF_HIGH | 11);
            break;
        case VREF_3_125V :
            setup_vref(VREF_HIGH | 12);
            break;
        case VREF_3_28125V :
            setup_vref(VREF_HIGH | 13);
            break;
        case VREF_3_4375V :
            setup_vref(VREF_HIGH | 14);
            break;
        case VREF_3_59375V :
            setup_vref(VREF_HIGH | 15);
            break;     
    }
}

/**
 * Function disable comparator module at all
 */
void comparator_disable_module()
{
    setup_comparator(NC_NC);                        // Disable comparator module
    gpio_init();                                    // Re init gpio module
}

/**
 * Function enables comparator 2 module and set threshold voltage to 3.6V
 */
void comparator_enable_int()
{
    clear_interrupt(INT_COMP2);                     // Clear interrupt flag and enable 
    enable_interrupts(INT_COMP2);                   // Comparator 2 interrupt on change
}

/**
 * Function disable comparator 2 module
 */
void comparator_disable_int()
{
    clear_interrupt(INT_COMP2);
    disable_interrupts(INT_COMP2);
}


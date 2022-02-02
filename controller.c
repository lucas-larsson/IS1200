/** 
 * controller.c
 * Functions for retrieving input states from the I/O shield
 * 
 * @author Alex Lindberg
 * @author Lucas Larsson
*/
#include "controller.h"
#include <stdint.h>

int get_switches()
{
    return (PORTD >> 8) & 0x000F;
}

int get_buttons()
{
    /* Bits are ordered as 3:0 -> BTN4:BTN1 */
    return 0x0 | (PORTD >> 4) & (0x07 << 1) | (PORTF & 0x02) >> 1;
}
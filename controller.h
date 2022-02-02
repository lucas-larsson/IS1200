/** 
 * controller.h
 * Functions for retrieving input states from the I/O shield
 * 
 * @author Alex Lindberg
 * @author Lucas Larsson
*/
#include <pic32mx.h>

/** 
 * @author  Alex Lindberg
 * @author  Lucas Larsson
 * @brief   Retrieves the state of the I/O boards' switches 
 *          This function was written as part of Lab3, course IS1200.
 * 
 * @return  (int) containing the switch states.
 *          The first bit, bit 0, corresponds to switch 1.
 *          Bits 1:3 corresponds to switches 2-4 in order.
 * */
int get_switches();

/** 
 * @author  Alex Lindberg
 * @author  Lucas Larsson
 * @brief   Retrieves the state of the I/O boards' buttons 
 *          This function was written as part of Lab3, course IS1200.
 * 
 * @return  (int) containing the button states.
 *          The first bit, bit 0, corresponds to button 1.
 *          Bits 1:3 corresponds to buttons 2-4 in order.
 * */
int get_buttons();
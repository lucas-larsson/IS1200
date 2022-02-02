/** 
 * main.h
 * This file written 2015 by F. Lundevall
 * Some parts are original code written by Axel Isaksson
 * 
 * @author Axel Isaksson
 * @author F. Lundevall
 * @author Alex Lindberg
*/

/* -------------------------------------------- */
/* ----------------- Includes ----------------- */
//#include <pic32mx.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "display.h"
#include "controller.h"
#include "pong.h"
#include "pong_ai.h"
#include "score.h"

/* --------------------------------------------- */
/* ---------------- Definitions ---------------- */

#define CLOCKFREQ 80000000    // 80 MHz
#define TIMEOUTPERIOD 0.00001 // 10 us
#define TMR2PRESCALER 16      // 5 MHz
#define TMR2PERIOD ((CLOCKFREQ / TMR2PRESCALER) * TIMEOUTPERIOD)

#define WIN_SCORE 3

/* -------------------------------------------- */
/* ------- Extern variable declarations ------- */

extern const float P_WIDTH;
extern const float P_HEIGHT;
extern const float B_WIDTH;
extern const float B_HEIGHT;
extern const int SCREEN_OFFSET;

/* --------------------------------------------- */
/* ----------- Variable declarations ----------- */

typedef enum Game_state
{
    MENU = 0,
    GAME_PVP = 1,
    GAME_PVM = 2,
    SCOREBOARD = 3,
    ACCEPT = 4
} currentState;

/* --------------------------------------------- */
/* ----------- Function declarations ----------- */

/** 
 * @author  Alex Lindberg
 * @author  Lucas Larsson
 * @brief   enables interrupts globaly.
*/
void enable_interrupt();

/** 
 * @author  Axel Isaksson
 * @author  F. Lundevall
 * @author  Alex Lindberg
 * @brief   Initializes the microcontroller unit.
*/
void initialize_system();

/** 
 * @author  Alex Lindberg
 * @author  Lucas Larsson
 * @brief   Initializes timer.
 *          This function was written as part of Lab3, course IS1200. 
*/
void initialize_timer();

/** 
 * @author  Alex Lindberg
 * @author  Lucas Larsson
 * @brief   Interrupt handling routing.
 *          This function was written as part of Lab3, course IS1200. 
*/
void user_isr();
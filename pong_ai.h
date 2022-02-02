/**
 * pong_ai.h
 * 
 * Contains information related to the A.I. component of the game.
 * 
 * @author Alex Linberg
*/
#include "pong.h"

/* -------------------------------------------- */
/* ------- Extern variable declarations ------- */

extern const float P_WIDTH;
extern const float P_HEIGHT;
extern const float B_WIDTH;
extern const float B_HEIGHT;
extern const int SCREEN_OFFSET;

/* --------------------------------------------- */
/* ---------------- Definitions ---------------- */

#define LOOK_DIST 25 + SCREEN_OFFSET
#define PREDICT_DEPTH 10

/* --------------------------------------------- */
/* ----------- Function declarations ----------- */

/**
 * @brief main control function for the A.I.
 * 
 * @param ai                    The paddle acting as A.I.
 * @param b                     The ball
 * @param current_switch_state  A.I. intelligence
*/
void pong_ai_run(struct paddle *ai, struct ball *b, int current_switch_state);

/**
 * @brief easiest AI mode. Simply moves up and down.
 * 
 * @param ai    the paddle acting as ai
 * @param b     the ball
*/
void pong_ai_smooth_brain(struct paddle *ai, struct ball *b);

/**
 * @brief normal AI mode. Tracks ball direction and tries to bounce it.
 * 
 * @param ai    the paddle acting as ai
 * @param b     the ball
*/
void pong_ai_normal_brain(struct paddle *ai, struct ball *b);

/**
 * @brief hardest AI mode. Tracks ball movement and direction.
 * 
 * @param ai    the paddle acting as ai
 * @param b     the ball
*/
void pong_ai_galaxy_brain(struct paddle *ai, struct ball *b);

void pong_ai_reset(struct paddle *ai);
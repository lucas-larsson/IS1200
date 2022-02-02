/**
 * pong.h
 * 
 * Header file to pong.c
 * Contains game logic and structs for the paddles and the ball.
 * 
 * @author Alex Lindberg
 * 
*/
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* ----------------------------------------------------- */
/* -------------------- Definitions -------------------- */

/* PI constant */
#define M_PI 3.14159265358979323846
/* Calculates the sign of a number */
#define SIGN(num) fabs(num) / num
/* Maps a values from one range of numbers onto another */
#define MAP(v, x0, y0, x1, y1) (x1 + ((y1 - x1) / (y0 - x0)) * (v - x0))
/* Converts degrees to radians */
#define TO_RAD(a) (a * (M_PI / 180.0))
/* The maximum angle of reflection in degrees */
#define MAX_REFLECT_ANGLE 45

#define BALL_START_POS_X 63.f
#define BALL_START_POS_Y 15.f
#define BALL_START_VX 2.0f
#define BALL_START_VY 0.f
#define BALL_START_VX_MAX 2.5f
#define BALL_START_VY_MAX 1.5f
#define BALL_START_VX_MIN 1.0f
#define BALL_START_VY_MIN 0.f

/* ----------------------------------------------------- */
/* ---------------------- Structs ---------------------- */

#ifndef PADDLE_HEADER
#define PADDLE_HEADER
/**
 * @brief Struct containing paddle information
 * @author Alex Lindberg
*/
struct paddle
{
    char Player;    // The name of the player
    uint8_t score;  // Current score
    bool is_ai;     // Whether the player is computer controlled
    float x, y;     // positional variables
} player1, player2;

#endif /* PADDLE_HEADER */

#ifndef BALL_HEADER
#define BALL_HEADER
/**
 * @brief Struct containing ball information
 * @author Alex Lindberg
*/
struct ball
{
    float x, y;           // positional variables
    float vx, vy;         // horizontal and vertical velocity
    float vx_MAX, vy_MAX; // Maximum velocity variables
    float vx_MIN, vy_MIN; // Minimum velocity variables
} the_ball;

#endif /* BALL_HEADER */

/* ----------------------------------------------------- */
/* --------------- Function declarations --------------- */

/**
 * @brief   Initializes a player
 * 
 * @param player    The player to initialize. 
 * @param is_ai     should be 'true' if the player is controlled by a human,
 *                  or 'false' if it's controlled by the computer.
 * @param name      The player's name.
*/
void pong_create_player(struct paddle *player, bool is_ai, char name);

/**
 * @brief   Initializes a ball. The coorinates for initialization are
 *          always the same, i.e. the middle of the playing field.
 * 
 * @param b         The ball to initialize.
*/
void pong_create_ball(struct ball *b);

/**
 * @brief   Initializes the game simply by calling pong_create_player() and
 *          pong_create_ball().
 * 
 * @param p1        The first player to initialize. 
 * @param p2        The second player to initialize. 
 * @param b         The ball.
*/
void pong_initialize_game(struct paddle *p1, struct paddle *p2, struct ball *b, int current_mode);

/**
 * @brief   Moves the ball.
 *          This function also checks for collisions and decides how the ball
 *          reacts in such an event.
 * 
 * @param p1            The first player to initialize. 
 * @param p2            The second player to initialize. 
 * @param b             The ball.
 * @param dt            delta time. Left for future work.
 * @param current_mode  the selected game mode
*/
void pong_move_ball(struct paddle *p1, struct paddle *p2, struct ball *b, float dt);

/**
 * @brief   Sets the velocity of a ball.
 * 
 * @param b         The target ball to move.
 * @param vx1       The horizontal velocity to set.
 * @param vy1       The vertical velocity to set.
*/
void pong_set_ball_velocity(struct ball *b, float vx1, float vy1);

/**
 * @brief   Updates the position of a player.
 * 
 * @param player    The target player to move.
 * @param vy1       The vertical velocity to set.
 * @param dt        delta time. Left for future work.
*/
void pong_move_paddle(struct paddle *player, float vy1, float dt);

/**
 * @brief   Sets the score of the target player.
 * 
 * @param player    The target player.
 * @param s         The score to set.
*/
void pong_set_score(struct paddle *player, int s);

/**
 * @brief   Resets the score of the target player to 0.
 * 
 * @param player    The target player.
*/
void pong_reset_score(struct paddle *player);

/**
 * @brief   Increments the score of the target player by 1.
 * 
 * @param player    The target player.
*/
void pong_increment_score(struct paddle *player);

/**
 * @brief  retrieves the sign of a number
 * 
 * @param num   the number
 * @return      the sign of num
*/
int math_get_sign(float num);
/**
 * pong.c
 * 
 * Contains game logic.
 * 
 * @author Alex Lindberg
*/
#include "pong.h"

/* --------------------------------------------- */
/* -------------- Global variables -------------- */

const float P_WIDTH = 4.f;   // Paddle width in pixels
const float P_HEIGHT = 10.f; // Paddle height in pixels

const float B_WIDTH = 2.f;  // Ball width in pixels
const float B_HEIGHT = 2.f; // Ball height in pixels

const int SCREEN_OFFSET = 16; // Number of pixels from screen
                              //  edge to the actual playingfield

/* --------------------------------------------- */
/* -------------- Local variables -------------- */

static float relative_intersection_y;
static float reflect_angle;

static int current_sign_x;
static int current_sign_y;

static bool boosted_ball = false;

/* --------------------------------------------- */
/* -------------- Local functions -------------- */

/**
 * @brief Checks whether a ball and paddle are colliding using axis-aligned bounding boxes
 * 
 * @param b         the ball
 * @param player    the player
*/
static int check_ball_collision(struct ball *b, struct paddle *player);

/** 
 * @brief Calculates the angle of reflection
 * 
 * @param ball_y    the balls current y position
 * @param paddle_y  the paddles current y position
*/
static float calc_ball_trajectory(float ball_y, float paddle_y);

/* ---------------------------------------------- */
/* ------------ Function definitions ------------ */

void pong_create_player(struct paddle *player, bool is_ai, char name)
{
    // Check if it's an AI
    if (is_ai)
    {
        player->is_ai = true;
        player->Player = 'A';
    }
    else
    {
        player->is_ai = false;
        player->Player = name;
    }

    // Create new player
    switch (player->Player)
    {
    case 'A':
    case '1':
    {
        player->x = 127.f - P_WIDTH - SCREEN_OFFSET;
        player->y = 15.f - (P_HEIGHT / 2.f);
        pong_reset_score(player);
    }
        break;
    case '2':
    {
        player->x = SCREEN_OFFSET;
        player->y = 15.f - (P_HEIGHT / 2.f);
        pong_reset_score(player);
    }
        break;
    default:
        break;
    }
}

void pong_create_ball(struct ball *b)
{
    b->x = BALL_START_POS_X;
    b->y = BALL_START_POS_Y;
    b->vx = BALL_START_VX;
    b->vy = BALL_START_VY;
    b->vx_MAX = BALL_START_VX_MAX;
    b->vy_MAX = BALL_START_VY_MAX;
    b->vx_MIN = BALL_START_VX_MIN;
    b->vy_MIN = BALL_START_VY_MIN;
}

void pong_initialize_game(struct paddle *p1, struct paddle *p2, struct ball *b, int current_mode)
{
    switch (current_mode)
    {
    case 1: // PVP
        pong_create_player(p1, false, '1');
        pong_create_player(p2, false, '2');
        break;
    case 2: // PVM
        pong_create_player(p1, true, 'A');
        pong_create_player(p2, false, '2');
        break;
    default:
        break;
    }
    pong_create_ball(b);
}

void pong_move_ball(struct paddle *p1, struct paddle *p2, struct ball *b, float dt)
{
    b->x += b->vx * dt;
    b->y += b->vy * dt;

    /* Player 1 scores */
    if (b->x < SCREEN_OFFSET - 1)
    {
        b->x = 63.f;
        b->y = 15.f;
        pong_set_ball_velocity(b, -(b->vx), 0.f);
        pong_increment_score(p1);
    }
    /* Player 2 scores */
    else if (b->x > 127 - SCREEN_OFFSET)
    {
        b->x = 63.f;
        b->y = 15.f;
        pong_set_ball_velocity(b, -(b->vx), 0.f);
        pong_increment_score(p2);
    }
    // Bounce on the floor and ceiling
    if (b->y <= 0.f || b->y >= 31.f)
    {
        pong_set_ball_velocity(b, b->vx, -(b->vy));
    }

    /* Ball-paddle collision detection and reaction */

    // RIGHT PADDLE BOUNCE
    if (check_ball_collision(b, p1))
    {
        // calculate the new trajectory
        reflect_angle = calc_ball_trajectory(b->y, p1->y);
        // set the velocity
        pong_set_ball_velocity(b, b->vx_MAX * cos(reflect_angle), b->vy_MAX * sin(reflect_angle));
        b->x = p1->x - B_WIDTH - 1;
    }

    // LEFT PADDLE BOUNCE
    if (check_ball_collision(b, p2))
    {
        // calculate the new trajectory
        reflect_angle = calc_ball_trajectory(b->y, p2->y);
        // set the velocity, x-velocity is reversed
        pong_set_ball_velocity(b, b->vx_MAX * -cos(reflect_angle), b->vy_MAX * sin(reflect_angle));
        b->x = p2->x + P_WIDTH + 1;
    }
}

/*
    We always want to set the velocity, not add to current velocity, 
    because it allows for better control of where the ball is going when
    we collide with the paddles. Upon collison we perform some calculations
    to determine which direction the ball is going based on its distance
    from the middle of the paddle. */
void pong_set_ball_velocity(struct ball *b, float vx1, float vy1)
{
    current_sign_x = math_get_sign(vx1);
    current_sign_y = math_get_sign(vy1);
    
    if (fabs(vx1) > b->vx_MAX)
        vx1 = b->vx_MAX * current_sign_x;
    else if (fabs(vx1) < b->vx_MIN)
        vx1 = b->vx_MIN * current_sign_x;
    if (fabs(vy1) > b->vy_MAX)
        vy1 = b->vy_MAX * current_sign_y;
    else if (fabs(vy1) < b->vy_MIN)
        vy1 = b->vy_MIN * current_sign_y;

    // Update velocities
    b->vx = vx1;
    b->vy = vy1;
}

void pong_move_paddle(struct paddle *player, float vy1, float dt)
{
    player->y += vy1 * dt;
}

void pong_set_score(struct paddle *player, int s)
{
    player->score = s;
}

void pong_reset_score(struct paddle *player)
{
    player->score = 0;
}

void pong_increment_score(struct paddle *player)
{
    player->score += 1;
}

int math_get_sign(float num)
{
    num = (num >= 0) ? 1 : -1;
    return num;
}

/* Returns true if the ball and paddle are colliding */
static int check_ball_collision(struct ball *b, struct paddle *player)
{
    return ((b->x + b->vx) < (player->x + P_WIDTH) && (b->x + B_WIDTH + b->vx) > (player->x) &&
            (b->y + b->vy) < (player->y + P_HEIGHT) && (b->y + B_HEIGHT + b->vy) > (player->y));
}

/* Returns the angle of reflection */
static float calc_ball_trajectory(float ball_y, float paddle_y)
{
    // get the relative point of intersection on the y axis
    relative_intersection_y = (ball_y + B_HEIGHT / 2) - (paddle_y - P_HEIGHT / 2);
    // If we hit on the lower part of the paddle the ball goes down, otherwise it goes up
    return MAP(relative_intersection_y, 0, P_HEIGHT, TO_RAD(180 + MAX_REFLECT_ANGLE), TO_RAD(180 - MAX_REFLECT_ANGLE));
}
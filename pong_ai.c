/** 
 * pong_ai.c
 * 
 * Contains ai logic for the game
 * 
 * @author Alex Lindberg
*/
#include "pong_ai.h"

/* --------------------------------------------- */
/* -------------- Local variables -------------- */

static float current_ai_direction = 1.0f;
static float next_ai_y;
static float next_ball_y;
static float distance_ball_to_ai;
static int lower_y, upper_y;

/* ---------------------------------------------- */
/* ------------ Function definitions ------------ */

void pong_ai_run(struct paddle *ai, struct ball *b, int current_switch_state)
{
    if (current_switch_state & 0x4)
    {
        pong_ai_normal_brain(ai, b);
    }
    else if (current_switch_state & 0x8)
    {
        pong_ai_galaxy_brain(ai, b);
    }
    else
    {
        pong_ai_smooth_brain(ai, b);
    }
}

void pong_ai_smooth_brain(struct paddle *ai, struct ball *b)
{
    pong_move_paddle(ai, current_ai_direction, 1.f);
    next_ai_y = ai->y + current_ai_direction;
    if (next_ai_y + P_HEIGHT > 31 || next_ai_y < 0)
        current_ai_direction *= -1.f;
}

static float predict_next_ball_y(float current_x, float current_y, float dx, float dy)
{
    float final_y = current_y + (B_HEIGHT / 2);
    int i;
    for (i = 0; i < PREDICT_DEPTH; i++)
    {
        current_x += dx;
        if (current_x > 127 - P_WIDTH)
            break;
        if (final_y + dy > 31 - (B_HEIGHT / 2) || final_y + dy < (B_HEIGHT / 2))
            dy *= -1;
        final_y += dy;
    }
    return final_y;
}

void pong_ai_normal_brain(struct paddle *ai, struct ball *b)
{
    distance_ball_to_ai = fabs(ai->x - (b->x+B_WIDTH/2));

    if (distance_ball_to_ai < LOOK_DIST && b->vx > 0.5f)
    {
        lower_y = ai->y + (B_HEIGHT / 2);
        upper_y = ai->y + P_HEIGHT - (B_HEIGHT / 2);
        next_ball_y = predict_next_ball_y(b->x, b->y, b->vx, b->vy);
        if (upper_y + b->vy < 31 && lower_y + b->vy > 0)
        {
            current_ai_direction = b->vy;
            if (next_ball_y > upper_y)
                current_ai_direction = fabs(current_ai_direction) * 1.2f;
            else if( next_ball_y  < lower_y)
                current_ai_direction = fabs(current_ai_direction) * -1.2f;
            pong_move_paddle(ai, current_ai_direction, 1.f);
        }
    }
}

void pong_ai_galaxy_brain(struct paddle *ai, struct ball *b)
{
    lower_y = ai->y - (B_HEIGHT / 2);
    upper_y = ai->y + P_HEIGHT + (B_HEIGHT / 2);
    next_ai_y = ai->y + b->vy*1.2f;
    if (next_ai_y + (P_HEIGHT) < 31 && next_ai_y > 1)
    {
        current_ai_direction = b->vy;
        if (b->y + (B_HEIGHT / 2) > upper_y || b->y + (B_HEIGHT / 2) < lower_y)
            current_ai_direction *= 1.2f;
        if(!(fabs(b->y - (ai->y + (P_HEIGHT/2)) ) > (P_HEIGHT/2)))
            pong_move_paddle(ai, current_ai_direction, 1.f);
    }
}

void pong_ai_reset(struct paddle *ai)
{
    pong_create_player(ai, true, 'A');
    current_ai_direction = 1.f;
    next_ai_y = ai->y;
    next_ball_y = 15.f;
    distance_ball_to_ai = 60;
}
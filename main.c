/** 
 * main.c
 * This file written 2015 by F. Lundevall
 * Some parts are original code written by Axel Isaksson
 * 
 * @author Axel Isaksson
 * @author F. Lundevall
 * @author Alex Lindberg
 * @author Lucas Larsson
*/
#include <pic32mx.h>
#include "main.h"

/* --------------------------------------------- */
/* -------------- Local varaibles -------------- */

static int timeout_counter; //
static int timeout_flag;
static int button_state;
static int switch_state;

const currentState STATE_TABLE[5] =
    {
        MENU,
        GAME_PVP,
        GAME_PVM,
        SCOREBOARD,
        ACCEPT};

/* --------------------------------------------- */
/* ----------------- Main loop ----------------- */

int main()
{
    initialize_system();
    /* Display */
    display_init();

    /* Init */
    initialize_timer();
    enable_interrupt();

    currentState current_state = MENU;
    currentState selected_state = MENU;
    bool new_game = true;
    bool game_on = false;
    bool checking_highscores = false;
    int score_cp = 0;
    char highscore_log[SCOREBOARD_ENTRIES][SCORE_STR_SIZE + 1];   // array to display
    static uint8_t record[SCOREBOARD_ENTRIES][SCORE_RECORD_SIZE]; // array to store record info

    /* Main loop */
    while (1)
    {
        while (!timeout_flag)
        {
        };
        timeout_flag = 0;

        button_state = get_buttons();
        switch_state = get_switches();

        /* If we aren't in a game, meaning we are in the menu... */
        if (!game_on && !checking_highscores)
        {
            display_clear_screen();

            if (button_state & 0x1) // button 1
            {
                selected_state = GAME_PVP;
                quicksleep(10000);
            }
            else if (button_state & 0x2) // button 2
            {
                selected_state = GAME_PVM;
                quicksleep(10000);
            }
            else if (button_state & 0x4) // button 3
            {
                selected_state = SCOREBOARD;
                quicksleep(10000);
            }
            else if (button_state & 0x8) // button 4
            {
                if (selected_state != MENU)
                {
                    current_state = STATE_TABLE[selected_state];
                    selected_state = ACCEPT;
                }
                quicksleep(10000);
            }

            display_print_text("Menu:         ", 0, 0);
            switch (STATE_TABLE[selected_state])
            {
            case GAME_PVP:
                display_print_text("P1 vs. P2     ", 0, 8);
                break;
            case GAME_PVM:
                display_print_text("Player vs. AI ", 0, 8);
                break;
            case SCOREBOARD:
                display_print_text("Show highscore", 0, 8);
                break;
            case ACCEPT:
            {
                if (current_state == GAME_PVP || current_state == GAME_PVM)
                    game_on = true;
                else if (current_state == SCOREBOARD)
                    checking_highscores = true;
                break;
            }
            default:
                display_print_text("Press Btn1/2/3", 0, 8);
                break;
            }
            display_update();
        }

        /* When the game is playing */
        else if (game_on && !checking_highscores)
        {

            /* NEW GAME */
            if (new_game)
            {
                pong_initialize_game(&player1, &player2, &the_ball, current_state);
                new_game = false;
            }

            /* GAME RUNNING */
            if (((player1.score == WIN_SCORE || player2.score == WIN_SCORE) && !player1.is_ai) || (player1.is_ai && player1.score == WIN_SCORE)) /* If a player wins... */
            {
                display_draw_filled_rect(SCREEN_OFFSET, 1, 127 - SCREEN_OFFSET - 1, 30, 0);
                display_draw_filled_rect(0, 7, 127, 24, 0);
                // Print winner
                if (player1.score == WIN_SCORE)
                {
                    if (player1.is_ai)
                        display_print_text(" The A.I. wins ", 4, 7);
                    else
                        display_print_text(" Player 1 wins ", 4, 7);
                }
                else
                {
                    if (player2.is_ai)
                        display_print_text(" The A.I. wins ", 4, 7);
                    else
                        display_print_text(" Player 2 wins ", 4, 7);
                }
                display_print_text(" Btn1 to quit ", 4, 16);

                if (button_state & 0x1) // We want to exit the game and go back to main menu
                {
                    // Reset game parameters
                    game_on = false;
                    current_state = MENU;
                    selected_state = MENU;
                    new_game = true;
                    // record match score
                    if (player1.is_ai)
                    {
                        uint8_t new_record[4] = {0x32, 0x32, 0x32, player2.score};
                        int c = 0;
                        int current_letter = 0x2E; // This is a dot '.'
                        do
                        {
                            display_clear_screen();
                            button_state = get_buttons();
                            display_print_text("Name:        ", 1, 7);
                            display_print_text((char *)new_record, 4, 16);
                            display_draw_filled_rect(10 + c*8, 15, 128, 31, 0);
                            new_record[c] = current_letter;
                            if (button_state & 0x2)
                            {
                                current_letter++;
                                quicksleep(20000);
                                if (current_letter > 0x5A || current_letter < 0x41)
                                    current_letter = 0x41;
                            }
                            else if (button_state & 0x4)
                            {
                                current_letter--;
                                quicksleep(20000);
                                if (current_letter < 0x41)
                                    current_letter = 0x5A;
                            }
                            else if (button_state & 0x8 && current_letter != 0x2E)
                            {
                                c += 1;
                                current_letter = 0x2E;
                                quicksleep(5000);
                            }
                            display_update();
                        } while (c < 3);

                        score_append_new_record(record, new_record);
                        score_convert_to_strings(highscore_log, record);
                    }
                    pong_set_score(&player1, 0);
                    pong_set_score(&player2, 0);

                    if (player1.is_ai)
                        pong_ai_reset(&player1);
                    if (player2.is_ai)
                        pong_ai_reset(&player2);
                }
                display_update();
                quicksleep(1000);
            }
            else if (switch_state & 0x1) /* PAUSED STATE */
            {
                display_draw_filled_rect(12, 7, 12 + (8 * 13), 24, 0);
                display_print_text(" Game Paused  ", 4, 7);
                display_print_text(" Btn1 to quit ", 4, 16);
                display_update();
                quicksleep(1000);
                if (button_state & 0x1)
                {
                    game_on = false;
                    current_state = MENU;
                    selected_state = MENU;
                    new_game = true;
                }
            }
            /* ----------------Game loop---------------- */
            else
            {
                // If player1 isn't an ai, take input
                if (!(player1.is_ai))
                {
                    if (button_state & 0x1) // button 1
                    {
                        if (player1.y + P_HEIGHT < 31)
                            pong_move_paddle(&player1, 1.f, 1.f);
                    }
                    if (button_state & 0x2) // button 2
                    {
                        if (player1.y > 1.f)
                            pong_move_paddle(&player1, -1.f, 1.f);
                    }
                }
                // Skynet activate
                else
                {
                    pong_ai_run(&player1, &the_ball, switch_state);
                }
                // If player2 isn't an ai, take input
                if (!(player2.is_ai))
                {
                    if (button_state & 0x4) // button 3
                    {
                        if (player2.y + P_HEIGHT < 31)
                            pong_move_paddle(&player2, 1.f, 1.f);
                    }
                    if (button_state & 0x8) // button 4
                    {
                        if (player2.y > 1.f)
                            pong_move_paddle(&player2, -1.f, 1.f);
                    }
                }
                // Skynet activate
                else
                {
                    pong_ai_run(&player2, &the_ball, switch_state);
                }

                // Update ball position
                pong_move_ball(&player1, &player2, &the_ball, 1.f);

                // Rendering
                display_clear_screen();

                char score_str[SCORE_STR_SIZE+1];
                // This is not the way text should be handled, but it works so it's fine
                display_print_text("P2            P1", 0, 7);
                score_convert_to_string(score_str, player2.score, player1.score);
                display_print_text(score_str, 0, 16);
                score_str[0] = '\0';

                display_draw_empty_rect(SCREEN_OFFSET - 1, 0, 127 - SCREEN_OFFSET, 31, 1);
                display_draw_filled_rect(SCREEN_OFFSET, 1, 127 - SCREEN_OFFSET - 1, 30, 0);
                display_draw_filled_rect((int)player1.x, (int)player1.y, (int)(player1.x + P_WIDTH), (int)(player1.y + P_HEIGHT), 1);
                display_draw_filled_rect((int)player2.x, (int)player2.y, (int)(player2.x + P_WIDTH), (int)(player2.y + P_HEIGHT), 1);
                display_draw_filled_rect((int)the_ball.x, (int)the_ball.y, (int)the_ball.x + B_WIDTH, (int)the_ball.y + B_HEIGHT, 1);
                display_update();
            }
        }
        else if (!game_on && checking_highscores)
        {
            if (button_state & 0x1) // button 1
            {
                checking_highscores = false;
                current_state = MENU;
                selected_state = SCOREBOARD;
                quicksleep(2000);
                display_clear_screen();
            }
            if ((button_state & 0x2) && score_cp > 0) // button 2
            {
                score_cp -= 1;
            }
            if ((button_state & 0x4) && score_cp < SCOREBOARD_ENTRIES - 2) // button 3
            {
                score_cp += 1;
            }
            display_clear_screen();
            display_print_text("Name: Scr:   B1>", 0, 0);
            if (record[score_cp][0] != 0 || record[score_cp][1] != 0 || record[score_cp][2] != 0 || record[score_cp][3] != 0)
                display_print_text(highscore_log[score_cp], 0, 7);
            if (record[score_cp + 1][0] != 0 || record[score_cp + 1][1] != 0 || record[score_cp + 1][2] != 0 || record[score_cp + 1][3] != 0)
                display_print_text(highscore_log[score_cp + 1], 0, 15);
            if (record[score_cp + 2][0] != 0 || record[score_cp + 2][1] != 0 || record[score_cp + 2][2] != 0 || record[score_cp + 2][3] != 0)
                display_print_text(highscore_log[score_cp + 2], 0, 23);
            display_update();
        }
    }
    int i;
    for (i = SCOREBOARD_ENTRIES - 1; i > 0; i--)
    {
        free(highscore_log[i]);
        free(record);
    }
    free(highscore_log);
    free(record);
}

/* ---------------------------------------------- */
/* ------------ Function definitions ------------ */

void initialize_system()
{
    /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel. */
    SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
    SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
    while (OSCCON & (1 << 21))
        ;                 /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while (OSCCON & (1 << 21))
        ;         /* Wait until PBDIV ready */
    SYSKEY = 0x0; /* Lock OSCCON */

    /* Configure switches and buttonpins as inputs */
    TRISDSET = 0x0FE0; // Switches 4:1, Buttons 4:2
    TRISFSET = 0x0001; // Button 1

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    SPI2STATCLR = 0x40;  /* SPI2STAT bit SPIROV = 0; */
    SPI2CONSET = 0x40;   /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x20;   /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x8000; /* SPI2CON bit ON = 1; */
}

void initialize_timer()
{
    T2CONCLR = 0xFFFF; // Clear Timer 2
    TMR2 = 0x0;        // Set timer value 0

    T2CONSET = 0x4 << 4; // Prescaling 1:16
    PR2 = TMR2PERIOD;    // period

    T2CONSET = 0x1 << 15; // start timer

    IPC(2) = 0x4;         // priority 4
    IECSET(0) = 0x1 << 8; // Enable interrupt
    IFSCLR(0) = 0x1 << 8; // Clear interrupt flag
}

void user_isr()
{
    if (IFS(0) & (1 << 8))
    {
        timeout_counter++;
        if (timeout_counter >= 3333)
        {
            timeout_flag = 1;    // sets the flag
            timeout_counter = 0; // reset counter
        }
        // Reset the interrupt flag
        IFSCLR(0) = 1 << 8;
    }
}
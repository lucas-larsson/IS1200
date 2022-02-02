/** 
 * display.h
 * Contains code relevant to rendering an image on the I/O board's OLED
 * display.
 * 
 * Large parts of this code is written 2015 by F. Lundevall
 * Some parts are original code written by Axel Isaksson
 * 
 * @author Axel Isaksson
 * @author F. Lundevall
 * @author Alex Lindberg
*/
#include <stdint.h>
#include <pic32mx.h> /* Declarations of system-specific addresses etc */

/* --------------------------------------------- */
/* ---------------- Definitions ---------------- */
#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* Display controller commands */
#define CMD_CHARGE_PHASE1(x) (uint8_t)(x)
#define CMD_CHARGE_PHASE2(x) (uint8_t)(x << 4)
#define CMD_SET_PAGE_ADDRESS (uint8_t)0x22

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define DISPLAY_ROW_SETS 4
#define DISPLAY_ROW_BITS 8

/* -------------------------------------------- */
/* ---------------- From Lab 1 ---------------- */

/* Declare display-related functions from mipslabfunc.c */
void display_init(void);
void display_print_text(char *s, int x, int y);
void display_image(int x, const uint8_t *data);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
void quicksleep(int cyc);

/* Declare text buffer for display output */
extern char textbuffer[4][16];
extern const uint8_t const font[128 * 8];

/* --------------------------------------------- */
/* ----------- Function declarations ----------- */

/**
 * @author      Alex Lindberg
 * @brief       Sets a pixel to either 1 or 0 depending on input 
 * 
 * @param x 	display x position
 * @param y 	display y position
 * @param op 	which operation to perform, either SET or CLR.
 * 				SET sets the value to 1, CLR sets it to 0.
*/
void display_set_pixel(uint8_t x, uint8_t y, uint8_t op);

/**
 * @author  Alex Lindberg
 * @brief Clears the screen data buffer of SET pixels
*/
void display_clear_screen();

/**
 * @author      Alex Lindberg
 * @brief       Draws a filed rectangle to the screen starting at the top 
 *              left corner (x0,y0) and ending at the bottom right (x1,y1).
 * 
 * @param x0 	top left corner x position
 * @param y0 	top left corner y position
 * @param x1 	bottom right corner x position
 * @param y1 	bottom right corner y position
 * @param op 	which operation to perform, either SET or CLR.
 * 				SET sets the value to 1, CLR sets it to 0.
*/
void display_draw_filled_rect(int8_t x0, int8_t y0,
                              int8_t x1, int8_t y1, uint8_t op);

/**
 * @author      Alex Lindberg
 * @brief       Draws a horizontal line to the screen.
 * 
 * @param x0 	start x position
 * @param x1 	end x position
 * @param y     which row to draw in
 * @param op 	which operation to perform, either SET or CLR.
 * 				SET sets the value to 1, CLR sets it to 0.
*/
void display_horizontal_line(int8_t x0, int8_t x1, int8_t y, uint8_t op);

/**
 * @author      Alex Lindberg
 * @brief       Draws a vertical line to the screen.
 * 
 * @param y0 	start y position
 * @param y1 	end y position
 * @param x     which column to draw in
 * @param op 	which operation to perform, either SET or CLR.
 * 				SET sets the value to 1, CLR sets it to 0.
*/
void display_vertical_line(int8_t y0, int8_t y1, int8_t x, uint8_t op);

/**
 * @author      Alex Lindberg
 * @brief       Draws an empty rectangle to the screen starting at the top 
 *              left corner (x0,y0) and ending at the bottom right (x1,y1).
 * 
 * @param x0 	top left corner x position
 * @param y0 	top left corner y position
 * @param x1 	bottom right corner x position
 * @param y1 	bottom right corner y position
 * @param op 	which operation to perform, either SET or CLR.
 * 				SET sets the value to 1, CLR sets it to 0.
*/
void display_draw_empty_rect(int8_t x0, int8_t y0,
                              int8_t x1, int8_t y1, uint8_t op);

//char * itoaconv( int num );
//void concat_strings(char *s1, char *s2);
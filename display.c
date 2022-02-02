/** 
 * display.c
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
#include "display.h"

/* --------------------------------------------- */
/* -------------- Local variables -------------- */

static uint8_t screen_data[128][4];

/* ---------------------------------------------- */
/* ------------ Function definitions ------------ */

void quicksleep(int cyc)
{
	int i;
	for (i = cyc; i > 0; i--)
		;
}

uint8_t spi_send_recv(uint8_t data)
{
	while (!(SPI2STAT & 0x08))
		;
	SPI2BUF = data;
	while (!(SPI2STAT & 1))
		;
	return SPI2BUF;
}

void display_init(void)
{
	/* Apply power to display controller (VDD) */
	DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	/* Turn off display */
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	/* Enable 7.5 V to display */
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	spi_send_recv(0xD9);
	spi_send_recv(CMD_CHARGE_PHASE1(1) | CMD_CHARGE_PHASE2(15));

	/* Apply power display (VBAT) */
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	/* Set COM output and scan direction */
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	/* Set COM pins */
	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	/* Turn on display */
	spi_send_recv(0xAF);

	/* Clear out graphic RAM */
	display_clear_screen();
	display_update();
}

void display_set_pixel(uint8_t x, uint8_t y, uint8_t op)
{
	/* If coordinates are outside of the screen, pass */
	if (x < 0 || x > DISPLAY_WIDTH - 1 || y < 0 || y > DISPLAY_HEIGHT - 1)
		return;

	/* Which row relative to the screen columns  */
	uint8_t current_bit = y % DISPLAY_ROW_BITS;
	/* Which set of rows */
	uint8_t current_row_set = (y - current_bit) / DISPLAY_ROW_BITS;

	/* Set the pixel in screen_data, 1 if op=SET, 0 if op=CLR  */
	switch (op)
	{
	case 1:
		screen_data[x][current_row_set] |= 0x1 << current_bit;
		break;
	default:
		screen_data[x][current_row_set] &= ~(0x1 << current_bit);
		break;
	}
}

void display_clear_screen()
{
	uint8_t i, j;
	for (j = 0; j < DISPLAY_ROW_SETS; j++)
	{
		for (i = 0; i < DISPLAY_WIDTH; i++)
		{
			screen_data[i][j] = 0x00;
		}
	}
}

void display_draw_filled_rect(int8_t x0, int8_t y0,
							  int8_t x1, int8_t y1, uint8_t op)
{
	uint8_t width = x1 - x0;
	uint8_t height = y1 - y0;
	uint8_t i, j;

	/* Check that width and height is non-negative */
	if (width < 0 || height < 0)
		return;

	/* Draw rectangle to screen */
	for (i = x0; (i < (x0 + width)) && (i < DISPLAY_WIDTH); i++)
	{
		for (j = y0; (j < (y0 + height)) && (j < DISPLAY_HEIGHT); j++)
			display_set_pixel(i, j, op);
	}
}

void display_draw_horizontal_line(int8_t x0, int8_t x1, int8_t y, uint8_t op)
{
	uint8_t width = x1 - x0;
	uint8_t i, j;

	/* Check that width and height is non-negative */
	if (width < 0 || y > 31 || y < 0)
		return;

	/* Draw rectangle to screen */
	for (i = x0; (i < (x0 + width)) && (i < DISPLAY_WIDTH); i++)
	{
		display_set_pixel(i, y, op);
	}
}

void display_draw_vertical_line(int8_t y0, int8_t y1, int8_t x, uint8_t op)
{
	uint8_t height = y1 - y0;
	uint8_t i, j;

	/* Check that width and height is non-negative */
	if (height < 0 || x > 127 || x < 0)
		return;

	/* Draw rectangle to screen */
	for (i = y0; (i < (y0 + height)) && (i < DISPLAY_HEIGHT); i++)
	{
		display_set_pixel(x, i, op);
	}
}

void display_draw_empty_rect(int8_t x0, int8_t y0,
							 int8_t x1, int8_t y1, uint8_t op)
{
	display_draw_horizontal_line(x0, x1, y0, op);
	display_draw_horizontal_line(x0, x1, y1, op);
	display_draw_vertical_line(y0, y1, x0, op);
	display_draw_vertical_line(y0, y1, x1, op);
}

void display_update(void)
{
	int i, j, k, c;
	for (j = 0; j < 4; j++)
	{
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		quicksleep(10);

		spi_send_recv(0x22);
		spi_send_recv(j);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;
		quicksleep(10);

		for (i = 0; i < DISPLAY_WIDTH; i++)
		{
			spi_send_recv(screen_data[i][j]);
		}
	}
}

/* ---------------- Code from mipslabfunc.c ---------------- */
void display_print_text(char *str, int x, int y)
{
	/* if the string is empty or we are trying to print outside the screen, return */
	if (!str || x < 0 || x > (127 - 8) || y < 0 || y > (31 - 8))
		return;

	int current_row_set, col, row;
	int n; /* number of characters printed */
	/* Copy string to screen buffer */
	for (n = 0; n < 16; n++)
	{
		/* Copy bitmap data to screen buffer */
		for (col = 0; col < 8; col++)
		{
			/* Get 8 pixel high row set from character bitmap */
			current_row_set = font[(*str) * 8 + col];

			/* Copy current current row set to screen buffer */
			for (row = 0; row < 8; row++)
			{
				/* If pixel in the current row set is 1, set it to 1 in the buffer */
				if ((current_row_set >> row) & 0x1)
					display_set_pixel(x + 8 * n + col, y + row, 1);
			}
		}
		*str++; // next character in the string
	}
}

void display_image(int x, const uint8_t *data)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for (j = 0; j < 32; j++)
			spi_send_recv(~data[i * 32 + j]);
	}
}

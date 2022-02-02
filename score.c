/** 
 * score.c
 * 
 * @author Alex Lindberg
 * @author Lucas Larsson
*/
#include "score.h"
#include <pic32mx.h>

/* --------------------------------------------- */
/* -------------- Local functions -------------- */

/* Swaps the information of two arrays of the same size */
static void score_swap_ids(uint8_t *a, uint8_t *b, int n);
/* Splits an unsigned 8-bit integer into an array containing its digits */
static void score_split_to_array(uint8_t *buf, uint8_t num, uint8_t count);
/* Counts the number of digits in an unsigned 8-bit integer */
static uint8_t score_count_digits(uint8_t i);

/* ---------------------------------------------- */
/* ------------ Function definitions ------------ */

void score_append_new_record(uint8_t record[SCOREBOARD_ENTRIES][SCORE_RECORD_SIZE], uint8_t new_record[SCORE_RECORD_SIZE])
{
	int idx = 0;
	// search for last record
	while (record[idx][0] != 0 || record[idx][1] != 0 || record[idx][2] != 0 || record[idx][3] != 0)
	{
		// If the new record is better than the last, swap them out and store the old record in new_record
		if (record[idx][3] < new_record[3])
			score_swap_ids(record[idx], new_record, 4);
		idx++;
	}
	if (idx < SCOREBOARD_ENTRIES)
	{
		record[idx][0] = new_record[0];
		record[idx][1] = new_record[1];
		record[idx][2] = new_record[2];
		record[idx][3] = new_record[3];
	}
}

void score_convert_to_strings(char strings[SCOREBOARD_ENTRIES][SCORE_STR_SIZE + 1], uint8_t record[SCOREBOARD_ENTRIES][SCORE_RECORD_SIZE])
{
	int i, j, d_count;
	uint8_t score_arr[3];
	for (i = 0; i < SCOREBOARD_ENTRIES; i++)
	{
		if (record[i][0] != 0 || record[i][1] != 0 || record[i][2] != 0 || record[i][3] != 0)
		{
			d_count = score_count_digits(record[i][3]);
			score_split_to_array(score_arr, record[i][3], d_count);
			for (j = 0; j < SCORE_STR_SIZE; j++)
			{
				if (j < 3)
					strings[i][j] = (char)record[i][j]; // name
				else if (j == 4)
					strings[i][j] = '-'; // dash between
				else if (j > 5 && d_count > 0)
				{
					strings[i][j] = (char)((score_arr[j - 6]) + 0x30); // score
					d_count--;
				}
				else
					strings[i][j] = ' '; // fill with spaces
			}
			strings[i][SCORE_STR_SIZE] = '\0';
		}
		else
		{
			strings[i][0] = '\0';
		}
	}
}

void score_convert_to_string(char string[SCORE_STR_SIZE + 1], uint8_t s1, uint8_t s2)
{
	int i, dc1, dc2;

	uint8_t score_arr2[3]; // changed to non static
	dc2 = score_count_digits(s2);
	score_split_to_array(score_arr2, s2, dc2);

	uint8_t score_arr1[3]; // changed to non static
	dc1 = score_count_digits(s1);
	score_split_to_array(score_arr1, s1, dc1);

	for (i = 0; i < SCORE_STR_SIZE; i++)
	{
		if (dc2 > 0) // start at index 0, draw the score
		{
			string[i] = (char)(score_arr2[i] + 0x30); // score 2
			dc2--;
		}
		else if (i > 13 && dc1 > 0) // start at index 13, draw the score
		{
			string[i] = (char)(score_arr1[i-14] + 0x30); // score 1
			dc1--;
		}
		else
			string[i] = ' '; // fill with spaces
	}
	string[SCORE_STR_SIZE] = '\0';
}

static void score_swap_ids(uint8_t *a, uint8_t *b, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		uint8_t tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}

static void score_split_to_array(uint8_t *buf, uint8_t num, uint8_t count)
{
	int i;
	for (i = count - 1; i >= 0; i--)
	{
		buf[i] = num % 10;
		num /= 10;
	}
}

static uint8_t score_count_digits(uint8_t i)
{
	uint8_t ret = 1;
	while (i /= 10)
		ret++;
	return ret;
}
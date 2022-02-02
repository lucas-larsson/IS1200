/** 
 * score.h
 * 
 * Contains code related to the scoreboard and converting numbers to strings
 * 
 * @author Alex Lindberg
*/
#include <stdint.h>

/* --------------------------------------------- */
/* ---------------- Definitions ---------------- */

#define SCOREBOARD_ENTRIES 7
#define SCORE_STR_SIZE 17 // The length of a string
#define SCORE_RECORD_SIZE 4  // The length of each record

/* --------------------------------------------- */
/* ----------- Function declarations ----------- */

/**
 * @brief   Adds a new record to the list of records.
 * 
 * @param record        A matrix of unsigned 8-bit integers. 
 *                      Usually the record of all scores.
 * @param new_record    An array of unsigned 8-bit integers.
*/
void score_append_new_record(uint8_t record[SCOREBOARD_ENTRIES][SCORE_RECORD_SIZE], uint8_t new_record[SCORE_RECORD_SIZE]);

/**
 * @brief   Fills the string array with the information from the record.
 * 
 * @param string    An array of strings to fill
 * @param record    A matrix of unsigned 8-bit integers. 
*/
void score_convert_to_strings(char strings[SCOREBOARD_ENTRIES][SCORE_STR_SIZE + 1], uint8_t record[SCOREBOARD_ENTRIES][SCORE_RECORD_SIZE]);

/**
 * @brief   Replaces the information of a string with the current player scores.
 *          Used during a match to display the score on screen.
 * 
 * @param string    A string to store the score values
 * @param s2        The score for player 2
 * @param s1        The score for player 1
*/
void score_convert_to_string(char string[SCORE_STR_SIZE + 1], uint8_t s2, uint8_t s1);
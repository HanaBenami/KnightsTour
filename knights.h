/****************************									   
* Author: Hana				*									   
* Last Update: 27/01/2020	*									   
* Knights Tours - Header	*									   
****************************/									   
																				
#ifndef __KNIGHTS_H__															   
#define __KNIGHTS_H__

/*----------------------------------------------------------------------------*/

#include <stddef.h> /* size_t */

/*----------------------------------------------------------------------------*/

#define BOARD_LEN (8) /* Using the current implementation, must be maximum 8 */

#define BOARD_SIZE (BOARD_LEN * BOARD_LEN)

/*----------------------------------------------------------------------------*/

/* Gets a starting position of a knights on a chess board and tries to find a 
   path it can move while visit each position on the board exactly one time. 
 * Each position is represents by two numbers [line, column].
 * Return value:
	- If succeed, returns 0 and updates result.
	- If fails, returns 1. In this case, result might include garbage. 	
 * Algorithmic complexity: O(?).	*/
int KnightsTours(size_t start_position[2], size_t result[BOARD_SIZE][2]);
	   
/*----------------------------------------------------------------------------*/

#endif /* __KNIGHTS_H__ */

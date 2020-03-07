/****************************									   
* Author: Hana				*									   
* Last Update: 28/01/2020	*									   
* Knights Tours - Source	*									   
****************************/									   

#include <stdlib.h>		/* EXIT_FAILURE / EXIT_SUCCESS */
#include <assert.h>		/* assert() */

#include "bit_arr.h"	/* bit_arr_t */

#include "knights.h"	/* current project */

/*----------------------------------------------------------------------------*/

#define POSSIBLE_MOVES (8)
#define INVALID_POSITION BOARD_SIZE
#define POSITION_ROW(position) ((position) / BOARD_LEN)
#define POSITION_COL(position) ((position) % BOARD_LEN)

/*----------------------------------------------------------------------------*/

/* Initializes two look-up-tables. 
	- possible_moves: The posiible next position from each position.
	- moves_to_position: Amount of possible moves from/to each position. */
static void InitPossibleMovesLutsIMP( 
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE]);

/* Sorts the line of current_position in the possible_moves LUT,
   to move "better" steps to earlier position. */
static void PrioritizePossibleMovesLutsIMP(size_t current_position,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE]);

/* Solves the knight's tour problem recursively. */
static int KnightsToursIMP(size_t moves_counter, size_t current_position,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t board, size_t path[BOARD_SIZE]);

#ifndef NDEBUG
/* Checks if the board is full and all the positions has been visited. */
static int IsBoardFullIMP(bit_arr_t board);
#endif 

/* Checks if the position is free, e.g. hasn't been visited yet. */
static int PositionIsFreeIMP(size_t position, bit_arr_t board);

/* Add a move to the given positions to all the relevant DS:
	- Marks the position on the board.
	- Add it to the path of the tour.
	- Reduces relevant values in possible_moves (after using a 
	  position, there are less posibble moves to/from closer positions). */
static void AddMoveIMP(size_t position, size_t moves_counter,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t *board, size_t path[BOARD_SIZE]);

/* Cancels all the operations of AddMoveIMP(). */
static void RemoveMoveIMP(size_t position, size_t moves_counter,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t *board, size_t path[BOARD_SIZE]);

/* Translates the path (one dimensional array) 
   to positions [row,column] (two dimensionals array). */
static void PathToResultArrayIMP(size_t path[BOARD_SIZE], 
		size_t result[BOARD_SIZE][2]);

/* Translates a position [row,column] to index (0 to row*column-1). */
static size_t PositionToIndexIMP(int row, int col);

/* Swaps to numbers, pointed by the pointers.
   This function doesn't work if *num1==*num2. */
static void SwapIMP(size_t *num1, size_t *num2);

/*----------------------------------------------------------------------------*/

int KnightsTours(size_t start_position[2], size_t result[BOARD_SIZE][2])
{
	size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES] = { 0 };
	size_t moves_to_position[BOARD_SIZE] = { 0 };
	size_t path[BOARD_SIZE] = { 0 };
	size_t start_index = 0;
	bit_arr_t board = 0;
	int status = EXIT_SUCCESS;
	
	assert(NULL != start_position);
	assert(start_position[0] < BOARD_LEN);
	assert(start_position[1] < BOARD_LEN);
	assert(NULL != result);

	InitPossibleMovesLutsIMP(possible_moves, moves_to_position);

	start_index = PositionToIndexIMP(start_position[0], start_position[1]);
	status = KnightsToursIMP(0, start_index, possible_moves, 
			moves_to_position, board, path);
	if (EXIT_SUCCESS == status)
	{
		PathToResultArrayIMP(path, result);
	}
	
	return status;
}

/*----------------------------------------------------------------------------*/

static void InitPossibleMovesLutsIMP( 
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE])
{
	size_t index = 0;
	int possible_delta[POSSIBLE_MOVES][2] = { {2, 1}, {1, 2}, {-1, 2}, 
			{-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };

	assert(NULL != possible_moves);
	assert(NULL != moves_to_position);

	for (; index < BOARD_SIZE; ++index)
	{
		int cur_row = POSITION_ROW(index);
		int cur_col = POSITION_COL(index);
		size_t moves = 0;
		size_t insert_move_index = 0;
		
		/* Inserts only valid positions to the LUT, to make sure it will
		 * be before invalid positions. */
		for (; moves < POSSIBLE_MOVES; ++moves)
		{
			int new_row = cur_row + possible_delta[moves][0];
			int new_col = cur_col + possible_delta[moves][1];
			size_t position = PositionToIndexIMP(new_row, new_col);
			if (INVALID_POSITION != position)
			{
				possible_moves[index][insert_move_index] = position;
				++insert_move_index;
				++moves_to_position[position];
			}
		}
			
		/* Fills the rest of the line with invalid positions. */
		for (; insert_move_index < POSSIBLE_MOVES; ++insert_move_index)
		{
			possible_moves[index][insert_move_index] = INVALID_POSITION;	
		}
	}
}

/*----------------------------------------------------------------------------*/

static int KnightsToursIMP(size_t moves_counter, size_t current_position,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t board, size_t path[BOARD_SIZE])
{
	size_t index = 0;
	
	assert(moves_counter <= BOARD_SIZE);
	assert(current_position <= BOARD_SIZE);
	assert(moves_counter < BOARD_SIZE);
	assert(NULL != possible_moves);
	assert(NULL != moves_to_position);
	assert(NULL != path);
	
	AddMoveIMP(current_position, moves_counter, possible_moves,
			moves_to_position, &board, path); 

	if (moves_counter == BOARD_SIZE - 1)
	{
		assert(IsBoardFullIMP(board));
		return EXIT_SUCCESS;	
	}

	PrioritizePossibleMovesLutsIMP(current_position, 
			possible_moves, moves_to_position); 
			
	for (index = 0; 
		(index < POSSIBLE_MOVES) && 
		(INVALID_POSITION != possible_moves[current_position][index]); 
		++index) /* Invalid positions are always at the end of the array */
	{
		size_t next_position = possible_moves[current_position][index];
		if (PositionIsFreeIMP(next_position, board))
		{
			if (EXIT_SUCCESS == KnightsToursIMP(moves_counter + 1, 
				next_position, possible_moves, moves_to_position, board, path))
			{
				return EXIT_SUCCESS;
			}
		}
	}

	RemoveMoveIMP(current_position, moves_counter, possible_moves,
			moves_to_position, &board, path);
	
	return EXIT_FAILURE;
}

/*----------------------------------------------------------------------------*/

#ifndef NDEBUG
static int IsBoardFullIMP(bit_arr_t board)
{
	return (BOARD_SIZE == CountOnLUT(board));	
}
#endif

/*----------------------------------------------------------------------------*/

static int PositionIsFreeIMP(size_t position, bit_arr_t board)
{
	assert(position < BOARD_SIZE);

	return IsOff(board, position);
}

/*----------------------------------------------------------------------------*/
		
static void AddMoveIMP(size_t position, size_t moves_counter,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t *board, size_t path[BOARD_SIZE])
{
	size_t index = 0;

	assert(position < BOARD_SIZE);
	assert(moves_counter < BOARD_SIZE);
	assert(NULL != possible_moves);
	assert(NULL != moves_to_position);
	assert(NULL != board);
	assert(NULL != path);
	assert(IsOff(*board, position));
	assert((INVALID_POSITION == path[moves_counter]) 
			|| (0 == path[moves_counter]));

	*board = SetOn(*board, position);
	path[moves_counter] = position;
	
	for (index = 0; 
		(index < POSSIBLE_MOVES) && 
		(INVALID_POSITION != possible_moves[position][index]); 
		++index) /* Invalid positions are always at the end of the array */
	{
		--moves_to_position[possible_moves[position][index]];
	}
}

/*----------------------------------------------------------------------------*/
		
static void RemoveMoveIMP(size_t position, size_t moves_counter,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE],
		bit_arr_t *board, size_t path[BOARD_SIZE])
{
	size_t index = 0;
	
	assert(position < BOARD_SIZE);
	assert(moves_counter < BOARD_SIZE);
	assert(NULL != possible_moves);
	assert(NULL != moves_to_position);
	assert(NULL != board);
	assert(NULL != path);
	assert(IsOn(*board, position));
	assert(position == path[moves_counter]);

	*board = SetOff(*board, position);
	path[moves_counter] = INVALID_POSITION;

	for (index = 0; 
		(index < POSSIBLE_MOVES) && 
		(INVALID_POSITION != possible_moves[position][index]); 
		++index) /* Invalid positions are always at the end of the array */ 
	{
		++moves_to_position[possible_moves[position][index]];
	}
}
/*----------------------------------------------------------------------------*/

static void PathToResultArrayIMP(size_t path[BOARD_SIZE], 
		size_t result[BOARD_SIZE][2])
{
	size_t index = 0;
	
	assert(NULL != path);
	assert(NULL != result);
	
	for ( ; index < BOARD_SIZE; ++index)
	{
		result[index][0] = POSITION_ROW(path[index]);
		result[index][1] = POSITION_COL(path[index]);
	}
}

/*----------------------------------------------------------------------------*/

static size_t PositionToIndexIMP(int row, int col)
{
	if ((0 <= row) && (row < BOARD_LEN) && (0 <= col) && (col < BOARD_LEN))
	{
		return (row * BOARD_LEN + col);
	}
	else
	{
		return INVALID_POSITION;
	}
}

/*----------------------------------------------------------------------------*/

static void PrioritizePossibleMovesLutsIMP(size_t current_position,
		size_t possible_moves[BOARD_SIZE][POSSIBLE_MOVES],
		size_t moves_to_position[BOARD_SIZE])
{
	size_t index = 0;
	size_t swaps = 0;
	
	assert(current_position < BOARD_SIZE);
	assert(NULL != possible_moves);
	assert(NULL != moves_to_position);
	
	do
	{
		swaps = 0;
		for (index = 0; 
			(index < (POSSIBLE_MOVES - 1)) && 
			(INVALID_POSITION != possible_moves[current_position][index + 1]); 
			++index) /* Invalid positions are always at the end of the array */
		{
			size_t *cur_move = &possible_moves[current_position][index];
			size_t *next_move = &possible_moves[current_position][index + 1];		
			if (moves_to_position[*cur_move] > moves_to_position[*next_move])
			{
				SwapIMP(cur_move, next_move);
				++swaps;
			}
		}
	}
	while (0 < swaps);
}

/*----------------------------------------------------------------------------*/

static void SwapIMP(size_t *num1, size_t *num2)
{
	assert(NULL != num1);
	assert(NULL != num2);
	assert(*num1 != *num2);

	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;
}

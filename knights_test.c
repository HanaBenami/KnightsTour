/****************************									   
* Author: Hana				*									   
* Last Update: 28/01/2020	*									   
* Knights Tours - Test		*									   
****************************/									   

#include <stdio.h>			/* printf() */
#include <string.h>			/* strcat() */
#include <unistd.h> 		/* sleep() */
#include <stdlib.h> 		/* system(), EXIT_SUCCESS */
#include <sys/time.h> 		/* struct timeval */
#include <assert.h> 		/* assert() */

#include "test_functions.h"	/* test functions */
#include "knights.h"		/* current project */

/*----------------------------------------------------------------------------*/

#define CLEAR_BOARD_POSITION (BOARD_SIZE)

static int TestKnightsToursIMP(size_t start_positions[2], double *time);
static int PrintTourIMP(size_t result[BOARD_SIZE][2], char *title);
static void PrintBoardIMP(size_t order[BOARD_LEN][BOARD_LEN]);
static void ClearScreenIMP(void);
static void PauseIMP(void);
static void WaitForEnterIMP(void);
static double TimivalDiff(struct timeval *start, struct timeval *end);
static void PrintResultsIMP(int results[BOARD_LEN][BOARD_LEN],                      
		double times[BOARD_LEN][BOARD_LEN]);

/*----------------------------------------------------------------------------*/

int main()
{
	size_t start_positions[BOARD_LEN][BOARD_LEN][2] = { 0 };
	int results[BOARD_LEN][BOARD_LEN] = { 0 };
	double times[BOARD_LEN][BOARD_LEN] = { 0 };
	size_t row = 0;
	size_t col = 0;
	
	for (col = 0; row < BOARD_LEN; ++row)
	{
		for (col = 0; col < BOARD_LEN; ++col)
		{
			start_positions[row][col][0] = row;
			start_positions[row][col][1] = col;  	
			results[row][col] = TestKnightsToursIMP(
					start_positions[row][col], &times[row][col]);
		}
	}

	ClearScreenIMP();
	printf("Knight's Tours Testing (Board size = %u X %u) - "
			"Time to solution [seconds]:\n", BOARD_LEN, BOARD_LEN);
	PrintResultsIMP(results, times);
	
	return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/

static void PrintResultsIMP(int results[BOARD_LEN][BOARD_LEN],
		double times[BOARD_LEN][BOARD_LEN])
{

	size_t row = 0;
	size_t col = 0;
	double total_time = 0;
	char separator[200] = "\n";

	assert(NULL != results);
	assert(NULL != times);

	for (col = 0; col < BOARD_LEN; ++col)
	{
		strcat(separator, "-----------------");
	}
	strcat(separator, "-\n");

	printf("%s", separator);
	printf("|\t|");
	for (col = 0; col < BOARD_LEN; ++col)
	{	
		printf("\t%lu\t|", col + 1);
	}
	
	for (row = 0; row < BOARD_LEN; ++row)
	{
		printf("   %s|%lu\t", separator, row + 1);
		for (col = 0; col < BOARD_LEN; ++col)
		{
			printf("|  ");
			((0 == results[row][col]) ? 
			 		printf("%.5f\t", times[row][col]) : printf("X\t\t"));
			total_time += times[row][col];
		}
		printf("|");
	}	
	printf("%s\nTotal time: %.5f seconds\n\n", separator, total_time);
}

/*----------------------------------------------------------------------------*/

static int TestKnightsToursIMP(size_t start_positions[2], double *time)
{
	size_t result[BOARD_SIZE][2] = { 0 };
	int status = 0;
	char title[300] = { 0 };
	struct timeval stop = { 0 };
	struct timeval start = { 0 };
	
	assert(NULL != start_positions);
	assert(start_positions[0] < BOARD_LEN);
	assert(start_positions[1] < BOARD_LEN);
	assert(NULL != time);
	
	ClearScreenIMP();
	
	sprintf(title, "Knight's Tour Testing (Board size = %u X %u): \n\n"
			"Starting position: [%lu,%lu] \n\n",
			BOARD_LEN, BOARD_LEN, start_positions[0], start_positions[1]);
	printf("%s", title);
	
	gettimeofday(&start, NULL);
	status = KnightsTours(start_positions, result);
	gettimeofday(&stop, NULL);
	
	if (0 == status)
	{
		char time_str[50] = { 0 };
		*time = TimivalDiff(&start, &stop);
		sprintf(time_str, "Solution time: %.5f seconds\n\n", *time);
		strcat(title, time_str);
		status = PrintTourIMP(result, title);
	}
	else
	{
		printf("No path has been found... \n\n");
	}
	
	WaitForEnterIMP();
	
	return status;
}

/*----------------------------------------------------------------------------*/

static int PrintTourIMP(size_t result[BOARD_SIZE][2], char *title)
{
	size_t index = 0;
	char tour[1000] = { 0 };
	size_t order[BOARD_LEN][BOARD_LEN] = { 0 };
	
	assert(NULL != result);
	assert(NULL != title);

	for (index = 0; index < BOARD_SIZE; ++index)
	{
		char str[20] = { 0 };
		sprintf(str, "[%lu,%lu] ", result[index][0], result[index][1]);
		strcat(tour, str);
		order[index / BOARD_LEN][index % BOARD_LEN] = CLEAR_BOARD_POSITION;
	}
	
	PauseIMP();
	for (index = 0; index < BOARD_SIZE; ++index)
	{
		ClearScreenIMP();
		printf("%s", title);
		printf("%s\n", tour);
		if (CLEAR_BOARD_POSITION != order[result[index][0]][result[index][1]])
		{
			printf("There is an error in the path :-( \n");
			return EXIT_FAILURE;
		}
		order[result[index][0]][result[index][1]] = index;	
		PrintBoardIMP(order);
		PauseIMP();
	}

	return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/

static void PrintBoardIMP(size_t order[BOARD_LEN][BOARD_LEN])
{
	size_t index = 0;
	size_t row = 0;
	char separator[100] = "\n";
	
	assert(NULL != order);
	
	for ( ; index < BOARD_LEN; ++index)
	{
		strcat(separator, "--------");
	}
	strcat(separator, "-\n");
	
	printf("%s", separator);
	for (row = 0; row < BOARD_LEN; ++row)
	{
		size_t col = 0;
		printf("|");
		for (col = 0; col < BOARD_LEN; ++col)
		{
			if (CLEAR_BOARD_POSITION != order[row][col])
			{
				printf("%4lu", order[row][col]);
			}
			printf(" \t|");
		}
		printf("%s", separator);
	}
	printf("\n");
}	

/*----------------------------------------------------------------------------*/

static void ClearScreenIMP(void)
{
	int status = system("clear");
	(void)status;
}

/*----------------------------------------------------------------------------*/

static void PauseIMP(void)
{
#ifndef NDEBUG	
	int status = system("sleep 0.01");
	(void)status;
#endif
}

/*----------------------------------------------------------------------------*/

static void WaitForEnterIMP(void)
{
#ifndef NDEBUG
	printf("Press ENTER to continue. \n");
	getchar();
#endif
}
																			  
/*----------------------------------------------------------------------------*/
																				
static double TimivalDiff(struct timeval *start, struct timeval *end)		   
{																			   
	double time = 0;															
																				
	assert(NULL != start);													  
	assert(NULL != end);														
																				
	time = end->tv_sec - start->tv_sec;										 
	time += (end->tv_usec - start->tv_usec) / 1000000.0;						
																				
	return time;																
}

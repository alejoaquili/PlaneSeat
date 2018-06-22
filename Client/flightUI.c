#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "planeSeatDBHandler.h"
#include "utils.h"

int main()
{
	flightSeat_t seats[TOTAL_SEATS];
	int i, j, k=0;

	for (j = 0; j < COL_NUMBER; j++)
	{	
		for (i = 0; i < ROW_NUMBER; i++)
		{
			seats[k].colLetter = (j + 97);
			seats[k].rowNumber = i;
			if (k%2 == 0)
				seats[k].occupied = 1;
			else
				seats[k].occupied = 0;
			k++;
		}
	}

	printPlane(seats);
	return 0;
}

void printPlane(flightSeat_t * seats)
{
	int plane[ROW_NUMBER][COL_NUMBER];
	int  m, i, j, k;
	char columns[COL_NUMBER] = {'A','B','C','D','E','F','G','H'};

	for (m = 0; m < TOTAL_SEATS; m++)
	{
		plane[seats[m].rowNumber][(seats[m].colLetter) - 97] = seats[m].occupied;
	}

	putchar('\n');
	printf("      A ");

	for (i = 1; i < COL_NUMBER; i++)
		printf ("%3c ", columns[i]);

	putchar('\n');
	printf("    +---");

	for (i = 0; i < (COL_NUMBER - 1); i++)
		printf("+---");

	printf("+\n");

	for (i = 0; i < ROW_NUMBER; i++)
	{
		printf("%3d ", i+1);

		for (j = 0; j < COL_NUMBER; j++)
		{
			printf("|");

			if (plane[i][j] == 0)
				printf("   ");
		
			else 
				printf(" * ");
		}

		printf("|\n");
		printf("    +---");
			
		for (k = 0; k < (COL_NUMBER - 1); k++) 
			printf("+---");

		printf("+\n");
	}
	putchar('\n');
}
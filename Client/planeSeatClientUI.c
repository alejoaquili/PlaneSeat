#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "planeSeatDBHandler.h"
#include "utils.h"
#include "commands.h"

#define COMMAND_QTY 6

static command_t commands[COMMAND_QTY] = {
    {"help", " ", help}, 
    {"add flight", "Add a new flight to the system.", addFlight},
    {"delete flight", "Delete a flight from the system.", deleteFlight},
    {"check flight status", "Check reserved seats for a certain flight.", check},
    {"make reservation", "Make a seat reservation for a flight.", reserve},
    {"cancel reservation", "Cancel a seat reservation for a flight.", cancel},
};

static int run(const char * name, command_t * commands, int clientSocketFd);

static void validate(int input);

static int extractCommand(char * command, const char * buffer);



int planeSeatClientUI(int clientSocketFd)
{ 
    printf("\n\nWelcome to the Flight Reservation Service.\n\tPlease run help to see the available commands.\n");
    char buffer[BUFFER_LENGTH], command[BUFFER_LENGTH];
    int  resp = 0;
    while(1)
    {
        printf("\n$ >> ");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
            extractCommand(command, buffer);
            resp = run(command, commands, clientSocketFd); 
            validate(resp);
        }
        CLEAN_BUFFER
    }
}

static int run(const char * name, command_t * commands, int clientSocketFd)
{
    int ret = INVALID_COMMAND;
    for (int i = 0; i < COMMAND_QTY; i++) 
    {
        if (strcmp(name, commands[i].name) == 0) 
        {
            ret = VALID_COMMAND;
            commands[i].function(clientSocketFd);
        }
    }
    return ret;
}

static void validate(int input) 
{
    if(input == INVALID_COMMAND)
        printf("\n\nInvalid command.\n");
}

static int extractCommand(char * command, const char * buffer) 
{
    int i = 0;
    for (i = 0; buffer[i] != '\0' && buffer[i] != ' '; i++)
        command[i] = buffer[i];
    command[i] = '\0';
    return i;
}

  
void printTest(void)
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


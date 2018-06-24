#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "planeSeatClientUI.h"
#include "planeSeatDBHandler.h"
#include "utils.h"
#include "commands.h"

#define BUFFER_LENGTH 256
#define COMMAND_QTY 7
#define VALID_COMMAND 0
#define INVALID_COMMAND -1

typedef struct command_t{
    char * name;
    char* description;
    int (*function)(int clientSocketFd);
} command_t;

static int run(const char * name, command_t * commands, int clientSocketFd);

static void validate(int input);

static int extractCommand(char * command, const char * buffer);

static int help(int clientSocketFd);


static command_t commands[COMMAND_QTY] = {
    {"help", " ", help}, 
    {"get flights", "Shows the list of flight on the system.", applyToGetFlights},
    {"add flight", "Add a new flight to the system.", applyToAddFlight},
    {"delete flight", "Delete a flight from the system.", applyToDeleteFlight},
    {"make reservation", "Make a seat reservation for a flight.", applyToReserve},
    {"cancel reservation", "Cancel a seat reservation for a flight.", applyToCancel},
    {"print flight", "Shows the seats of a flight.", applyToPrintFlightDistribution}
};


int planeSeatClientUI(int clientSocketFd)
{ 
    printf("\n\nWelcome to the Flight Reservation Service.\n\tPlease run help to see the available commands.\n");
    char buffer[BUFFER_LENGTH], command[BUFFER_LENGTH];
    int  resp = 0;
	do
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
    while(buffer[0] != '\0' && !IS_QUIT(buffer));
	return 0;
}

static int run(const char * name, command_t * commands, int clientSocketFd)
{
    for (int i = 0; i < COMMAND_QTY; i++) 
    {
        if (strcmp(name, commands[i].name) == 0) 
        {
            commands[i].function(clientSocketFd);
			return VALID_COMMAND;
        }
    }
    return INVALID_COMMAND;
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

static int help(int clientSocketFd)
{
    printf("\n\nWelcome to the Flight Reservation service\n");
    printf("These are the available commands:\n");
    for (int i=1; i < COMMAND_QTY; i++)
        printf("\n%s:\t%s\n", commands[i].name, commands[i].description);
    
    return 0;
}


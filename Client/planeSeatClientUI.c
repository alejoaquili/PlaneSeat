#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "planeSeatClientUI.h"
#include "utils.h"
#include "commands.h"

static int run(const char * name, command_t * commands, int clientSocketFd);

static void validate(int input);

static int extractCommand(char * command, const char * buffer);

static int help(int clientSocketFd);


static command_t commands[COMMAND_QTY] = {
    {"help", " ", help}, 
    {"getFlights", "Shows the list of flight on the system.", applyToGetFlights},
    {"addFlight", "Add a new flight to the system.", applyToAddFlight},
    {"deleteFlight", "Delete a flight from the system.", applyToDeleteFlight},
    {"makeReservation", "Make a seat reservation for a flight.", applyToReserve},
    {"cancelReservation", "Cancel a seat reservation for a flight.", applyToCancel},
    {"printFlight", "Shows the seats of a flight.", applyToPrintFlightDistribution}
};


int planeSeatClientUI(int clientSocketFd)
{ 
    printf("\n\nWelcome to the Flight Reservation Service.\n\tPlease run help to see the available commands.\n");
    char buffer[BUFFER_LENGTH], command[BUFFER_LENGTH];
    int  resp = 0;
	do
    {
        printf("\n$ >> ");
        scanf("%256s",buffer);

        if (buffer[0] != '\0') 
        { 
            extractCommand(command, buffer);
            resp = run(command, commands, clientSocketFd); 
            validate(resp);
        }
        CLEAN_BUFFER
    }
    while(1);
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
    for (i = 0; buffer[i] != '\0' && buffer[i] != ' ' ; i++)
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


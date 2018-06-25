#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "commands.h"
#include "serialization.h"
#include "utils.h"
#include "planeSeatSerialized.h"
#include "planeSeatClientUI.h"


typedef enum {SEND_FLIGHTS = 0, ADD_FLIGHT, DELETE_FLIGHT, ADD_RESERVATION, DELETE_RESERVATION, SEND_FSD} operation_t;

static void printPlane(flightSeat_t * seats);


int applyToGetFlights(int clientSocketFd)
{
	int qty;
	flight_t * flights;
	char operation[2];
	sprintf(operation, "%d", SEND_FLIGHTS);

	write(clientSocketFd, operation, 2);

    char * string = readStringToDeserialize(clientSocketFd);

    flights = deserializeToFlights(string, &qty);
	printf("This is the list of flights: \n");
	for(int i = 0; i < qty; i++)
		printf("\tFlight Number: %s, Origin: %s, Destination: %s\n", flights[i].flightNumber, flights[i].origin, flights[i].destination);

    freeSpace(2, string, flights);
    return 0;
}

int applyToAddFlight(int clientSocketFd)
{	
	char * request[3] = {"flight number", "origin", "destination"};
	char buffer[BUFFER_LENGTH], operation[2];
	char * flightSerialized[3];
	sprintf(operation, "%d", ADD_FLIGHT);

	for(int i = 0; i < 3; i++)
	{
		printf("Please enter a %s for the new flight. Type quit to cancel\n", request[i]);
		scanf("%256s", buffer);
		if(buffer[0] == '\0' || IS_QUIT(buffer)) 
			return 0;
		flightSerialized[i] = serializeString(buffer);
	}

	write(clientSocketFd, operation, 2);
	for(int i = 0; i < 3; i++)
	{
		write(clientSocketFd, flightSerialized[i], strlen(flightSerialized[i]));
		free(flightSerialized[i]);
	}
    return 0;
}

int applyToDeleteFlight(int clientSocketFd)
{	
	char buffer[BUFFER_LENGTH], operation[2];
	char * flightSerialized;
	sprintf(operation, "%d", DELETE_FLIGHT);

	printf("Please enter a flight number to delete. Type quit to cancel\n");
	scanf("%256s", buffer);

	if(buffer[0] == '\0' || IS_QUIT(buffer)) 
		return 0;

	flightSerialized = serializeString(buffer);

	write(clientSocketFd, operation, 2);
	write(clientSocketFd, flightSerialized, strlen(flightSerialized));
	free(flightSerialized);
    return 0;
}

int applyToReserve(int clientSocketFd)
{
	char * request[4] = {"flight number", "user id", "column letter", "row number"};
	char buffer[BUFFER_LENGTH], operation[2];
	char * reservationSerialized[4];
	sprintf(operation, "%d", ADD_RESERVATION);
	
	for(int i = 0; i < 4; i++)
	{
		printf("Please enter a %s for the new reservation. Type quit to cancel\n", request[i]);
		scanf("%256s", buffer);
		if(buffer[0] == '\0' || IS_QUIT(buffer)) 
			return 0;
		reservationSerialized[i] = serializeString(buffer);
	}

	write(clientSocketFd, operation, 2);
	for(int i = 0; i < 4; i++)
	{
		write(clientSocketFd, reservationSerialized[i], strlen(reservationSerialized[i]));
		free(reservationSerialized[i]);
	}
    return 0;
}

int applyToCancel(int clientSocketFd)
{
	char * request[4] = {"flight number", "user id", "column letter", "row number"};
	char buffer[BUFFER_LENGTH], operation[2];
	char * reservationSerialized[4];
	sprintf(operation, "%d", DELETE_RESERVATION);
	
	for(int i = 0; i < 4; i++)
	{
		printf("Please enter a %s for the calcelation. Type quit to cancel\n", request[i]);
		scanf("%256s", buffer);
		if(buffer[0] == '\0' || IS_QUIT(buffer)) 
			return 0;
		reservationSerialized[i] = serializeString(buffer);
	}

	write(clientSocketFd, operation, 2);
	for(int i = 0; i < 4; i++)
	{
		write(clientSocketFd, reservationSerialized[i], strlen(reservationSerialized[i]));
		free(reservationSerialized[i]);
	}
    return 0;
}

int applyToPrintFlightDistribution(int clientSocketFd)
{
	int qty;
	flightSeat_t * seats;
	char buffer[BUFFER_LENGTH], operation[2];
	char * flightSerialized;
	sprintf(operation, "%d", SEND_FSD);
        
	printf("Please enter a flight number to print. Type quit to cancel\n");
	scanf("%256s", buffer);

	if(buffer[0] == '\0' || IS_QUIT(buffer)) 
		return 0;

	flightSerialized = serializeString(buffer);
	write(clientSocketFd, operation, 2);
	write(clientSocketFd, flightSerialized, strlen(flightSerialized));
	free(flightSerialized);

    char * string = readStringToDeserialize(clientSocketFd);
    seats = deserializeToFlightSeats(string, &qty);

	if(qty > 0)
		printPlane(seats);
	else
		printf("Inexistent flight\n");

    freeFlightSeatsDistribution(seats, qty);
    free(string);
	return 0;
}


static void printPlane(flightSeat_t * seats)
{
	int plane[ROW_NUMBER][COL_NUMBER];
	int  m, i, j, k;
	char columns[COL_NUMBER] = {'A','B','C','D','E','F','G','H'};

	for (m = 0; m < TOTAL_SEATS; m++)
	{
		plane[seats[m].rowNumber - 1][(seats[m].colLetter) - 97] = seats[m].occupied;
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


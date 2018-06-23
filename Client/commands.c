#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "flightUI.c" //crear este archivo

#define QUIT -1

static int help(int argc, char * argv[])
{
    if(!checkArguments(0, argc))
        return 1;
    printf("\n\nWelcome to the Flight Reservation service\n");
    printf("These are the available commands:\n");
    for (int i=1; i < COMMAND_QTY; i++)
        printf("\n%s:\t%s\n", commands[i].name, commands[i].description);
    
    return 0;
}

static int addFlight(char * args)
{	
	char buffer[BUFFER_LENGTH], command[BUFFER_LENGTH];
    int added = 0;

    do
    {
        printf("Please enter a flight number for the new flight. Type quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
        	if (checkValidNumber(buffer))
        	{
        		if (checkNotRepeated(buffer))
        		{
        			addFlightToDB(buffer);
        			added = 1;
        		}

        		else
        			printf("Flight number already exists\n");
        	}
        	else
        		printf("Invalid flight number. Please only enter positive integers\n");
            
        }
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer) && !added);
    return 0;
}

static int deleteFlight(char * args)
{	
	char buffer[BUFFER_LENGTH], command[BUFFER_LENGTH];
    int deleted = 0;

    do
    {
        printf("Please enter a flight number to delete. Type quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
        	if (findFlight(buffer) != NULL)
        	{
        		deleteFlightFromDB(buffer);
        		deleted = 1;
        	}
        	else
        		printf("Invalid flight number.\n");
            
        }
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer) && !deleted);
    return 0;
}

static int check(char * flight)
{
	flightSeat_t * seats
	
	if (seats = findFlight(flight) != NULL)
	{
		printPlane(seats);
		return 0;
	}
	printf("Wrong flight number.\n");
	return 1;
}

static int reserve(char * args)
{
	char buffer[BUFFER_LENGTH], letter;
	char * flightNumber;
    int reserved = 0, number;

    do
    {
        printf("Please enter a client ID. Insert only numbers. Type quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
        	if (checkValidNumber(buffer))
        	{
        		flightNumber = askForFlightNumber(buffer);
        		if (flightNumber == NULL)
        			return 0;

        		printPlane(flightNumber);

        		if (askForSeat(&letter, &number) == QUIT)	
        			return 0;
        		
        		if (seatFree(fligthNumber, letter, number))
        		{
	        		reserveSeat(id, flightNumber, letter, number);
	        		printPlane(fligthNumber);
	        		reserved = 1;
	        	}
	        	else 
	        		printf("Seat taken\n");
        	}
        	else
        		printf("Invalid ID. Please only enter positive integers\n");
            
        }
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer) && !reserved);
    return 0;
}

static int cancel(char * args)
{
	char buffer[BUFFER_LENGTH], letter;
	char * flightNumber;
    int canceled = 0, number;

    do
    {
        printf("Please enter a client ID. Type quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
        	flightNumber = askForFlightNumber(buffer);
        	if (flightNumber == NULL)
        		return 0;

        	printPlane(fligthNumber);

        	if (ạskForSeat(&letter, &number) == QUIT)
        		return 0;

        	if (existsReservation(buffer, fligthNumber, letter, number))
        	{
        		cancelSeat(id, flightNumber)
        		printPlane(buffer);
        		canceled = 1;
        	}
        	else
        		printf("There is no such reservation\n");
            
        }
        
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer) && !canceled);
    return 0;
}

static char * askForFlightNumber(char * id)
{
	char buffer[BUFFER_LENGTH];
    int reserved = 0;

    do
    {
        printf("Please enter an existing flight number. Type quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);

        if (buffer[0] != '\0' && !IS_QUIT(buffer)) 
        { 
        	if (findFlight(buffer) != NULL)
        	{
        		return buffer;
        	}
        	else
        		printf("Invalid flight number.\n");
            
        }
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer));
    
    if (IS_QUIT(buffer))
    	return 	NULL;
}

static int askForSeat(char * letter, int * number)
{
	char buffer[BUFFER_LENGTH];
	int valid = 0, cant;
	
	do
    {
        printf("Please enter an available flight seat.\nFormat: [A-H][01-30]\nType quit to cancel\n");
        scanf(buffer, BUFFER_LENGTH);
        cant = sscanf(buffer, "%c %d", letter, number);

        if (buffer[0] != '\0' && !IS_QUIT(buffer) && cant == 2) 
        { 
        	if (checkSeat(*letter, *number))
        	{
        		valid = 1;
        	}
        	else
        		printf("Invalid seat number.\n");
            
        }
        CLEAN_BUFFER
    }
    while (!IS_QUIT(buffer) && !valid);
    
    if (IS_QUIT(buffer))
    	return 	QUIT;
    return 0;
}

static int checkSeat(char letter, int number)
{
	if (toUpper(letter) < 'A' || toUpper(letter) > 'H')
		return 0;

	if (number < 1 || number > 30)
		return 0;

	return 1;
}

static int checkValidNumber(char * number)
{
	int num = atoi(number);
    if (!isdigit(num) || num < 0)
		return 0;
	return 1;
}

static int existsReservation(char * id, int fligthNumber, char letter, int number)
{
	//se fija si existe la reservation. devuelve 1 si existe y 0 si no
}

static int seatFree(int flightNumber, char letter, int number)
{
	//devuelve 1 si el asiento esta free, 0 si no
}

static void reserveSeat(id, flightNumber, buffer)
{
	//reserva el asiento
}


static int checkNotRepeated(char * number)
{
	//checkea si ya esta en la db
}

static void addFlightToDB(char * number)
{
	//lo agrega
}

static void deleteFlightFromDB(char * number)
{
	//lo elimina
}

static flightSeat_t * findFlight(char * flight)
{
	//se fija si existe el flight. si existe devuelve el vector de seats, sino null
	return NULL;
}
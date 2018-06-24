#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "errorslib.h"
#include "socketlib.h"
#include "connectionHandler.h"
#include "planeSeatDBHandler.h"
#include "planeSeatSerialized.h"
#include "seralization.h"

#define OPERATIONS_QTY 6


static void sendFlights(int socketFd);

static void addFlight(int socketFd);

static void deleteFlight(int socketFd);

static void addReservation(int socketFd);

static void deleteReservation(int socketFd);

static void sendFlightDistribution(int socketFd);


typedef void (* planeSeatOperations_t)(int);

static planeSeatOperations_t planeSeatOperations[OPERATIONS_QTY] = {sendFlights, addFlight, deleteFlight, addReservation, deleteReservation, sendFlightDistribution};

static dataBaseADT db = NULL;


int main(int argc , char *argv[])
{
    checkAreEquals(argc, 2, "Invalid arguments to Connection Handler");
    if(db == NULL)
        initializeConnectionHandler();     

    int operation, connectFd = atoi(argv[1]);
    char buffer[2];
    
    while((read(socketFd, buffer, 2) > 0) //deberia ser blockeante 
    {
        operation = atoi(buffer);
        planeSeatOperations[operation](socketFd);
    }
    return 0;
}

void initializeConnectionHandler(void)
{
   db = createPlaneSeatDataBaseHandler();
}

static void sendFlights(int socketFd)
{
    int qty;
    flight_t * flights = getFlights(db, &qty);
    char * flightsText = serializeFlights(flights);
    
    write(socketFd, flightsText, strlen(flightsText));
    
    freeFlights(flights, qty);
    free(flightsText);
}

static void addFlight(int socketFd)
{
    char * data[3];
    for(int i = 0; i < 3; i++)
    {
        char * string = readStringToDeserialize(socketFd);
        data[i] = deserializeToString(string);
        free(string);
    }
    addNewFlight(db, data[0], data[1], data[2]);
    freeSpace(3, data[0], data[1], data[2]);
}

static void deleteFlight(int socketFd)
{
    char * string = readStringToDeserialize(socketFd);
    char * flightNumber = deserializeToString(string);

    deleteFligth(db, flightNumber);
    freeSpace(2, string, flightNumber);
}

static void addReservation(int socketFd)
{
    char * dataString[4];

    for(int i = 0; i < 4; i++)
    {
        char * string = readStringToDeserialize(socketFd);
        dataString[i] = deserializeToString(string);
        free(string);
    }

    addNewReservation(db, dataString[0], dataString[1], atoi(dataString[2]), atoi(dataString[3]));
    freeSpace(2, dataString[0], dataString[1], dataString[2], dataString[3]);
}

static void deleteReservation(int socketFd)
{
    char * dataString[4];

    for(int i = 0; i < 4; i++)
    {
        char * string = readStringToDeserialize(socketFd);
        dataString[i] = deserializeToString(string);
        free(string);
    }

    deleteReservation(db, dataString[0], dataString[1], atoi(dataString[2]), atoi(dataString[3]));
    freeSpace(2, dataString[0], dataString[1], dataString[2], dataString[3]);
}

static void getFlightDistribution(int socketFd)
{
    char * string = readStringToDeserialize(socketFd);
    char * flightNumber = deserialize(string, String);
    char * flightNumber = deserializeToString(string);
    int qty;
    flightSeats_t * fsd = getFlightSeatsDistribution(db, flightNumber, &qty);
    
    char * fsdText = serializeFlightSeats(fsd, qty);
    write(socketFd, fsdText, strlen(fsdText));

    freeFlightSeatsDistribution(fsd, qty);
    freeSpace(3, string, flightNumber, fsdText);
}

char * deserializeToString(char * string)
{
    arrayADT array = deserialize(string);
    char * resp = (char *) getValueInArray(array, 0);
    freeArray(array);
    return resp;
}

int deserializeToInt(char * string)
{
    arrayADT array = deserialize(string);
    int resp = *((int *) getValueInArray(array, 0));
    freeArray(array);
    return resp;
}



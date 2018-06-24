#include <stdlib.h>
#include <unistd.h>
#include "errorslib.h"
#include "utils.h"
#include "connectionHandler.h"
#include "planeSeatDBHandler.h"
#include "planeSeatSerialized.h"
#include "serialization.h"

#define OPERATIONS_QTY 6


static void acceptSendFlights(int connectFd);

static void acceptAddFlight(int connectFd);

static void acceptDeleteFlight(int connectFd);

static void acceptAddReservation(int connectFd);

static void acceptDeleteReservation(int connectFd);

static void acceptSendFlightDistribution(int connectFd);


typedef void (* planeSeatOperations_t)(int);

static planeSeatOperations_t planeSeatOperations[OPERATIONS_QTY] = {acceptSendFlights, acceptAddFlight, acceptDeleteFlight, acceptAddReservation, acceptDeleteReservation, acceptSendFlightDistribution};

static dataBaseADT db = NULL;


int main(int argc , char *argv[])
{
    checkAreEquals(argc, 2, "Invalid arguments to Connection Handler");
    if(db == NULL)
        initializeConnectionHandler();     

    int operation, connectFd = atoi(argv[1]);
    char buffer[2];
    
    while((read(connectFd, buffer, 2) > 0)) //deberia ser blockeante 
    {
        operation = atoi(buffer);
        planeSeatOperations[operation](connectFd);
    }
    return 0;
}

void initializeConnectionHandler(void)
{
   db = createPlaneSeatDataBaseHandler();
}

static void acceptSendFlights(int connectFd)
{
    int qty;
    flight_t * flights = getFlights(db, &qty);
    char * flightsText = serializeFlights(flights, qty);
    
    write(connectFd, flightsText, strlen(flightsText));
    
    freeFlights(flights, qty);
    free(flightsText);
}

static void acceptAddFlight(int connectFd)
{
    char * data[3];
    for(int i = 0; i < 3; i++)
    {
        char * string = readStringToDeserialize(connectFd);
        data[i] = deserializeToString(string);
        free(string);
    }
    addNewFlight(db, data[0], data[1], data[2]);
    freeSpace(3, data[0], data[1], data[2]);
}

static void acceptDeleteFlight(int connectFd)
{
    char * string = readStringToDeserialize(connectFd);
    char * flightNumber = deserializeToString(string);

    deleteFlight(db, flightNumber);
    freeSpace(2, string, flightNumber);
}

static void acceptAddReservation(int connectFd)
{
    char * dataString[4];

    for(int i = 0; i < 4; i++)
    {
        char * string = readStringToDeserialize(connectFd);
        dataString[i] = deserializeToString(string);
        free(string);
    }

    addNewReservation(db, dataString[0], dataString[1], atoi(dataString[2]), atoi(dataString[3]));
    freeSpace(2, dataString[0], dataString[1], dataString[2], dataString[3]);
}

static void acceptDeleteReservation(int connectFd)
{
    char * dataString[4];

    for(int i = 0; i < 4; i++)
    {
        char * string = readStringToDeserialize(connectFd);
        dataString[i] = deserializeToString(string);
        free(string);
    }

    deleteReservation(db, dataString[0], dataString[1], atoi(dataString[2]), atoi(dataString[3]));
    freeSpace(2, dataString[0], dataString[1], dataString[2], dataString[3]);
}

static void acceptSendFlightDistribution(int connectFd)
{
    char * string = readStringToDeserialize(connectFd);
    char * flightNumber = deserializeToString(string);
    int qty;
    flightSeat_t * fsd = getFlightSeatsDistribution(db, flightNumber, &qty);
    
    char * fsdText = serializeFlightSeats(fsd, qty);
    write(connectFd, fsdText, strlen(fsdText));

    freeFlightSeatsDistribution(fsd, qty);
    freeSpace(3, string, flightNumber, fsdText);
}



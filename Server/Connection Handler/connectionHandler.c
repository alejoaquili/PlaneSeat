#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "errorslib.h"
#include "planeSeatDBHandler.h"
#include "seralization.h"

#define OPERATIONS_QTY 6
typedef enum {SEND_FLIGHTS = 0, ADD_FLIGHT, DELETE_FLIGHT, ADD_RESERVATION, DELETE_RESERVATION, GET_FSD} operation_t;

typedef struct message_t
{
    operation_t operation;
    size_t size;
} message_t;

typedef void (* planeSeatOperations_t)(int, int);


static planeSeatOperations_t planeSeatOperations[OPERATIONS_QTY] = {sendFlights,  };

static dataBaseADT db;


int main(int argc , char *argv[])
{
    checkAreEquals(argc, 2, "Invalid arguments to Connection Handler");
    int connectFd = atoi(argv[1]);
    message_t msg;
    char buffer[MSG_SIZE]; 
    while(read(socketFd, buffer, sizeofSerialization(message_t)) > 0) //deberia ser blockeante o esperar un mensage quit
    {
        deserializer(buffer, &msg);
        planeSeatOperations[msg->operation](socketFd, msg->size);
    }
    return 0;
}

void initializeConnectionHandler()
{
   db = createPlaneSeatDataBaseHandler();
}

void sendFlights(int socketFd, int size)
{
    int qty;
    flight_t * flights = getFlights(db, &qty);
    char * flightsText = serializate(flights);
    send(socketFd, flightsText, strlen(flightsText));
}

void addFlight(int socketFd, int size)
{


}


void deleteFlight(int socketFd, int size)
{
    char buffer[MSG_SIZE] = 

}


void addReservation(int socketFd, int size)
{


}

void deleteReservation(int socketFd, int size)
{


}


void getFlightDistribution(int socketFd, int size)
{


}



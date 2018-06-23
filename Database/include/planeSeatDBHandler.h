#ifndef PLANE_SEAT_DB_HANDLER_H
#define PLANE_SEAT_DB_HANDLER_H

#include "dataBaseADT.h"

#define DB_NAME "planeSeat"
#define MAX_QUERY_LENGTH 512


#define COL_NUMBER 8
#define ROW_NUMBER 30
#define TOTAL_SEATS COL_NUMBER * ROW_NUMBER
#define COL_MIN_LETTER 'a'

typedef struct flight_t {
    char * flightNumber;
    char * origin;
    char * destination;
} flight_t;

typedef struct flightSeat_t{
    char * flightNumber;
    char colLetter;
    int rowNumber;
    boolean occupied;
} flightSeat_t;

dataBaseADT createPlaneSeatDataBaseHandler(void);

int addNewFlight(dataBaseADT db, char * flightNumber, char * origin, char * destination);

int addNewReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char * userId);

int deleteFligth(dataBaseADT db, char * flightNumber);

int deleteReservation(dataBaseADT db, char * flightNumber, char * userId, char colLetter, int rowNumber);

flight_t * getFlights(dataBaseADT db, int * qty);

void freeFlights(flight_t * flights, int size);

flightSeat_t * getFlightSeatsDistribution(dataBaseADT db, char * flightNumber, int * qty);

void freeFlightSeatsDistribution(flightSeat_t * fsd, int size);

#endif


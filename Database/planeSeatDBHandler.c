#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planeSeatDBHandler.h"

static	char * createFlightTableQuery = "CREATE TABLE flight(" \
        "flightNumber   TEXT," \
        "origin         TEXT," \
        "destination    TEXT," \
        "PRIMARY KEY(flightNumber));";

static	char * createFlightSeatsTableQuery = "CREATE TABLE flightSeats(" \
        "flightNumber   TEXT," \
        "colLetter      CHAR(1)," \
        "rowNumber      INTEGER," \
        "occupied    INTEGER," \
        "PRIMARY KEY(flightNumber, colLetter, rowNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";
	
static	char * createReservationTableQuery = "CREATE TABLE reservation(" \
        "flightNumber   TEXT," \
        "userId         TEXT NOT NULL," \
        "colLetter      CHAR(1) NOT NULL," \
        "rowNumber      INTEGER NOT NULL," \
        "PRIMARY KEY(flightNumber, colLetter, rowNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";

static char * insertFlightFmt = "INSERT INTO flight (flightNumber, origin, destination) VALUES(";

static int insertFlightFmtSize = 0;

static char * insertFlightSeatsFmt = "INSERT INTO flightSeats (flightNumber, colLetter, rowNumber, occupied) VALUES(";

static int insertFlightSeatsFmtSize = 0;

static char * insertReservationFmt = "INSERT INTO reservation (flightNumber, userId, colLetter, rowNumber) VALUES(";

static int insertReservationFmtSize = 0;

static char * deleteFromFlightFmt = "DELETE FROM flight WHERE flightNumber = ";

static int deleteFromFlightFmtSize = 0;

static char * updateFlightSeatsFmt = "UPDATE flightSeats SET occupied = ";

static int updateFlightSeatsFmtSize = 0;

static char * deleteFromReservationFmt = "DELETE FROM reservation WHERE flightNumber = ";

static int deleteFromReservationFmtSize = 0;

static int createTables(dataBaseADT db);
static int addNewFlightSeats(dataBaseADT db, char * flightNumber);
static int checkFlightReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char *  userId);


dataBaseADT createPlaneSeatDataBaseHandler(void)
{
    dataBaseADT db = createDataBase(DB_NAME, true);
    if(createTables(db) < 0)
        return NULL;
    insertFlightFmtSize = strlen(insertFlightFmt);
    insertFlightSeatsFmtSize = strlen(insertFlightSeatsFmt);
    insertReservationFmtSize = strlen(insertReservationFmt);
    updateFlightSeatsFmtSize = strlen(updateFlightSeatsFmt);
    deleteFromFlightFmtSize = strlen(deleteFromFlightFmt);
    deleteFromReservationFmtSize = strlen(deleteFromReservationFmt);
    return db;
}

int addNewFlight(dataBaseADT db, char * flightNumber, char * origin, char * destination)
{
    if(db == NULL || flightNumber == NULL || origin == NULL || destination == NULL)
        return -1;
    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, insertFlightFmt, insertFlightFmtSize);
    sprintf(newQuery + insertFlightFmtSize, "'%s', '%s', '%s');", flightNumber, origin, destination);
    int result = executeQueryDataBase(db, newQuery, false);
    if(result >= 0)
        result = addNewFlightSeats(db, flightNumber);
    return result;
}

int addNewReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char * userId)
{
    if(!checkFlightReservation(db, flightNumber, colLetter, rowNumber, userId))
        return -1;
    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, updateFlightSeatsFmt, updateFlightSeatsFmtSize);
    sprintf(newQuery + updateFlightSeatsFmtSize, "1 WHERE flightNumber = '%s' AND colLetter = '%c' AND rowNumber = %d;", flightNumber, colLetter, rowNumber);
    int result = executeQueryDataBase(db, newQuery, false);
    
    if(result < 0)
        return -1;
    memcpy(newQuery, insertReservationFmt, insertReservationFmtSize);
    sprintf(newQuery + insertReservationFmtSize, "'%s', '%s', '%c', %d);", flightNumber, userId, colLetter, rowNumber);
    return executeQueryDataBase(db, newQuery, false);
}

int deleteFligth(dataBaseADT db, char * flightNumber)
{
    if(db == NULL || flightNumber == NULL)
        return -1;

    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, deleteFromFlightFmt, deleteFromFlightFmtSize);
    sprintf(newQuery + deleteFromFlightFmtSize, "'%s';", flightNumber);
    return executeQueryDataBase(db, newQuery, false);
}

int deleteReservation(dataBaseADT db, char * flightNumber, char * userId, char colLetter, int rowNumber)
{
    if(!checkFlightReservation(db, flightNumber, colLetter,rowNumber, userId))
        return -1;

    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, deleteFromReservationFmt, deleteFromReservationFmtSize);
    sprintf(newQuery + deleteFromReservationFmtSize, "'%s' AND userId = '%s' AND colLetter = '%c' AND rowNumber = %d;", flightNumber, userId, colLetter, rowNumber);
    int result = executeQueryDataBase(db, newQuery, false);
    if(result < 0)
        return -1;

    memcpy(newQuery, updateFlightSeatsFmt, updateFlightSeatsFmtSize);
    sprintf(newQuery + updateFlightSeatsFmtSize, "0 WHERE flightNumber = '%s' AND colLetter = '%c' AND rowNumber = %d;", flightNumber, colLetter, rowNumber);
    return executeQueryDataBase(db, newQuery, false);
}

flight_t * getFlights(dataBaseADT db, int * qty)
{
    if(db == NULL || qty == NULL)
        return NULL;
        
    char * query = "SELECT COUNT(*) FROM flight;";
    prepareStatement(db, query);
    int rowQty = 0;
    stepStatement(db);
    getIntFromColumn(db, 0, &rowQty);
    finalizeStatement(db);

    flight_t * flights = malloc(rowQty * sizeof(flight_t));
    char * newQuery = "SELECT * FROM flight;";
    prepareStatement(db, newQuery);
    int i = 0;
    while(stepStatement(db) == 100)
    {
        flights[i].flightNumber = getTextFromColumn(db, 0);
        flights[i].origin = getTextFromColumn(db, 1);
        flights[i].destination = getTextFromColumn(db, 2);
        i++;
    }
    finalizeStatement(db);
    *qty = rowQty;
    return flights;
}

void freeFlights(flight_t * flights, int size)
{
    if(flights == NULL || size < 0)
        return;
    for(int i = 0 ; i < size ; i++)
    {
        freeDBText(flights[i].flightNumber);
        freeDBText(flights[i].origin);
        freeDBText(flights[i].destination);
    }
    free(flights);
}

flightSeat_t * getFlightSeatsDistribution(dataBaseADT db, char * flightNumber, int * qty)
{
    if(db == NULL || flightNumber == NULL || qty == NULL)
        return NULL;
        
    char query[MAX_QUERY_LENGTH];
    sprintf(query, "SELECT COUNT(*) FROM flightSeats WHERE flightNumber = '%s';", flightNumber); 
    prepareStatement(db, query);
    int rowQty = 0;
    stepStatement(db);
    getIntFromColumn(db, 0, &rowQty);
    finalizeStatement(db);

    flightSeat_t * flightSeatDistribution = malloc(rowQty * sizeof(flightSeat_t));
    char newQuery[MAX_QUERY_LENGTH];
    sprintf(newQuery, "SELECT * FROM flightSeats WHERE flightNumber = '%s';", flightNumber);
    prepareStatement(db, newQuery);
    int i = 0;
    while(stepStatement(db) == 100)
    {
        flightSeatDistribution[i].flightNumber = getTextFromColumn(db, 0);
        flightSeatDistribution[i].colLetter = getCharFromColumn(db, 1);
        getIntFromColumn(db, 2, &(flightSeatDistribution[i].rowNumber));
        getIntFromColumn(db, 3, (int *) &(flightSeatDistribution[i].occupied));
        i++;
    }
    finalizeStatement(db);
    *qty = rowQty;
    return flightSeatDistribution;
}

void freeFlightSeatsDistribution(flightSeat_t * fsd, int size)
{
    if(fsd == NULL || size < 0)
        return;
    for (int i = 0 ; i < size ; i++)
        freeDBText(fsd[i].flightNumber);
    free(fsd);
}

static int createTables(dataBaseADT db)
{
    if(db == NULL)
        return -1;
    int result = 0;
    result += executeQueryDataBase(db, createFlightTableQuery, false);
    result += executeQueryDataBase(db, createFlightSeatsTableQuery, false);
    result += executeQueryDataBase(db, createReservationTableQuery, false);
    return result;
}

static int addNewFlightSeats(dataBaseADT db, char * flightNumber)
{
    char newQuery[MAX_QUERY_LENGTH];
    int result = 0;
    for(int i = 0; i < COL_NUMBER ; i++)
    {
        for(int j = 1 ; j <= ROW_NUMBER; j++)
        {
            memcpy(newQuery, insertFlightSeatsFmt, insertFlightSeatsFmtSize);
            sprintf(newQuery + insertFlightSeatsFmtSize, "'%s', '%c', %d, %d);", flightNumber, 'a' + i, j, false);
            result += executeQueryDataBase(db, newQuery, false);
            if(result < 0)
                return result;
        }
    }
    return result;
}

static int checkFlightReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char * userId)
{
    if(db == NULL || flightNumber == NULL || userId == NULL ||colLetter < COL_MIN_LETTER || colLetter >= (COL_MIN_LETTER + COL_NUMBER) || rowNumber <= 0 || rowNumber > ROW_NUMBER)
        return 0;
    return 1;
}


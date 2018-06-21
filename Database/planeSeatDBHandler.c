#include <stdio.h>
#include <string.h>
#include "dataBaseADT.h"

#define DB_NAME "planeSeat"
#define MAX_QUERY_LENGTH 512


#define COL_NUMBER 8
#define ROW_NUMBER 30
#define TOTAL_SEATS COL_NUMBER * ROW_NUMBER
#define COL_MIN_LETTER 'a'


static	char * createFlightTableQuery = "CREATE TABLE flight(" \
        "flightNumber   TEXT," \
        "origin         TEXT," \
        "destination    TEXT," \
        "PRIMARY KEY(flightNumber));";

static	char * createFlightSeatsTableQuery = "CREATE TABLE flightSeats(" \
        "flightNumber   TEXT," \
        "colLetter      CHAR(1)," \
        "rowNumber      INTEGER," \
        "occupied    BOOLEAN," \
        "PRIMARY KEY(flightNumber, colLetter, rowNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";
	
static	char * createReservationTableQuery = "CREATE TABLE reservation(" \
        "flightNumber   TEXT," \   
        "userId         TEXT," \
        "colLetter      CHAR(1) NOT NULL," \
        "rowNumber      INTEGER NOT NULL," \
        "PRIMARY KEY(flightNumber, userId, colLetter, rowNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";

static char * insertFlightFmt = "INSERT INTO flight (flightNumber, origin, destination) VALUES(";

static int insertFlightFmtSize = 0;

static char * insertFlightSeatsFmt = "INSERT INTO flightSeats (flightNumber, colLetter, rowNumber, occupied) VALUES(";

static int insertFlightSeatsFmtSize = 0;

static char * insertReservationFmt = "INSERT INTO reservation (flightNumber, userId, colLetter, rowNumber) VALUES(";

static int insertReservationFmtSize = 0;

static char * deleteFromFlightFmt = "DELETE FROM flight WHERE flightNumber = ";

static int deleteFromFlightFmtSize = 0;

static char * deleteFromFlightSeatsFmt = "DELETE FROM flightSeats WHERE flightNumber = ";

static int deleteFromFlightSeatsFmtSize = 0;

static char * deleteFromReservationFmt = "DELETE FROM reservation WHERE flightNumber = ";

static int deleteFromReservationFmtSize = 0;

static int createTables(dataBaseADT db);
static int addNewFlightSeats(dataBaseADT db, char * flightNumber);
static int checkFlightReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char *  userId);


dataBaseADT createDataBase(void)
{
    dataBaseADT db = createDataBase(DB_NAME, true);
    if(createTables(db) < 0)
        return -1;
    insertFlightFmtSize = strlen(insertFlightFmt);
    insertFlightSeatsFmtSize = strlen(insertFlightSeatsFmt);
    insertReservationFmtSize = strlen(insertReservationFmt);
    deleteFromFlightFmtSize = strlen(deleteFromFlightFmt);
    deleteFromFlightSeatsFmtSize = strlen(deleteFromFlightSeatsFmt);
    deleteFromReservationFmtSize = strlen(deleteFromReservationFmt);
    return 0;
}

int addNewFlight(dataBaseADT db, char * flightNumber, char * origin, char * destination)
{
    if(flightNumber == NULL || origin == NULL || destination == NULL)
        return -1;
    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, insertFlightFmt, insertFlightFmtSize);
    sprintf(newQuery + insertFlightFmtSize, "%s, %s, %s);", flightNumber, origin, destination);
    int result = executeQueryDataBase(db, newQuery, false);
    if(result < 0)
        return -1;
    addNewFlightSeats(db, flightNumber);
    return result;
}

int addNewReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char * userId);
{
    if(!checkFlightReservation(db, flightNumber, colLetter,rowNumber, userId))
        return -1;

    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, insertFlightSeatsFmt, insertFlightSeatsFmtSize);
    sprintf(newQuery + insertFlightSeatsFmtSize, "%s, %c, %d, %s);", flightNumber, colLetter, rowNumber, "true");
    int result = executeQueryDataBase(db, newQuery, false);
    if(result < 0)
        return -1;
    memset(newQuery, 0, MAX_QUERY_LENGTH);
    memcpy(newQuery, insertReservationFmt, insertReservationFmtSize);
    sprintf(newQuery + insertReservationFmtSize, "%s, %s, %c, %d);", flightNumber, userId, colLetter, rowNumber);
    return executeQueryDataBase(db, newQuery, false);
}

int deleteFligth(dataBaseADT db, char * flightNumber)
{
    if(db == NULL || flightNumber == NULL)
        return -1;
    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, deleteFromFlightFmt, deleteFromFlightFmtSize);
    sprintf(newQuery + deleteFromFlightFmtSize, "%s ;", flightNumber);
    int result = executeQueryDataBase(db, newQuery, false);
    if(result < 0)
        return -1;
    memset(newQuery, 0, MAX_QUERY_LENGTH);
    memcpy(newQuery, deleteFromFlightSeatsFmt, deleteFromFlightSeatsFmtSize);
    sprintf(newQuery + deleteFromFlightSeatsFmtSize, "%s ;", flightNumber);
    return executeQueryDataBase(db, newQuery, false);
}

int deleteReservation(dataBaseADT db, char * flightNumber, char * userId, char colLetter, int rowNumber)
{
    if(!checkFlightReservation(db, flightNumber, colLetter,rowNumber, userId))
        return -1;
    char newQuery[MAX_QUERY_LENGTH];
    memcpy(newQuery, deleteFromReservationFmt, deleteFromReservationFmtSize);
    sprintf(newQuery + deleteFromReservationFmtSize, "%s AND userId = %s AND colLetter = %c AND rowNumber = %d;", flightNumber, userId, colLetter, rowNumber);
    return executeQueryDataBase(db, newQuery, false);
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
    for(int i = (int)'a'; i < COL_NUMBER ; i++)
    {
        for(int j = 1 ; j < ROW_NUMBER; i++)
        {
            memcpy(newQuery, insertFlightSeatsFmt, insertFlightSeatsFmtSize);
            sprintf(newQuery + insertFlightSeatsFmtSize, "%s, %c, %d, %s);", flightNumber, (char)i, j, "false");
            result += executeQueryDataBase(db, newQuery, false);
            if(result < 0)
                return result;
            memset(newQuery, 0, MAX_QUERY_LENGTH);
        }
    }
    return result;
}

static int checkFlightReservation(dataBaseADT db, char * flightNumber, char colLetter, int rowNumber, char * userId)
{
    if(db == NULL || flightNumber == NULL || userId == NULL ||colLetter < COL_MIN_LETTER || colLetter > COL_MIN_LETTER+ COL_NUMBER || rowNumber <= 0 || rowNumber >= ROW_NUMBER)
        return 0;
    return 1
}
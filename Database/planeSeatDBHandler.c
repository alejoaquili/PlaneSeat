#include "dataBaseADT.h"

#define DB_NAME "planeSeat"

static	char * createFlightTableQuery = "CREATE TABLE flight(" \
        "flightNumber   TEXT," \
        "origin         TEXT," \
        "destination    TEXT," \
        "freeSeats      INTEGER," \
        "PRIMARY KEY(flightNumber));";

static	char * createFlightSeatsTableQuery = "CREATE TABLE flightSeats(" \
        "flightNumber   TEXT," \
        "rowLetter      CHAR(1)," \
        "colNumber      INTEGER," \
        "occupied    BOOLEAN," \
        "PRIMARY KEY(flightNumber, rowLetter, colNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";
	
static	char * createReservationTableQuery = "CREATE TABLE reservation(" \
        "flightNumber   TEXT," \   
        "userId         TEXT," \
        "rowLetter      CHAR(1) NOT NULL," \
        "colNumber      INTEGER NOT NULL," \
        "PRIMARY KEY(flightNumber, userId, rowLetter, colNumber)," \
        "FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);";

int createDataBase(void)
{
    dataBaseADT db = createDataBase(DB_NAME, true);
    if(createTables(db) < 0)
        return -1;




    return 0;
}

int createTables(dataBaseADT db)
{
    if(db == NULL)
        return -1;
    int result = 0;
    result += executeQueryDataBase(db, createFlightTableQuery, false);
    result += executeQueryDataBase(db, createFlightSeatsTableQuery, false);
    result += executeQueryDataBase(db, createReservationTableQuery, false);
    return result;
}

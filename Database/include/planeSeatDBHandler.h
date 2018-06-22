#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "dataBaseADT.h"


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


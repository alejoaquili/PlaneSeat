#ifndef PLANE_SEAT_SERIALIZED_H
#define PLANE_SEAT_SERIALIZED_H


#include "planeSeatDBHandler.h"


char * serializeFlights(flight_t * flights, int qty);

flight_t * deserializeToFlights(char * flightsText, int * qty);

char * serializeFlightSeats(flightSeat_t * fsd, int qty);

flightSeat_t * deserializeToFlightSeats(char * fsdText, int * qty);


#endif


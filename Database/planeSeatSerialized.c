#include <stdio.h>
#include <stdlib.h>
#include "errorslib.h"
#include "planeSeatDBHandler.h"
#include "serialization.h"

char * serializeFlights(flight_t * flights, int qty)
{
    arrayADT arrayFlights = newArray();
    for(int i = 0; i < qty; i++)
    {
        arrayADT flight = newArray();
        addNodeToArray(flight, newArrayNode((flights[i].flightNumber), String));
        addNodeToArray(flight, newArrayNode((flights[i].origin), String));
        addNodeToArray(flight, newArrayNode((flights[i].destination), String));
        addNodeToArray(arrayFlights, newArrayNode(flight, Array));
    }
    char * flightsText = serialize(arrayFlights, Array);
    freeArray(arrayFlights);
    return flightsText;
}

flight_t * deserializeToFlights(char * flightsText, int * qty)
{
    arrayADT arrayFlights = deserialize(flightsText);
    *qty = getArraySize(arrayFlights);
    flight_t * flights = calloc(*qty, sizeof(flight_t));

    for(int i = 0; i < *qty; i++)
    {
        arrayADT flightArray = (arrayADT) getValueInArray(arrayFlights, i);
        flights[i].flightNumber = (char *) getValueInArray(flightArray, 0);
        flights[i].origin = (char *) getValueInArray(flightArray, 1);
        flights[i].destination = (char *) getValueInArray(flightArray, 2); 
    }
    freeArray(arrayFlights);
    return flights;
}

char * serializeFlightSeats(flightSeat_t * fsd, int qty)
{
    arrayADT arrayFlightSeats = newArray();
    for(int i = 0; i < qty; i++)
    {
        arrayADT seat = newArray();
        addNodeToArray(seat, newArrayNode(fsd[i].flightNumber, String));
        addNodeToArray(seat, newArrayNode(&(fsd[i].colLetter), Integer));
        addNodeToArray(seat, newArrayNode(&(fsd[i].rowNumber), Integer));
        addNodeToArray(seat, newArrayNode(&(fsd[i].occupied), Integer));

        addNodeToArray(arrayFlightSeats, newArrayNode(seat, Array));
    }
    char * fsdText = serialize(arrayFlightSeats, Array);
    freeArray(arrayFlightSeats);
    return fsdText;
}

flightSeat_t * deserializeToFlightSeats(char * fsdText, int * qty)
{
    arrayADT arrayFlightSeats = deserialize(fsdText);
    *qty = getArraySize(arrayFlightSeats);
    flightSeat_t * fsd = calloc(*qty, sizeof(flightSeat_t));

    for(int i = 0; i < *qty; i++)
    {
        arrayADT seatArray = (arrayADT) getValueInArray(arrayFlightSeats, i);
        fsd[i].flightNumber = (char *) getValueInArray(seatArray, 0);
        fsd[i].colLetter = (char) *((int *) getValueInArray(seatArray, 1));
        fsd[i].rowNumber = *((int *) getValueInArray(seatArray, 2));
        fsd[i].occupied = *((int *) getValueInArray(seatArray, 3));   
    }
    freeArray(arrayFlightSeats);
    return fsd;
}



/*
char * serializeReservation(reservation_t reservation)
{
    arrayADT arrayReservation = newArray();
    
    addNodeToArray(arrayReservation, newArrayNode(reservation.flightNumber, String));
    addNodeToArray(arrayReservation, newArrayNode(reservation.userId, String));
    addNodeToArray(arrayReservation, newArrayNode(reservation.colLetter, Integer));
    addNodeToArray(arrayReservation, newArrayNode(reservation.rowNumber, Integer));

    char * reservationText = serialize(arrayReservation);
    freeArray(arrayReservation);
    return reservationText;
}

reservation_t deserializeToReservation(char * reservationText)
{
    arrayADT arrayReservation = deserialize(reservationText);
    reservation_t reservation;

    reservation.flightNumber = (char *) getValueInArray(arrayReservation, 0);
    reservation.userId = (char *) getValueInArray(arrayReservation, 1);
    reservation.colLetter = *((int *) getValueInArray(arrayReservation, 2)); 
    reservation.rowNumber = *((int *) getValueInArray(arrayReservation, 3)); 

    freeArray(arrayReservation);
    return reservation;
}*/



#ifndef COMMANDS_H
#define COMMANDS_H


int applyToGetFlights(int clientSocketFd);

int applyToAddFlight(int clientSocketFd);

int applyToDeleteFlight(int clientSocketFd);

int applyToReserve(int clientSocketFd);

int applyToCancel(int clientSocketFd);

int applyToPrintFlightDistribution(int clientSocketFd);


#endif


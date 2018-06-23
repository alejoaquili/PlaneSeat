#ifndef COMMANDS_H
#define COMMANDS_H

int help(int clientSocketFd);

int addFlight(int clientSocketFd);

int deleteFlight(int clientSocketFd);

int check(int clientSocketFd);

int reserve(int clientSocketFd);

int cancel(int clientSocketFd);


#endif
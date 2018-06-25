#ifndef SOCKET_LIB_H
#define SOCKET_LIB_H


int openSocket(void);

void bindToPort(int socketFd, int port);

int acceptClientSocket(int socketFd);

int connectClientToSocket(int clientSocketFd, int port, char * ip);

char * readStringToDeserialize(int socketFd);


#endif


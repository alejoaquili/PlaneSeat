#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "socketlib.h"

#define PORT 7200
#define IP "127.0.0.1"

int main (void)
{
	int clientSocketFd = openSocket();

	connectClientToSocket(clientSocketFd, PORT, IP);

	CLIENT_HANDLER(clientSocketFd);
}



#include <stdio.h>
#include "socketlib.h"
#include "planeSeatClientUI.h"

#define PORT 7200
#define IP "127.0.0.1"

int main (void)
{
	int clientSocketFd = openSocket();

	connectClientToSocket(clientSocketFd, PORT, IP);

	planeSeatClientUI(clientSocketFd);
}



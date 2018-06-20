#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socketLib.h"


int openSocket(void)
{
    int socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if(socketFd < 0)
        perror("socket() Failed");
    return socketFd;
}

void bindToPort(int socketFd, int port)
{
    struct sockaddr_in  inAddress;
    inAddress.sin_family = PF_INET;
    inAddress.sin_port = (in_port_t)htons(port);
    inAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int) == -1))
    	perror("setsockopt(): cannot reuse the socket");
    int result = bind(socket, (struct sockaddr *) &inAddress, sizeof(inAddress));
    if(c < 0)
    	perror("bind(): the port is currently in use");
}

int acceptClientSocket(int socketFd)
{
	int acceptClientSocketFd;
	struct sockaddr_storage acceptClientAddress;
	socklen_t addressSize = sizeof(acceptClientAddress);
	acceptClientSocketFd = accept(socketFd, (struct sockaddr *)&acceptClientAddress, &addressSize);
	if(acceptClientSocketFd < 0)
		perror("accept() Failed");
	return acceptClientSocketFd;
}
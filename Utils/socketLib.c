#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socketlib.h"
#include "errorslib.h"

int openSocket(void)
{
    int socketFd = socket(PF_INET, SOCK_STREAM, 0);
    checkFail(socketFd, "socket() Failed");
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
    	fail("setsockopt(): cannot reuse the socket");

    int result = bind(socketFd, (struct sockaddr *) &inAddress, sizeof(inAddress));
    checkFail(result, "bind(): the port is currently in use");
}

int acceptClientSocket(int socketFd)
{
	int acceptClientSocketFd;
	struct sockaddr_storage acceptClientAddress;
	socklen_t addressSize = sizeof(acceptClientAddress);
	
    acceptClientSocketFd = accept(socketFd, (struct sockaddr *)&acceptClientAddress, &addressSize);
	checkFail(acceptClientSocketFd, "accept() Failed");
	return acceptClientSocketFd;
}


int connectClientToSocket(int clientSocketFd, int port, char * ip)
{
	struct sockaddr_in inAddress;
	inAddress.sin_family = AF_INET;
	inAddress.sin_port = htons(port);
	inAddress.sin_addr.s_addr = inet_addr(ip);

	int result = connect(clientSocketFd, (struct sockaddr *)& inAddress, sizeof(inAddress));
	checkFail(result, "connect() Failed");
	return result;
}

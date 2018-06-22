#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7200
#define IP "127.0.0.1"

int connectClientToSocket(void);

int main (void)
{
	int clientSocketFd = connectClientToSocket();

}

int connectClientToSocket(void)
{
	int clientSocketFd = socket(PF_INET, SOCK_STREAM, 0);
	if(clientSocketFd < 0)
	{
		fprintf(stderr, "Client socket Failed.\n");
		return -1;
	}
	struct sockaddr_in inAddress;
	inAddress.sin_family = AF_INET;
	inAddress.sin_port = htons(PORT);
	inAddress.sin_addr.s_addr = inet_addr(IP);
	int result = connect(clientSocketFd, (struct sockaddr *)& inAddress, sizeof(inAddress));
	if(result < 0)
	{
		fprintf(stderr, "Client socket Failed.\n");
		close(clientSocketFd);
		return -1;
	}
	return clientSocketFd;
}


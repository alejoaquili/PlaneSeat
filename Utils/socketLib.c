

int openSocket(void)
{
    int socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if(socketFd < 0)
        perror("socket() Failed");
    return socketFd;
}

void bindToPort(int socketFd, int port)
{
    struct sockaddr_in  
}
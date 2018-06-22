#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <semaphore.h>
#include <netinet/in.h>
#include "socketlib.h"
#include "errorslib.h"
#include "server.h"

#define SEM_NAME "conectionsSemaphore"

void sigChildHandler(int sig);
void sigEndHandler(int sig);

static int listenFd;
static sem_t * semaphore;

int main(int argc , char *argv[])
{
    signal(SIGCHLD, sigChildHandler);
    signal(SIGKILL, sigEndHandler);
    signal(SIGTERM, sigEndHandler);
    signal(SIGQUIT, sigEndHandler);
    semaphore = sem_open(SEM_NAME, O_CREAT|O_EXCL, 0777, CONNECTIONS_QTY);

    listenFd = openSocket();
    checkFail(listenFd, "Error in listener\n");
    
    bindToPort(listenFd, PORT);
    checkFail(listen(listenFd, 10) == -1, "Unable to linten in this port\n");
    printf("Port binded");
    
    while(1)
    {
        int connectFd = acceptClientSocket(listenFd);

        sem_wait(semaphore);
        switch(fork()) 
        {
            case 0:
                close(listenFd);
                sem_close(semaphore);
                printf("Conexion aceptada\n");
                execlp(CONECTION_HANDLER, " ", ((char *)NULL));

                fail("exec() Failed");
                close(connectFd);
                exit(0);
            case -1:
                fail("Conection fork fail");
            default:
                close(connectFd); 
        }
    }
    return 0;
}

void sigChildHandler(int sig)
{
    sem_post(semaphore);
    while(waitpid(-1, 0, WNOHANG) > 0);
    return;
}


void sigEndHandler(int sig)
{
    sem_unlink(SEM_NAME);
    close(listenFd);
    return;
}


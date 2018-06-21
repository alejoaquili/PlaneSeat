#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <semaphore.h>
#include <netinet/in.h>

#define CONNECTIONS_QTY 2
#define PORT 7200

void sigchld_handler(int sig);

static sem_t * semaphore;

int main(int argc , char *argv[])
{
    signal(SIGCHLD, sigchldHandler);
    signal(SIGKILL, sigendHandler);
    signal(SIGTERM, sigendHandler);
    signal(SIGKICK, sigendHandler);
    semaphore = sem_open("santiputo3", O_CREAT|O_EXCL, 0777, CONNECTIONS_QTY);

    int listenFd = openSocket();
    checkFail(listenFd, "Error en listener\n");
    
    bindToPort(listenFd, PORT);
    checkFail(listen(listenFd, 10) == -1, "No es posible escuchar en ese puerto\n");
    printf("Enlazado al puerto");
    
    while(1)
    {
        struct sockaddr_storage client;
        unsigned int addressSize = sizeof(client);
        printf("Esperando al cliente");
        
        int connectFd = accept(listenFd, (struct sockaddr *)&client, &addressSize);
        if(connectFd == -1)
        {
            printf("se cago, hay q handelear la falla de Conexion de servidor");
        }

        sem_wait(semaphore);
        if(fork() == 0) 
        {
            close(listenFd);
            sem_close(semaphore);
            printf("Conexion aceptada\n");
            execlp(CONECTION_HANDLER, " ", ((char *)NULL));

			fail("exec() Failed");
            close(connectFd);
            exit(0);
        }
        close(connectFd); 
    }
    return 0;
}

void sigchldHandler(int sig)
{
    sem_post(semaphore);
    while(waitpid(-1, 0, WNOHANG) > 0);
    return;
}


void sigendHandler(int sig)
{
    sem_unlink(shm->semName);
    close()
    while(waitpid(-1, 0, WNOHANG) > 0);
    return;
}
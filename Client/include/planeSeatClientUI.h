#ifndef PLANE_SEAT_CLIENT_UI_H
#define PLANE_SEAT_CLIENT_UI_H

#define BUFFER_LENGTH 256
#define CLEAN_BUFFER { int MACc; while ((MACc = getchar()) != '\n' && MACc != EOF); }
#define IS_QUIT(word) (strcmp(word, "quit") == 0)

typedef struct {
    char * name;
    char* description;
    int (*function)(int clientSocketFd);
} command_t;

int planeSeatClientUI(int clientSocketFd);

void printPlane(flightSeat_t * seats);


#endif
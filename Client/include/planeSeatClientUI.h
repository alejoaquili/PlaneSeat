#ifndef PLANE_SEAT_CLIENT_UI_H
#define PLANE_SEAT_CLIENT_UI_H


#define CLEAN_BUFFER { int MACc; while ((MACc = getchar()) != '\n' && MACc != EOF); }
#define IS_QUIT(word) (strcmp(word, "quit") == 0)

#define BUFFER_LENGTH 256
#define COMMAND_QTY 7
#define VALID_COMMAND 0
#define INVALID_COMMAND -1

typedef struct command_t{
    char * name;
    char* description;
    int (*function)(int clientSocketFd);
} command_t;

int planeSeatClientUI(int clientSocketFd);


#endif


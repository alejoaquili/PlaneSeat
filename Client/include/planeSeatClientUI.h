#ifndef PLANE_SEAT_CLIENT_UI_H
#define PLANE_SEAT_CLIENT_UI_H


#define CLEAN_BUFFER { int MACc; while ((MACc = getchar()) != '\n' && MACc != EOF); }
#define IS_QUIT(word) (strcmp(word, "quit") == 0)


int planeSeatClientUI(int clientSocketFd);


#endif


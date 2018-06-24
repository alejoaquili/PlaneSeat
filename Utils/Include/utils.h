#ifndef UTILS_H
#define UTILS_H

typedef char boolean;

#define true 1
#define false 0


char * readStringToDeserialize(int fd);

void freeSpace(int qty, ...);

char * deserializeToString(char * string);

int deserializeToInt(char * string);


#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"
#include <unistd.h>
#include "serialization.h"

char * readStringToDeserialize(int fd)
{
    char buffer[10];
    read(fd, buffer, 10);
    int nbytes = valueOfInt(buffer);
    char * buffer2 = calloc(1, nbytes);
    read(fd, buffer2, nbytes);
    return buffer2;
}

void freeSpace(int qty, ...) 
{
	va_list args;
    va_start(args, qty);

    for (int i = 0; i < qty; i++)
        free(va_arg(args, void *));

    va_end(args);
}


char * deserializeToString(char * string)
{
    arrayADT array = deserialize(string);
    char * resp = (char *) getValueInArray(array, 0);
    freeArray(array);
    return resp;
}

int deserializeToInt(char * string)
{
    arrayADT array = deserialize(string);
    int resp = *((int *) getValueInArray(array, 0));
    freeArray(array);
    return resp;
}

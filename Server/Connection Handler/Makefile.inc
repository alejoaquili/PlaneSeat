
CC       = gcc
# Compiling Flags:
CFLAGS   = -g -c -Wall -pedantic -std=c99 -Wno-unused-function -I./include -I./../../Utils/include -I./../../Database/include

LINKER 	 = gcc
# Linking Flags:
LFLAGS 	 = -g -Wall -pedantic -std=c99 -lsqlite3 -Wno-unused-function
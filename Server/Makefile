include Makefile.inc

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)

all: clean obj

obj:$(SOURCES)
	$(CC) -c $(CFLAGS) $(SOURCES)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS)

.PHONY: clean  all


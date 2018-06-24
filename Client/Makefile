include Makefile.inc

TARGET := client
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)


all: clean comp link

comp:$(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

link:$(OBJECTS)
	$(LINKER) $(LFLAGS) $(OBJECTS) ./../Utils/*.o ./../Database/*.o -o $(TARGET).out

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

%.out : %.o
	$(LINKER) $(LFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGET).out

.PHONY: clean  all


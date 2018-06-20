include Makefile.inc

TARGET := AllTest
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
OUTPUTS := $(SOURCES:.c=.out)


all: clean comp link run

comp:$(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

link:$(OBJECTS)
	$(LINKER) $(LFLAGS) $(OBJECTS) ./../Utils/*.o ./../Database/*.o -o $(TARGET).out

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

%.out : %.o
	$(LINKER) $(LFLAGS) $< -o $@

clean:
	rm -rf $(OUTPUTS)
	rm -rf  $(OBJECTS)

run:
	./AllTest.out

.PHONY: clean  all obj run

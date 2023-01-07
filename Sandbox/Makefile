CC=g++
INCDIRS=-I.
CFLAGS=-Wall -Wextra $(INCDIRS) -g 

BINARY=sandbox
CPPFILES=automata.cpp parser.cpp sandbox.cpp
OBJECTS=automata.o parser.o sandbox.o

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^
	
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^
	
clean:
	rm -rf $(OBJECTS) $(BINARY)

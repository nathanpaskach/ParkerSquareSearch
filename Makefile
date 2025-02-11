OBJECTS= bigd.o bigdigits.o main.o
INCLUDES= bigd.h bigdigits.h bigdtypes.h
CFLAGS= -std=c99 -pedantic -Wall -Wpointer-arith -Wstrict-prototypes -Wno-format -O2 -g
CC= gcc

default: ParkerSquareSearch.exe

ParkerSquareSearch.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ParkerSquareSearch

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $<
    
clean:
	del /S *.o
	del /S *.exe
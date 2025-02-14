OBJECTS= bigd.o bigdigits.o main.o
OBJECTS128= main128.o
INCLUDES= bigd.h bigdigits.h bigdtypes.h
CFLAGS= -std=c99 -Wall -Wpointer-arith -Wstrict-prototypes -Wno-format -O3
CC= gcc

default: ParkerSquareSearch.exe ParkerSquareSearch128.exe

ParkerSquareSearch.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ParkerSquareSearch
    
ParkerSquareSearch128.exe: $(OBJECTS128)
	$(CC) $(CFLAGS) $(OBJECTS128) -o ParkerSquareSearch128

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $<
    
clean:
	del /S *.o
	del /S *.exe
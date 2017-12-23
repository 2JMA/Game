########################################################
CC=gcc
CFLAGS= -g -Wall -ansi -pedantic
LMFLAGS = -lm
THREADFLAGS = -pthread
#CFLAGS = -g -O3 -ansi -pedantic
EJS = main mainImpossible mainExchanges mainFinal 
########################################################
OBJECTSMAIN = nprint.o image.o place.o utils.o resources.o objects.o character.o
########################################################

all: $(EJS)

main: $(OBJECTSMAIN)
	$(CC) $(CFLAGS) -o main main.c $(OBJECTSMAIN) $(LMFLAGS) $(THREADFLAGS)

mainImpossible: $(OBJECTSMAIN)
	$(CC) $(CFLAGS) -o mainImpossible mainimpossible.c $(OBJECTSMAIN) $(LMFLAGS) $(THREADFLAGS)

mainExchanges: $(OBJECTSMAIN)
	$(CC) $(CFLAGS) -o mainExchanges mainExchanges.c $(OBJECTSMAIN) $(LMFLAGS) $(THREADFLAGS)

mainFinal: $(OBJECTSMAIN)
	$(CC) $(CFLAGS) -o mainFinal mainFinal.c $(OBJECTSMAIN) $(LMFLAGS) $(THREADFLAGS)

nprint.o: nprint.c nprint.h
	$(CC) $(CFLAGS) -c nprint.c

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

place.o: place.c place.h
	$(CC) $(CFLAGS) -c place.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

resources.o: resources.c resources.h
	$(CC) $(CFLAGS) -c resources.c

objects.o: objects.c objects.h
	$(CC) $(CFLAGS) -c objects.c

character.o: character.c character.h
	$(CC) $(CFLAGS) -c character.c

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS)

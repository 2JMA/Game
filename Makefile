########################################################
CC=gcc
CFLAGS= -g -Wall -ansi -pedantic
LMFLAGS = -lm
THREADFLAGS = -pthread
#CFLAGS = -g -O3 -ansi -pedantic
EJS = main 
########################################################
OBJECTSMAIN = nprint.o image.o place.o utils.o resources.o objects.o character.o mainImpossible.o mainExchanges.o mainFinal.o mainQuestions.o
########################################################

all: $(EJS)

main: $(OBJECTSMAIN)
	$(CC) $(CFLAGS) -o main main.c $(OBJECTSMAIN) $(LMFLAGS) $(THREADFLAGS)

mainImpossible.o: mainImpossible.c mainImpossible.h
	$(CC) $(CFLAGS) -c mainImpossible.c

mainExchanges.o: mainExchanges.c mainExchanges.h
	$(CC) $(CFLAGS) -c mainExchanges.c 

mainFinal.o: mainFinal.c mainFinal.h
	$(CC) $(CFLAGS) -c mainFinal.c

mainQuestions.o: mainQuestions.c mainQuestions.h
	$(CC) $(CFLAGS) -c mainQuestions.c

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

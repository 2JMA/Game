#ifndef TYPES_H
#define TYPES_H

#define NUM_COLS 240
#define NUM_ROWS 68
#define OC_CHAR '+'
#define DOOR_CHAR '#'
/*
Number of rows and columns of the terminal
Using tput cols and tput lines on the terminal*/


#define Y_ELSE 666
#define X_ELSE 666

typedef enum {
 ERROR = -1, OK = 1
} Status;

typedef enum {
 FALSE = 0, TRUE = 1
} Bool; 

typedef enum {
 FAR = 0, NEAR = 1, INSIDE = 2
} Position;

typedef enum {
 OCCUPIED = 0, AVAILABLE = 1, DOOR = 2
} PlaceAvailable;


typedef struct Location{
	int x;
	int y;
}location;

#endif /* TYPES_H */

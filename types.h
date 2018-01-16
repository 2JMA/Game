#ifndef TYPES_H
#define TYPES_H

#define NUM_COLS 146
#define NUM_ROWS 58
#define OC_CHAR 1
#define DOOR_CHAR 2
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

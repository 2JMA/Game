#ifndef TYPES_H
#define TYPES_H

#define NUM_COLS 139
#define NUM_ROWS 52
#define OC_CHAR 1
/*
Number of rows and columns of the terminal
Using tput cols and tput lines on the terminal*/

typedef enum {
 ERROR = 0, OK = 1
} Status;

typedef enum {
 FALSE = 0, TRUE = 1
} Bool; 

typedef enum {
 FAR = 0, NEAR = 1, INSIDE = 2
} Position;

typedef struct Location{
	int x;
	int y;
}location;

#endif /* TYPES_H */

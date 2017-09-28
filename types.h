#ifndef TYPES_H
#define TYPES_H

typedef enum {
 ERROR = 0, OK = 1
} Status;

typedef enum {
 FALSE = 0, TRUE = 1
} Bool; 

typedef struct location{
	int lat;
	int lng;
};

#endif /* TYPES_H */

#ifndef UTILS_H
#define UTILS_H

#include "image.h"

/*Aux strunct to pass the arguments
The first image is the one compared to the others*/
typedef struct thread_near_args{
    Image **img; 
    int numImg;
    int pos;
}thread_near_args;

typedef struct thread_shoot_args{
	Place *place;
    Image *shooter;
    Image *evil;
    int move;
    /*At the moment, move is just -1 it the bullet goes up
     or +1 if it goes down*/
}thread_shoot_args;

typedef struct thread_guard_args{
    Image *amok;
    Place *textRect;
    int speed;
    int numImg;
    int *moveType;
    Image **images;
    location *moveDir;
}thread_guard_args;


char *fileToStr(char *file);

/*Stops the program x miliseconds*/
void sleep(unsigned int miliSeconds);
void multLocation(location *loc, int num);
#endif /* UTILS_H */
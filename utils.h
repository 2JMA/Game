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

char *fileToStr(char *file);

/*Stops the program x miliseconds*/
void sleep(unsigned int miliSeconds);
#endif /* UTILS_H */
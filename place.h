#ifndef PLACE_H
#define PLACE_H
 
#include <stdio.h>
#include <stdlib.h>   
#include "types.h"

typedef struct _Place Place;

Place *createPlace(int r, int c, char *fileName, int bgColor, int fgColor, char wall, char bg);
void freePlace(Place *p);
/*Set up the place with a custom map from a string*/
Status setUpPlace(Place *p, char *data);
/*Print the place*/
void printPlace(Place *place);
/*Return true if the place available between xi-xf and yi-yf is free*/
Status placeAvailable(Place *p, int xi, int xf, int yi, int yf);
/*A series of functions to get propierties of a place*/
char placeGetWall(Place *p);
char placeGetBg(Place *p);
int placeGetBgColor(Place *p);
int placeGetFgColor(Place *p);

#endif  /* PLACE_H */

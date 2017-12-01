#ifndef PLACE_H
#define PLACE_H
 
#include <stdio.h>
#include <stdlib.h>   
#include "types.h"

typedef struct _Place Place;

Place *createPlace(int r, int c, int nR, int nC, int bgColor, int fgColor);
void freePlace(Place *p);
/*Set up the place with a custom map from a string*/
Status setUpPlace(Place *p, char *data);
/*Return true if the place available between xi-xf and yi-yf is free*/
Status placeAvailable(Place *p, int xi, int xf, int yi, int yf);

#endif  /* PLACE_H */

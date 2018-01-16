#ifndef PLACE_H
#define PLACE_H
 
#include <stdio.h>
#include <stdlib.h>   
#include "types.h"

typedef struct _Place Place;

Place *createPlace(int row, int column, char *fileName, int bgColor, int fgColor, char wall, char bg);
void freePlace(Place *p);
/*Set up the place with a custom map from a string*/
Status setUpPlace(Place *p, char *data);
/*Print the place*/
void printPlace(Place *place);

/*Print some text inside a place that is just a rectangle, with no more that
one line for each side as wall*/
int printInsidePlace(Place *place, char *text, int color);

/*Return true if the place available between xi-xf and yi-yf is free*/
PlaceAvailable placeAvailable(Place *p, int xi, int xf, int yi, int yf);
/*A series of functions to get propierties of a place*/
char placeGetWall(Place *p);
char placeGetBg(Place *p);
int placeGetBgColor(Place *p);
int placeGetFgColor(Place *p);

int placeGetLastRow(Place *p);
int placeGetLastColumn(Place *p);
int placeGetFirstRow(Place *p);
int placeGetFirstColumn(Place *p);

#endif  /* PLACE_H */

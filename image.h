#ifndef IMAGE_H
#define IMAGE_H
 
#include <stdio.h>
#include <stdlib.h>    
#include <math.h>
#include "types.h"
#include "place.h"

/*Default bg and fg colors*/
#define OR_BG 40
#define OR_FG 37

typedef struct _Image Image;

Image *createImage(char *src, int r, int c, int bgColor, int fgColor, Place *place);
void freeImage(Image *img);

PlaceAvailable imageChangePosition(Image *img, int x, int y);
void imageChangeColor(Image *img, int bgColor, int fgColor);
PlaceAvailable imageMoveTo(Image *img, int x, int y);
PlaceAvailable imageMove(Image *img, int x, int y);
/*If wait==TRUE, it waits for the thread to stop*/ 
PlaceAvailable imageSmoothMoveTo(Image *img, int x, int y, int time, Bool wait);
PlaceAvailable imageSmoothMove(Image *img, int x, int y);

Position imagesNear(Image *img1, Image *img2);
int getImageX(Image *img);
int getImageY(Image *img);
int getImageRows(Image *img);
int getImageCols(Image *img);

int printImageData(Image *i);

void initScreen();

int imagePrint(Image *img);
void imageClear(Image *img);

#endif  /* IMAGE_H */


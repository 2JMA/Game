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

Status imageChangePosition(Image *img, int lat, int lng);
void imageChangeColor(Image *img, int bgColor, int fgColor);
Status imageMoveTo(Image *img, int lat, int lng);
Status imageMove(Image *img, int lat, int lng);
Status imageSmoothMoveTo(Image *img, int lat, int lng);
Status imageSmoothMove(Image *img, int lat, int lng);

Bool imagesNear(Image *img1, Image *img2);
int getImageX(Image *img);
int getImageY(Image *img);
int getImageRows(Image *img);
int getImageCols(Image *img);

int printImageData(Image *i);

void initScreen();

int imagePrint(Image *img);
void imageClear(Image *img);

#endif  /* IMAGE_H */


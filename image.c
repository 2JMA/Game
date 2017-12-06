#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <string.h>

#include "nprint.h"
#include "utils.h"
#include "image.h"

/*Element to be printed an cleared.
It is the main graphical element of the game.
in order to move it, you have to clear it, change iRow, iColumn, 
and print it again
*/
struct _Image{
	char *src;
	int bgColor, fgColor;
	int iColumn, iRow;
	int nColumns, nRows;
	Place *place;
};


Image *createImage(char *fileName, int r, int c, int bgColor, int fgColor, Place *place){
	Image *img;
	char *p, *src;
	if(fileName == NULL) return NULL;
	if(place == NULL) return NULL;
	src = fileToStr(fileName);
	if(src == NULL) return NULL;

	img = (Image*)malloc(sizeof(Image));
	if(img == NULL) return NULL;

	img->iColumn=c;
	img->iRow=r;
	img->bgColor = bgColor;
	img->fgColor = fgColor;
	img->place = place;
	img->src = src;

	//Get the number of rows and columns of the str, as if it was a matrix
	int temp;
	for(p=src, img->nColumns=1, img->nRows=1, temp=1; *p!='\0'; p++){
		/*I don't know how it wors, it is from stackoverflow*/
		if ((*p & 0xC0) != 0x80) ++temp;

		if(*p=='\n'){
			img->nRows += 1;
			if(temp > img->nColumns){
				//We don't want to count the \n
				img->nColumns=temp-1;
			}
			temp=1;
		}
	}

	//Because the last char mey be a \0, not \n
	if(temp > img->nColumns){
		//We don't want to count the \n
		img->nColumns=temp-1;
	}

	if(c + img->nColumns -1 > NUM_COLS) img->iColumn = NUM_COLS - img->nColumns;
	if(r + img->nRows -1 > NUM_ROWS)  img->iRow = NUM_ROWS - img->nRows;

	return img;
}

/*Free the image*/
void freeImage(Image *img){
	if(img == NULL) return;
	if(img->src == NULL) return;
	free(img->src);
	free(img);
}

/*Change the image colors*/
void imageChangeColor(Image *img, int bgColor, int fgColor){
	if(img == NULL) return;

	img->bgColor = bgColor;
	img->fgColor = fgColor;
}

/*Change the image coordinates*/
Status imageChangePosition(Image *img, int x, int y){
	Status result;
	if(img == NULL) return ERROR;
	if(x <= 0 || y <= 0) return ERROR;
	if(x + img->nColumns -1 > NUM_COLS) return ERROR;
	if(y + img->nRows -1 > NUM_ROWS) return ERROR;

	result = placeAvailable(img->place, x, x + img->nColumns, y, y + img->nRows);
	if(result != 1){
		return result;
	}

	img->iColumn = x;
	img->iRow = y;
	return OK;
}


/*Move the image to an exact position*/
Status imageMoveTo(Image *img, int x, int y){
	Status result;
	imageClear(img);
	result = imageChangePosition(img, x, y);
	imagePrint(img);
	return result;
}

/*Move the image by incrementing its position*/
Status imageMove(Image *img, int x, int y){
	Status result;
	imageClear(img);
	result = imageChangePosition(img, img->iColumn + x, img->iRow + y);
	imagePrint(img);
	return result;
}
/*Move the image slowly to an exact position*/
Status imageSmoothMoveTo(Image *img, int x, int y){
	
}

/*Move the image slowly by incrementing its position*/
Status imageSmoothMove(Image *img, int x, int y){
	
}

/*Return TRUE just if the two images are strictly next to each other*/
Bool imagesNear(Image *img1, Image *img2){
	int c1x, c2x, c1y, c2y;
	if(img1 ==  NULL || img2 == NULL) return FALSE;

	c1x = img1->iColumn + img1->nColumns/2;
	c2x = img2->iColumn + img2->nColumns/2;
	c1y = img1->iRow + img1->nRows/2;
	c2y = img2->iRow + img2->nRows/2;

	double dist = sqrt( pow(c1x-c2x, 2) + pow(c1y-c2y, 2));
	double rad1 = 2*(img1->nColumns/2 +img1->nRows/2)/3;
	double rad2 = 2*(img2->nColumns/2 + img2->nRows/2)/3;

	_move_cursor_to(0, 0);
	printf("\n\n%f, %f\n %f, %f", dist, rad1+rad2, rad1, rad2);

	return dist <= rad1+rad2;
}

int getImageX(Image *img){
	if(img == NULL) return -1;
	return img->iColumn;
}

int getImageY(Image *img){
	if(img == NULL) return -1;
	return img->iRow;
}

int getImageRows(Image *img){
	if(img == NULL) return -1;
	return img->nRows;
}

int getImageCols(Image *img){
	if(img == NULL) return -1;
	return img->nColumns;
}

/*Just for debugging, print the image info in the left upper corner*/
int printImageData(Image *i){
	_move_cursor_to(0, 0);
	printf("%d, %d\n%d, %d", i->iColumn, i->iRow, i->nColumns, i->nRows);
}

int imagePrint(Image *img){
    int n, i, nRows;
    if(img == NULL) return -1;

    _prepare_font(img->bgColor, img->fgColor);
    _move_cursor_to(img->iRow, img->iColumn);
    
    i=0;
    n=0;
    nRows = 0;

    while(img->src[i] != '\0'){
    	if(img->src[i] != '\n'){
    		n += printf("%c", img->src[i]);
    	}else{
    		nRows++;
    		_move_cursor_to(img->iRow + nRows, img->iColumn);
    	}
    	i++;
    }
    
    fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
}

void imageClear(Image *img){
    int i, j, nRows;
    char bg;
    if(img == NULL) return;

    _prepare_font(placeGetBgColor(img->place), placeGetFgColor(img->place));
    _move_cursor_to(img->iRow, img->iColumn);
    bg = placeGetBg(img->place);
    
    for(i=0; i < img->nRows ; i++){
    	_move_cursor_to(img->iRow + i, img->iColumn);
   		for(j=0; j< img->nColumns; j++) {
    		printf("%c", bg);
    	}
    }

    fflush(stdout);
}

/*Example from stackoverflow to get the len of unicode characters*/
size_t utf8len(char *s){
    size_t len = 0;
    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
    return len;
}

/*DEPRECATED*/
void imageClear2(Image *img){
    int i, j;

    if(img == NULL) return;

    _prepare_font(OR_BG, OR_FG);
    _move_cursor_to(img->iRow, img->iColumn);

    //Clear all the spaces of the image
    for(i=0; i< img->nRows; i++){
    	for(j=0; j< img->nColumns; j++){
    		printf(" ");
    	}
    	printf("\n");
    }
    fflush(stdout);
}

/*int nPrint(Image *img, int bgColor, int fgColor){
    int n;
    _prepare_font(bgColor, fgColor);
    _move_to(img->iRow, img->iColumn);
    n = printf("%s", img->src);
    fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
}*/
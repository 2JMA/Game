#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include <pthread.h>

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

pthread_mutex_t mutex;


Image *createImage(char *fileName, int r, int c, int bgColor, int fgColor, Place *place){
	Image *img;
	char *p, *src;
	int temp;
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

	/*Get the number of rows and columns of the str, as if it was a matrix*/
	for(p=src, img->nColumns=1, img->nRows=1, temp=1; *p!='\0'; p++){
		/*I don't know how it wors, it is from stackoverflow*/
		if ((*p & 0xC0) != 0x80) ++temp;

		if(*p=='\n'){
			img->nRows += 1;
			if(temp > img->nColumns){
				/*We don't want to count the \n*/
				img->nColumns=temp-1;
			}
			temp=1;

		}else if(*p=='\r' && *(p+1)=='\n'){
			/*Fix compatibility issues ith windows files*/
			p++;
			img->nRows += 1;
			if(temp > img->nColumns){
				/*We don't want to count the \n*/
				img->nColumns=temp-1;
			}
			temp=1;
		}
	}

	/*Because the last char mey be a \0, not \n*/
	if(temp > img->nColumns){
		/*We don't want to count the \n*/
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
PlaceAvailable imageChangePosition(Image *img, int x, int y){
	PlaceAvailable result;
	if(img == NULL) return ERROR;
	if(x <= 0 || y <= 0) return ERROR;
	if(x + img->nColumns -1 > NUM_COLS) return ERROR;
	if(y + img->nRows -1 > NUM_ROWS) return ERROR;

	result = placeAvailable(img->place, x, x + img->nColumns, y, y + img->nRows);
	if(result == OCCUPIED){
		return result;
	}

	img->iColumn = x;
	img->iRow = y;
	return result;
}


/*Move the image to an exact position*/
PlaceAvailable imageMoveTo(Image *img, int x, int y){
	PlaceAvailable result;
	pthread_mutex_lock(&mutex);
	imageClear(img);
	result = imageChangePosition(img, x, y);
	imagePrint(img);
	pthread_mutex_unlock(&mutex);
	return result;
}

/*Move the image by incrementing its position*/
PlaceAvailable imageMove(Image *img, int x, int y){
	PlaceAvailable result;
	pthread_mutex_lock(&mutex);
	imageClear(img);
	result = imageChangePosition(img, img->iColumn + x, img->iRow + y);
	imagePrint(img);
	pthread_mutex_unlock(&mutex);
	return result;
}

/*Aux strunct to pass the arguments*/
struct thread_args{
    Image *img;
    int x, y, time;
    PlaceAvailable result;
};

/*Auxiliar function to move the image slowly to an exact position*/
void *imageSmoothMoveToAux_thread(void *arguments){
	int i, diff, s;
	PlaceAvailable result;
	struct thread_args *args;
	if(arguments == NULL) pthread_exit(NULL);
	args = (struct thread_args *)arguments;

	if(args->img->iColumn == args->y){
		/*printf("Caso 1\n");*/
		/*We need the number of positions and the sign of the slide*/
		diff = args->x - args->img->iRow;
		s = diff/abs(diff);
		diff = abs(diff);
		for(i=0; i<diff; i + s){
			result = imageMove(args->img, 0, s);
			if(result != AVAILABLE){
				/*printf("ERROR: %d\n", result);*/
				args->result = result;
				pthread_exit(NULL);
			}
			sleep(args->time);
		}
		args->result = AVAILABLE;
		pthread_exit(NULL);
	}else if(args->img->iRow == args->x){
		/*printf("Caso 2\n");*/
		diff = args->y - args->img->iColumn;
		s = diff/abs(diff);
		diff = abs(diff);
		for(i=0; i<diff; i + s){
			result = imageMove(args->img, s, 0);
			if(result != AVAILABLE){
				/*printf("ERROR: %d\n", result);*/
				args->result = result;
				pthread_exit(NULL);
			}
			sleep(args->time);
		}
		args->result = AVAILABLE;
		pthread_exit(NULL);
	}else{
		/*printf("Caso 3\n");*/

	}
	return NULL;
}

PlaceAvailable imageSmoothMoveToAux(Image *img, int x, int y, int time){
	int i, j;
	PlaceAvailable result;

	if(img->iColumn == y){
		/*printf("Caso 1\n");*/
		/*We need the number of positions and the sign of the slide*/
		int diff = x - img->iRow;
		int s = diff/abs(diff);
		diff = abs(diff);
		for(i=0; i<diff; i + s){
			result = imageMove(img, 0, s);
			if(result != AVAILABLE){
				/*printf("ERROR: %d\n", result);*/
				return result;
			}
			sleep(time);
		}
		return AVAILABLE;
	}else if(img->iRow == x){
		/*printf("Caso 2\n");*/
		int diff = y - img->iColumn;
		int s = diff/abs(diff);
		diff = abs(diff);
		for(i=0; i<diff; i + s){
			result = imageMove(img, s, 0);
			if(result != AVAILABLE){
				/*printf("ERROR: %d\n", result);*/
				return result;
			}
			sleep(time);
		}
		return AVAILABLE;
	}else{
		/*printf("Caso 3\n");*/

	}
}

/*Move the image slowly to an exact position*/
PlaceAvailable imageSmoothMoveTo(Image *img, int x, int y, int time, Bool wait){
	pthread_t p;
	PlaceAvailable result = ERROR;
	struct thread_args *args = NULL;

	if(wait == TRUE){
		result = imageSmoothMoveToAux(img, x, y, time);
	}else{
		pthread_create(&p, NULL, imageSmoothMoveToAux_thread, (void *)args);
		args = (struct thread_args *) malloc(sizeof(struct thread_args));
		args->img = img;
		args->x = x;
		args->y = y;
		args->time = time;
		args->result = OK;
	}
	if(args != NULL)
		free(args);

	return result;
}

/*Move the image slowly by incrementing its position*/
PlaceAvailable imageSmoothMove(Image *img, int x, int y){
	return -1;
}

/*Auxiliar image to detect wether to images are really near or not
It returns FALSE if not and TRUE if yes.
TODO RETURN INTS SO THAT WE KNOW IF THEY ARE NEAR OR THEY ARE TOUCHING EACH OTHER

This is obviously not efficient at all, but it was the best way i have found to solve this problem.
We alloc two more rows and columns than necessary in order to check if the sides of a point are occupied or not
Otherwise we may get a core violation error
*/
Position imagesNearAux(Image *img1, Image *img2){
	short **matrix, flag;
	int xMin, xMax, yMin, yMax;
	int i, j;
	Position result = FAR;
	if(img1 ==  NULL || img2 == NULL) return FAR;

	if(img1->iColumn < img2->iColumn){
		xMin = img1->iColumn;
	}else{
		xMin = img2->iColumn;
	}

	if(img1->iRow < img2->iRow){
		yMin = img1->iRow;
	}else{
		yMin = img2->iRow;
	}

	if((img1->iColumn + img1->nColumns) < (img2->iColumn + img2->nColumns)){
		xMax = img2->iColumn + img2->nColumns;
	}else{
		xMax = img1->iColumn + img1->nColumns;
	}

	if((img1->iRow + img1->nRows) < (img2->iRow + img2->nRows)){
		yMax = img2->iRow + img2->nRows;
	}else{
		yMax = img1->iRow + img1->nRows;
	}

	matrix = (short **)malloc(sizeof(short*)*(yMax - yMin + 3));
	if(matrix == NULL){
		return FAR;
	}

	for(i=0; i<(yMax - yMin + 3); i++){
		matrix[i] = (short*)calloc((xMax - xMin + 3),sizeof(short));
		if(matrix[i] == NULL){
			for(j=i; j>=0; j--){
				free(matrix[j]);
			}
			free(matrix);
			return FAR;
		}
	}

	/*Once we have the matrix, we print the images inside and see if they are near or not*/
	/*Print the first image*/
	for(i=(img1->iRow - yMin + 1); i<(img1->iRow + img1->nRows - yMin  + 1); i++){
		for(j=(img1->iColumn - xMin + 1); j<(img1->iColumn + img1->nColumns - xMin  + 1); j++){
			matrix[i][j]=1;
		}
	}

	for(i=(img2->iRow - yMin + 1), flag=0; i<(img2->iRow + img2->nRows - yMin + 1); i++){
		for(j=(img2->iColumn - xMin + 1); j<(img2->iColumn + img2->nColumns - xMin + 1); j++){

			if(matrix[i][j]==1){
				/*Uno esta encima del otro*/
				result = INSIDE;
				flag = 1;
				break;
			}
			if(matrix[i-1][j]==1 || matrix[i+1][j]==1 || matrix[i][j-1]==1 || matrix[i][j+1]==1){
				/*Estan cerca*/
				result = NEAR;
				flag = 1;
				break;
			}
		}
		if(flag == 1) break;
	}

	for(i=0; i<(yMax - yMin + 1); i++){
		free(matrix[i]);
	}
	free(matrix);
	return result;
}

/*Return TRUE just if the two images are strictly next to each other*/
Position imagesNear(Image *img1, Image *img2){
	int c1x, c2x, c1y, c2y;
	double dist, rad1, rad2, radMax;
	if(img1 ==  NULL || img2 == NULL) return -1;

	c1x = img1->iColumn + img1->nColumns/2;
	c2x = img2->iColumn + img2->nColumns/2;
	c1y = img1->iRow + img1->nRows/2;
	c2y = img2->iRow + img2->nRows/2;

	dist = sqrt( pow(c1x-c2x, 2) + pow(c1y-c2y, 2));
	rad1 = sqrt( pow(img1->nColumns, 2) + pow(img1->nRows, 2));
	rad2 = sqrt( pow(img2->nColumns, 2) + pow(img2->nRows, 2));
	radMax = rad1;
	if(rad1 < rad2) radMax = rad2;

	/*_move_cursor_to(0, 0);
	printf("\n\n%f, %f\n %f, %f", dist, rad1+rad2, rad1, rad2);*/

	if(dist > radMax) return FAR;

	return imagesNearAux(img1, img2);
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
	return printf("%d, %d\n%d, %d", i->iColumn, i->iRow, i->nColumns, i->nRows);
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
    int i, j;
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

    /*Clear all the spaces of the image*/
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

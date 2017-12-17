#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "nprint.h"
#include "utils.h"

#include "place.h"


/*Element to be printed an cleared.
It is the main graphical element of the game.
in order to move it, you have to clear it, change iRow, iColumn, 
and print it again
*/
struct _Place{
	int **matrix;
	int bgColor, fgColor;
	int iColumn, iRow;
	int nColumns, nRows;
	char wall, bg;
};

Place *createPlace(int r, int c, char *fileName, int bgColor, int fgColor, char wall, char bg){
	int i, j, temp;
	char *p, *map;
	Status result;
	if(fileName == NULL) return NULL;

	map = fileToStr(fileName);
	if(map == NULL) return NULL;

	Place *place = (Place*)malloc(sizeof(place[0]));
	if(place == NULL) return NULL;

	place->bgColor = bgColor;
	place->fgColor = fgColor;
	place->wall = wall;
	place->bg = bg;
	place->iColumn = c;
	place->iRow = r;


	//Get the number of rows and columns of the map, as if it was a matrix
	for(p = map, place->nColumns = 0, place->nRows = 1, temp = 0; *p!='\0'; p++){
		/*I don't know how it wors, it is from stackoverflow*/
		if ((*p & 0xC0) != 0x80) ++temp;

		if(*p=='\n'){
			place->nRows += 1;
			if(temp > place->nColumns){
				//We don't want to count the \n
				place->nColumns = temp-1;
			}
			temp = 0;
		}
	}

	//Because the last char mey be a \0, not \n
	if(temp > place->nColumns){
		//We don't want to count the \n
		place->nColumns=temp-1;
	}

	if(c + place->nColumns -1 > NUM_COLS) place->iColumn = NUM_COLS - place->nColumns;
	if(r + place->nRows -1 > NUM_ROWS)  place->iRow = NUM_ROWS - place->nRows;

	place->matrix=(int **)malloc(sizeof(int*)*(place->nRows));
	if(place->matrix == NULL){
		free(place);
		free(map);
		return NULL;
	} 

	for(i=0; i<place->nRows; i++){
		/*Initialize all the map with zeros*/
		place->matrix[i] = (int*)calloc(place->nColumns, sizeof(int));
		if(place->matrix[i] == NULL){
			for(j=i; j>=0; j--){
				free(place->matrix[j]);
			}
			free(place->matrix);
			free(place);
			free(map);
			return NULL;
		}
	}

	result = setUpPlace(place, map);
	if(result != OK){
		freePlace(place);
		free(map);
		return NULL;
	}
	free(map);

	return place;
}

void printPlace(Place *place){
	int i, j;
	if(place == NULL) return;

	_prepare_font(place->bgColor, place->fgColor);

	for(j=0; j < place->nRows; j++){
		_move_cursor_to(place->iRow + j, place->iColumn);
		for(i=0; i < place->nColumns; i++){
			if(place->matrix[j][i] == OC_CHAR)
				printf("%c", place->wall);
			else
				printf("%c", place->bg);
		}
	}
	
	fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
}

void freePlace(Place *p){
	int i;
	if(p == NULL) return;
	if(p->matrix == NULL) return;

	for(i=0; i < p->nRows; i++){
		if(p->matrix[i] != NULL){
			free(p->matrix[i]);
		}
	}

	free(p->matrix);
	free(p);
}

Status setUpPlace(Place *p, char *data){
	int i, j;
	char *c;
	if(p == NULL || data == NULL) return ERROR;

	for(c = data, i=0, j=0; *c != '\0'; c++){
		if(*c != '\n'){
			if(j >= p->nRows || i >= p->nColumns) return ERROR;

			if(*c != ' ' && *c != '\0'){
				p->matrix[j][i] = OC_CHAR;
			}else{
				p->matrix[j][i] = 0;
			}
			i++;
		}else{
			i=0;
			j++;
		}
	}
	return OK;
}

/*Return true if the place available between xi-xf and yi-yf is free*/
Status placeAvailable(Place *p, int xi, int xf, int yi, int yf){
	if(p == NULL) return ERROR;
	int i, j;

	if(xi < p->iColumn || yi < p->iRow) return -1;
	if(xf > p->nColumns + p->iColumn || yf > p->nRows + p->iRow) return -2;
 	
 	/*Change the baricentric reference of the object.*/
 	xi -=  p->iColumn;
 	xf -=  p->iColumn;
 	yi -=  p->iRow;
 	yf -=  p->iRow;

	for(i=yi; i<yf; i++){
		for(j=xi; j<xf; j++){
			if(p->matrix[i][j] == OC_CHAR)
				return -3;
		}
	}

	return OK;
}

char placeGetWall(Place *p){
	if(p == NULL) return ' ';
	return p->wall;
}

char placeGetBg(Place *p){
	if(p == NULL) return ' ';
	return p->bg;
}

int placeGetBgColor(Place *p){
	if(p == NULL) return -1;
	return p->bgColor;
}

int placeGetFgColor(Place *p){
	if(p == NULL) return -1;
	return p->fgColor;
}
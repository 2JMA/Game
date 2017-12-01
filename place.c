#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "nprint.h"

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
};

Place *createPlace(int r, int c, int nRows, int nColumns, int bgColor, int fgColor){
	int i, j;
	Place *place=(Place*)malloc(sizeof(place[0]));
	if(place == NULL) return NULL;

	place->bgColor = bgColor;
	place->fgColor = fgColor;
	place->iColumn = c;
	place->iRow = r;
	place->nColumns = nColumns;
	place->nRows = nRows;

	/*TODO FIX */
	if(r+nRows > NUM_ROWS) place->nRows = NUM_ROWS - r -1;
	if(c+nColumns > NUM_COLS) place->nRows = NUM_COLS - c -1;

	place->matrix=(int **)malloc(sizeof(int*)*nRows);
	if(place->matrix == NULL){
		free(place);
		return NULL;
	} 

	for(i=0; i<nRows; i++){
		/*Initialize all the map with zeros*/
		place->matrix[i] = (int*)calloc(nColumns, sizeof(int));
		if(place->matrix[i] == NULL){
			for(j=i; j>=0; j--){
				free(place->matrix[j]);
			}
			free(place->matrix);
			free(place);
			return NULL;
		}
	}

	return place;

}

void printPlace(Place *place){
	int i, j;
	if(place == NULL) return;

	_prepare_font(place->bgColor, place->fgColor);
    _move_cursor_to(place->iRow, place->iColumn);

	for(j=0; j < place->nRows; j++){
		for(i=0; i < place->nColumns; i++){
			if(place->matrix[j][i] == 0)
				printf("=");
		}
		_move_cursor_to(place->iRow + j, place->iColumn);
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
			if(j > p->nRows || i > p->nColumns) return ERROR;

			if(*c != ' '){
				p->matrix[j][i] = 1;
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
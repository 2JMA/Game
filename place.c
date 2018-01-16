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
	Place *place;
	Status result;
	if(fileName == NULL) return NULL;

	map = fileToStr(fileName);
	if(map == NULL) return NULL;

	place = (Place*)malloc(sizeof(place[0]));
	if(place == NULL) return NULL;

	place->bgColor = bgColor;
	place->fgColor = fgColor;
	place->wall = wall;
	place->bg = bg;
	place->iColumn = c;
	place->iRow = r;


	/*Get the number of rows and columns of the map, as if it was a matrix*/
	for(p = map, place->nColumns = 0, place->nRows = 1, temp = 0; *p!='\0'; p++){
		/*I don't know how it works, it is from stackoverflow*/
		if ((*p & 0xC0) != 0x80) ++temp;

		if(*p=='\n'){
			place->nRows += 1;
			if(temp > place->nColumns){
				/*We don't want to count the \n*/
				place->nColumns = temp-1;
			}
			temp = 0;
		}else if(*p=='\r' && *(p+1)=='\n'){
			/*Fix compatibility issues ith windows files*/
			p++;
			place->nRows += 1;
			if(temp > place->nColumns){
				/*We don't want to count the \n*/
				place->nColumns = temp-1;
			}
			temp = 0;
		}
		
	}

	/*Because the last char may be a \0, not \n*/
	if(temp > place->nColumns){
		/*We don't want to count the \n*/
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
			else if (place->matrix[j][i] == DOOR_CHAR)
				printf("%c", place->bg);
				/*The doors have no especial character atm*/
			else if (place->matrix[j][i] == NLINE_CHAR)
				printf("%c", '!');
			else
				printf("%c", place->bg);
		}
	}

	fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
}

int printInsidePlace(Place *place, char *text, int color){
	int n, iCol, i, j;
	char *c;
	if(place == NULL || text == NULL) return -1;
	if(strlen(text) > (place->nRows-4)*(place->nColumns-4)) return -1;

	/*First of all we clear the space*/
	printPlace(place);

	iCol = place->iColumn + 2;
	if(place->iColumn <= 0){
		iCol = 3;
	}
	_move_cursor_to(place->iRow +2, iCol);
	_prepare_font(place->bgColor, color);

	if(strlen(text) < place->nColumns-4){
		/*The text fits in just one line*/
		n = printf("%s", text);
	}else{
		for(c=text, i=0, j=0, n=0; *c != '\0'; c++, i++){
			/*i is the number of column, j is the number of row*/
			if(*c == '\n'){
				i = 0;
				j++;
				_move_cursor_to(place->iRow +2+j, iCol);
			}else if(i < place->nColumns-5){
				n+=printf("%c", *c);
			}else if(i == place->nColumns-5 && (*c!='\n' || *c!=' ')){
				n+=printf("-");
				i = 0;
				j++;
				_move_cursor_to(place->iRow +2+j, iCol);
				n+=printf("%c", *c);
			}else{
				_move_cursor_to(place->iRow +2+j, iCol);
				n+=printf("%c", *c);
			}
		}
	}

	fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
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
		/*Ignore the \r and use just the \n*/
		if(*c=='\r' && *(c+1)=='\n') continue;
		if(*c != '\n'){
			if(j >= p->nRows || i >= p->nColumns) return ERROR;

			if(*c == '#'){
				p->matrix[j][i] = DOOR_CHAR;
			}else if(*c != ' ' && *c != '\0'){
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
PlaceAvailable placeAvailable(Place *p, int xi, int xf, int yi, int yf){
	int i, j;
	if(p == NULL) return ERROR;

	if(xi < p->iColumn || yi < p->iRow) return ERROR;
	if(xf > p->nColumns + p->iColumn || yf > p->nRows + p->iRow) return ERROR;

 	/*Change the baricentric reference of the object.*/
 	xi -=  p->iColumn;
 	xf -=  p->iColumn;
 	yi -=  p->iRow;
 	yf -=  p->iRow;

	for(i=yi; i<yf; i++){
		for(j=xi; j<xf; j++){
			if(p->matrix[i][j] == OC_CHAR)
				return OCCUPIED;
			else if(p->matrix[i][j] == DOOR_CHAR)
				return DOOR;
		}
	}

	return AVAILABLE;
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

int placeGetLastRow(Place *p){
	if(p == NULL) return -1;
	return p->iRow + p->nRows-1;
}

int placeGetLastColumn(Place *p){
	if(p == NULL) return -1;
	return p->iColumn + p->nColumns-1;
}

int placeGetFirstRow(Place *p){
	if(p == NULL) return -1;
	return p->iRow;
}

int placeGetFirstColumn(Place *p){
	if(p == NULL) return -1;
	return p->iColumn;
}
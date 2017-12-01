#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include "nprint.h"
#include "types.h"


/*
  Prints on the screen the style for writing on this window,
  foreground colour, background colour, and font style.
 */
void _prepare_font(int bgColor, int fgColor) {
    printf("%c[%d;%dm", 27, bgColor, fgColor);
    return;
}

/*It should return the max row and column of the terminal. Unfortunately it doesn't work*/
int _get_screen_dim(int *x, int*y){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    printf("%c[6n", 27);
    fflush(stdout);
    scanf("\033[%d;%dR", y, x);
}

/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 */
void _move_cursor_to(int r, int c) {
    if (r < 0) r = 0;
    if (c < 0) c = 0;
    if (r >= NUM_ROWS) r = NUM_ROWS-1;
    if (c >= NUM_COLS) c = NUM_COLS-1;
    printf("%c[%d;%dH", 27, r, c);
    return;
}

void _clean_screen(){
  printf("\e[1;1H\e[2J");
}

void _init_screen(){
	/*Ajustar el tamaño del terminal filas;columnas 
	printf("%c[3;3;100t", 27);*/
    _prepare_font(OR_BG, OR_FG);
    _clean_screen();
}

/*int imagePrint(Image *img){
    int n;
    if(img == NULL) return -1;

    _prepare_font(img->bgColor, img->fgColor);
    _move_to(img->iRow, img->iColumn);
    n = printf("%s", img->src);
    fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
}

void imageClear(Image *img){
    int i, j;
    _prepare_font(OR_BG, OR_FG);
    _move_to(img->iRow, img->iColumn);

    //Clear all the spaces of the image
    for(i=0; i< img->nRows; i++){
    	for(j=0; j< img->nColumns; j++){
    		printf(" ");
    	}
    	printf("\n");
    }
    fflush(stdout);
}

int nPrint(Image *img, int bgColor, int fgColor){
    int n;
    _prepare_font(bgColor, fgColor);
    _move_to(img->iRow, img->iColumn);
    n = printf("%s", img->src);
    fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
}*/
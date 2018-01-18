#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>
#include "nprint.h"
#include "utils.h"
#include "types.h"

char *fileToStr(char *file){
	char *row, *temp, *string;
	FILE *f;

	/*max amount of text the terminal can display*/
	row = (char*) malloc(sizeof(row[0])*NUM_COLS);
	temp = (char*) malloc(sizeof(temp[0])*(NUM_COLS*NUM_ROWS+1));

	if(row==NULL || temp==NULL){
		free(row);
		free(temp);
		return NULL;
	}

	f = fopen(file, "r");
	if( f == NULL){
		free(row);
		free(temp);
		return NULL;
	}

	fgets(row,NUM_COLS,f);
	strcpy(temp,row);

	while(!feof(f)){
		fgets(row,NUM_COLS,f);
		strcat(temp,row);
	}
	fclose(f);
	free(row);

	/*Copy the string into another one more size-optimized*/
	string = (char *)malloc(sizeof(string[0])*(1+strlen(temp)));
	if(string == NULL){
		free(temp);
		return NULL;
	}

	strcpy(string, temp);
	free(temp);
	return string;	
}

void sleep(unsigned int miliSeconds){
    clock_t goal = miliSeconds*1000 + clock();
    while (goal > clock());
}

void multLocation(location *loc, int num){
	if(loc == NULL) return;

	(loc->x) *= num;
	(loc->y) *= num;
}

void exitGame(int mode){
	struct termios new;	       
	tcgetattr(fileno(stdin), &new);    
	new.c_lflag &= ICANON;	              
	new.c_lflag &= ECHO;
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;	         
	tcsetattr(fileno(stdin), TCSANOW, &new);
	_move_cursor_to(NUM_ROWS-1, NUM_COLS-1);
	_show_cursor();
	exit(mode);
}

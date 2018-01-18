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

location _read_key() {
	char choice;
  	location dir;
 	choice = fgetc(stdin);

	dir.x = 0;
 	dir.y = 0;

	if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
	    choice = fgetc(stdin);

	    switch(choice) {
		    case('A'):
		  	    dir.y = -1;
		   		break;
		    case('B'):
		      	dir.y = 1;
		      	break;
		    case('C'):
		      	dir.x = 1;
		      	break;
		    case('D'):
		      	dir.x = -1;
		      	break;

		    /*Exchanges and questions*/  	
		    case('t'):
				dir.x=1;
				dir.y=1;
				break;
			case('e'):
				dir.x=2;
				dir.y=2;
				break;
	    }
  	}else if(choice == ' '){
 	  	/*Shot*/
 	  	dir.x = 2;
 	  	dir.y = 0;
 	}else{
 		dir.x = 0;
 	  	dir.y = 0;
 	}

 	return dir;   
}

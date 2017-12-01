#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>

#include "nprint.h"
#include "place.h"
#include "image.h"
#include "utils.h"



//From nprint
#define OR_BG 40
#define OR_FG 37

#define MAX_LINE 301
#define CYAN_FG 36
#define RED_FG 31
#define MAGENTA_FG 35
#define YELLOW_FG 33

#define CYAN_BG 46
#define RED_BG 41
#define MAGENTA_BG 45

#define WHITE_BG 47
#define WHITE_FG 37
#define BLACK_BG 40
#define BLACK_FG 30

struct termios initial;

/*
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;	              /*here we are setting up new. This line tells to stop the 
						canonical mode (which means waiting for the user to press 
						enter before sending)*/
	new.c_lflag &= ~ECHO;                 /*by deactivating echo, we tell the terminal NOT TO 
						show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;                  /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;	              /*I really have no clue what this does, it must be somewhere in the book tho*/
	/*new.c_lflag &= ~ISIG;                 here we discard signals: the program won't end even if we 
						press Ctrl+C or we tell it to finish*/

	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
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
	    }
  	}else{
  		dir.x = -1;
		dir.y = -1;
  	}
 	return dir;   
}

void sleep(unsigned int miliSeconds){
    clock_t goal = miliSeconds*1000 + clock();
    while (goal > clock());
}

void main(){
	int MAX_X, MAX_Y;
	char line[MAX_LINE];
	FILE *f = NULL;


	f = fopen("draw.txt", "r");

	char draw[] = "¯\\_(ツ)_/¯";
	char bearDraw[] = "ʕ•ᴥ•ʔ";
	char cute[] = "(づ｡◕‿‿◕｡)づ";
	char cute2[] = "｡◕‿‿◕｡";
	char works1[] = "(ᵔᴥᵔ)";
	char works2[] = "♡‿♡";
	char test[] = "\n          ██████████            \n      ████          ████        \n    ██                  ██      \n  ██                      ██    \n  ██  ██  ██              ██    \n██    ██  ██        ████    ██  \n██                ██    ██  ██  \n██  ██  ██  ██          ██  ██  \n██  ██████████        ██      ██\n██  ██████████                ██\n  ██  ██████████              ██\n  ██  ██  ██  ██            ██  \n    ██                  ████    \n      ██████        ████        \n            ████████            ";
	char test2[] = "██████████";
	char *map;

	if(f == NULL){
		printf("Error al abrir el archivo");
		return;
	}

	_term_init();
	_init_screen();

	Place *place = createPlace(10, 10, 26, 54, OR_BG, WHITE_FG);
	Image *iDraw = createImage(draw, 40, 20, OR_BG, CYAN_FG, place);
	Image *iBear = createImage(bearDraw, 10, 20 , OR_BG, RED_FG, place);
	Image *iCute = createImage(cute2, 40, 40 , OR_BG, YELLOW_FG, place);
	Image *iTest = createImage(test, 20, 10 , CYAN_BG, MAGENTA_FG, place);

	map=fileToStr("map.txt");
	setUpPlace(place, map);
	printPlace(place);
	//imagePrint(iDraw);
	imagePrint(iBear);
	//imagePrint(iCute);
	//imagePrint(iTest);

	/*sleep(2000);
	printf("HOLA\n");
	sleep(5000);
	nclear(i);*/

	/*int i;
	for(i=0; i<=14; i++){
		sleep(200);
		imageMove(iDraw, 1, 1);
		imageMove(iCute, -2, -3);
	}*/

	location dir;
	Bool near1;
	while(1){

		dir = _read_key();
		Status r = imageMove(iBear, dir.x, dir.y);
		/*printData(iBear);
		
		near1 = imagesNear(iCute, iBear); 

		_move_cursor_to(0, 0);
		printf("%d", r);*/
	}

	freeImage(iDraw);
	freeImage(iBear);
	freeImage(iCute);
	freeImage(iTest);
	freePlace(place);

	fclose(f);
}
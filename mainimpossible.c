#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>

#include "nprint.h"
#include "place.h"
#include "image.h"
#include "utils.h"
#include <pthread.h>



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

void _term_reset() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */

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

void thread_function_move_recta1(Image* im){
	if (im==NULL) return;
	int i;

	while(1){
		for(i=1; i<=20; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}

		for(i=1; i<=20; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}
	}
}

void thread_function_move_recta2(Image* im){
	if (im==NULL) return;
	int i;

	while(1){
		for(i=1; i<=20; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}

		for(i=1; i<=20; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}
	}
}

void thread_function_move_cuadrado1(Image* im){
	if (im==NULL) return;
	int i;

	while(1){
		for(i=1; i<=20; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}

		for(i=1; i<=20; i++){
			imageMove(im, 0, 1);
			sleep(200);
		}

		for(i=1; i<=20; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}

		for(i=1; i<=20; i++){
			imageMove(im, 0, -1);
			sleep(200);
		}
	}
}

void main(){
	int MAX_X, MAX_Y;
	char line[MAX_LINE];
	pthread_t p1, p2, p3;

	_term_init();
	_init_screen();

	Place *place = createPlace(10, 10, "Maps/map3.txt", OR_BG, YELLOW_FG, '#', '.');
	Image *iBear = createImage("Images/prueba.txt", 12, 20 , OR_BG, RED_FG, place);
	Image *im1 = createImage("Images/1.txt", 15, 20 , OR_BG, CYAN_FG, place);
	Image *im2 = createImage("Images/2.txt", 20, 40 , OR_BG, CYAN_FG, place);
	Image *im3 = createImage("Images/3.txt", 14, 25 , OR_BG, CYAN_FG, place);
	Image *win = createImage("Images/win.txt", 23, 25 , OR_BG, CYAN_FG, place);
	Image *lose = createImage("Images/lose.txt", 23, 25 , OR_BG, CYAN_FG, place);


	printPlace(place);
	imagePrint(iBear);
	imagePrint(im1);
	imagePrint(im2);
	imagePrint(im3);

	pthread_create(&p1,NULL,thread_function_move_recta1, im1);
	pthread_create(&p2,NULL,thread_function_move_recta2, im2);
	pthread_create(&p3,NULL,thread_function_move_cuadrado1, im3);

	location dir;
	Position near1,near2,near3;
	int times = 0;
	while(times < 200){

		dir = _read_key();
		imageMove(iBear, dir.x, dir.y);
		times++;
		/*
		printImageData(iBear);
		*/
		near1 = imagesNear(im1, iBear);
		near2 = imagesNear(im2, iBear);
		near3 = imagesNear(im3, iBear);
		_move_cursor_to(0, 0);
		printf("%d, %d", getImageX(iBear),getImageY(iBear));
		
		/*printf("%d", near1);
		printf("%d", near2);
		printf("%d", near3);
		*/
		if ((near1==2)||(near2==2)||(near3==2)){
			times=200;
			imageClear(im1);
			imageClear(im2);
			imageClear(im3);
			imagePrint(lose);
			_move_cursor_to(0, 0);
			
		}
		else if ((getImageX(iBear)==61) && (getImageY(iBear)==35)){
			times=200;
			imageClear(im1);
			imageClear(im2);
			imageClear(im3);
			imagePrint(win);
			_move_cursor_to(0, 0);
			
		}
	}
	pthread_cancel(p1);
	pthread_cancel(p2);
	pthread_cancel(p3);

	freeImage(iBear);
	freeImage(im1);
	freeImage(im2);
	freeImage(im3);
	freePlace(place);
	_term_reset();
	return;
}
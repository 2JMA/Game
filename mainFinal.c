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

void *thread_read_key(void *direction) {
	char choice;
	location *dir;

 	dir = (location *)direction;

 	while(1){
 		choice = fgetc(stdin);
 		if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
 		    choice = fgetc(stdin);
 	
 		    switch(choice) {
 			    case('A'):
 			  	    dir->y = -1;
 			   		break;
 			    case('B'):
 			      	dir->y = 1;
 			      	break;
 			    case('C'):
 			      	dir->x = 1;
 			      	break;
 			    case('D'):
 			      	dir->x = -1;
 			      	break;
 		    }
 	  	}
 	
 	 	choice = 0;
 	 	sleep(100);
 	}
}

void* thread_imagesNear(void *arguments){
	int i;
	Position p;
	if(arguments == NULL) return NULL;
	thread_near_args *args;
	args = (thread_near_args *)arguments;
	while(1){
		for(i=1; i<args->numImg; i++){
			p = imagesNear(args->img[0], args->img[i]);
			if(p == INSIDE){
				args->pos = p;
				pthread_exit(NULL);
				return NULL;
			}
		}
	}
}

void main(){
	int MAX_X, MAX_Y;
	char line[MAX_LINE];
	pthread_t read_keys, near_thread;
	location dir;
	thread_near_args args;

	_term_init();
	_init_screen();

	Place *place = createPlace(1, 1, "Maps/square1.txt", OR_BG, YELLOW_FG, '#', '.');
	Image *iBear = createImage("Images/bear.txt", 12, 20 , OR_BG, RED_FG, place);
	Image *evil = createImage("Images/emoji2.txt", 23, 25 , OR_BG, CYAN_FG, place);
	Image *bullet1 = createImage("Images/bullet.txt", 21, 22 , OR_BG, YELLOW_FG, place);
	Image *bullet2 = createImage("Images/bullet.txt", 10, 4 , OR_BG, YELLOW_FG, place);


	printPlace(place);
	imagePrint(iBear);
	imagePrint(evil);
	imagePrint(bullet1);
	imagePrint(bullet2);
	//imageSmoothMoveTo(bullet1, 0, 22, 50);

	Image *imgs[] = {iBear, evil, bullet1, bullet2};
	args.img = imgs;
	args.numImg = 4;
	args.pos = 0;

	int times = 0;
	dir.x = 0;
	dir.y = 0;

	pthread_create(&read_keys, NULL, thread_read_key, &dir);
	pthread_create(&near_thread, NULL, thread_imagesNear, &args);
	while(args.pos == 0){

		if(dir.x !=0 || dir.y != 0){
			imageMove(iBear, dir.x, dir.y);
			imagePrint(bullet1);
			imagePrint(bullet2);
			imagePrint(evil);
			dir.x = 0;
			dir.y = 0;
			times++;
		}
	}

	printf("Acabado\n");

	pthread_cancel(read_keys);
	//pthread_cancel(near_thread);
	freeImage(iBear);
	freeImage(evil);
	freeImage(bullet1);
	freeImage(bullet2);
	freePlace(place);
	_term_reset();
	return;
}
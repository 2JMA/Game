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

#define NUM_LEVELS 3


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

void thread_function_move_recta12(Image* im){
	int i;
	if (im==NULL) return;

	while(1){
		for(i=1; i<=30; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}

		for(i=1; i<=30; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}
	}
}

void thread_function_move(thread_guard_args* args){
	int i;
	PlaceAvailable result;
	Position p;
	if (args==NULL){
		printInsidePlace(args->textRect, "ARGS == NULL", placeGetFgColor(args->textRect));
		return;
	}

	imagePrint(args->amok);
	for(i=0; i<args->numImg; i++){
		result = imagePrint((Image *)args->images[i]);
	}


	while(1){
		for(i=0; i<args->numImg; i++){
			result = imageMove((Image *)args->images[i], args->moveDir[i].x, args->moveDir[i].y);

			if(result != AVAILABLE){
				multLocation(&(args->moveDir[i]), -1);	
			}
	

			p = imagesNear(args->amok, (Image *)args->images[i]);
			if(p == INSIDE || p == NEAR){
				printInsidePlace(args->textRect, "OOH, YOU HAVE BEEN KILLED", placeGetFgColor(args->textRect));
				_move_cursor_to(placeGetLastRow(args->textRect)+1, 1);
				args->res = 0;
				pthread_exit(NULL);
			}

			if(args->res == 1){
				pthread_exit(NULL);
			}

		}
		sleep(args->speed);
	}

}


void modifyGuards(thread_guard_args *args){

	if (args->res==0){
		args->res=-1;
	}else if (args->res==1){
		if (args->level==1){
			args->res = -1;
			args->numImg = 5;
			args->speed = 50;
			args->level=2;
		}
		else if (args->level==2){
			args->res = -1;
			args->numImg = 7;
			args->speed = 30;
			args->level=3;
		}

	}

	if (args->level==1){
		imageMoveTo(args->amok, 2, 2);
		imageMoveTo(args->images[0], 60, 20);
		imageMoveTo(args->images[1], 120, 30);
		imageMoveTo(args->images[2], 180, 30);

	}else if (args->level==2){
		imageMoveTo(args->amok, 2, 2);
		imageMoveTo(args->images[0], 60, 20);
		imageMoveTo(args->images[3], 80, 40);
		imageMoveTo(args->images[2], 180, 30);
		imageMoveTo(args->images[1], 120, 30);
		imageMoveTo(args->images[4], 140, 30);
		
	}else if (args->level==3){
		imageMoveTo(args->amok, 2, 2);
		imageMoveTo(args->images[0], 60, 20);
		imageMoveTo(args->images[3], 80, 40);
		imageMoveTo(args->images[5], 100, 30);
		imageMoveTo(args->images[1], 120, 30);
		imageMoveTo(args->images[4], 140, 30);
		imageMoveTo(args->images[6], 160, 30);
		imageMoveTo(args->images[2], 180, 30);
	}

}

int mainImpossible(Place *map, Place *textRect, Place *infoRect, Image *amok){
	char *mapStr;
	pthread_t guardThread;
	thread_guard_args guardArgs;
	location l1, l2, l3, l4, l5, l6, l7;
	PlaceAvailable result;

	/*Set up the images and places*/
	imageMoveTo(amok, 4, 4);
	mapStr = fileToStr("Maps/ImpossibleGameMap.txt");
	if(mapStr == NULL) return -1;
	result = setUpPlace(map, mapStr);
	free(mapStr);

	printPlace(map);
	printPlace(textRect);
	printPlace(infoRect);
	imagePrint(amok);


	/*Game itself*/
	Image *im1 = createImage("Images/guard.txt", 6, 10 , OR_BG, CYAN_FG, map);
	Image *im2 = createImage("Images/guard.txt", 16, 40 , OR_BG, CYAN_FG, map);
	Image *im3 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im4 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im5 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im6 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im7 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	
	location dir;
	Position near1,near2,near3;
	int times = 0, i=0, fwin=0, op=0;

	imageMoveTo(amok, 2, 2);
	imageMoveTo(im1, 60, 20);
	imageMoveTo(im4, 80, 40);
	imageMoveTo(im6, 100, 30);
	imageMoveTo(im2, 120, 30);
	imageMoveTo(im5, 140, 30);
	imageMoveTo(im7, 160, 30);
	imageMoveTo(im3, 180, 30);
	printPlace(map);
	
	printInsidePlace(textRect, "LEVEL 1\n", OR_BG);	

	l1.x=0;
	l1.y=1;
	l2.x=0;
	l2.y=-1;
	l3.x=0;
	l3.y=1;
	l4.x=0;
	l4.y=-1;
	l5.x=0;
	l5.y=1;
	l6.x=-1;
	l6.y=1;
	l7.x=-1;
	l7.y=1;


	Image *images[] = {im1, im2, im3, im4, im5, im6, im7};
	location moveDir[] = {l1, l2, l3, l4, l5, l6, l7};
	guardArgs.amok = amok;
	guardArgs.images = images;
	guardArgs.moveDir = moveDir;
	guardArgs.textRect = textRect;
	guardArgs.numImg = 3;
	guardArgs.speed = 80;
	guardArgs.res = -1;
	guardArgs.level = 1;

	pthread_create(&guardThread, NULL, thread_function_move, &guardArgs);

	while(1){
		dir = _read_key();
		result= imageMove(amok, dir.x, dir.y);
		times++;
		if(result == DOOR){
			printInsidePlace(textRect, "CONGRATS! YOU PASSED THE LEVEL", placeGetFgColor(textRect));
			guardArgs.res=1;
			/*Let the thread recognize that we have won*/
			sleep(100);
		}

		if(guardArgs.res == 0 || guardArgs.res == 1){
			/*We lost the game*/
			modifyGuards(&guardArgs);
			pthread_create(&guardThread, NULL, thread_function_move, &guardArgs);
		}
	}
	pthread_cancel(guardThread);

	freeImage(im1);
	freeImage(im2);
	freeImage(im3);
	freeImage(im4);
	freeImage(im5);
	freeImage(im6);
	freeImage(im7);

	return 1;
}

void main(){
	

	_term_init();
	_init_screen();

	Place *place = createPlace(1, 1, "Maps/ImpossibleGameMap.txt", OR_BG, YELLOW_FG, '#', ' ');
	Place *textRect = createPlace(placeGetLastRow(place)+1, placeGetFirstColumn(place), "Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	Place *infoRect = createPlace(placeGetFirstRow(place), placeGetLastColumn(place)+1, "Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	Image *amok = createImage("Images/amok.txt", 2, 2 , OR_BG, RED_FG, place);

	mainImpossible(place, textRect, infoRect, amok);

	freeImage(amok);	
	freePlace(place);
	_term_reset();
	return;

}

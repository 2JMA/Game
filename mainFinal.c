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

struct termios initial;
int win = -1;
int liveEvil = 6; 
/*Max time to win, in seconds*/
double maxTime = 20;

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

void *thread_evil_move(void *image){
	Image *img;
	if(image == NULL) return NULL;
	img = (Image *)image;
	int move = 10;
	while(liveEvil > 0){
		imageSmoothMoveTo(img, getImageY(img), getImageX(img)+move, 60, TRUE);
		move = -1*move;
	}
}

void *thread_shoot(void *arguments){
	Status result;
	Position p;
	thread_shoot_args *args;
	Image *bullet;

	if(arguments == NULL) return NULL;
	args = (thread_shoot_args *)arguments;
	bullet = createImage("Images/bullet.txt", getImageY(args->shooter)+args->move, getImageX(args->shooter) , OR_BG, YELLOW_FG, args->place);
	imagePrint(bullet);

	while(1){
		result = imageMove(bullet, 0, args->move);
		if(result != AVAILABLE){
			imageClear(bullet);
			freeImage(bullet);
			pthread_exit(NULL);
		}

		p = imagesNear(bullet, (Image *)args->evil);
		if(p == INSIDE || p == NEAR){
			/*Quitar vida al personaje y comprobar si esta muerto o no*/
			liveEvil--;
			imageClear(bullet);
			freeImage(bullet);
			pthread_exit(NULL);
		}
		sleep(100);
	}

	imageClear(bullet);
	freeImage(bullet);
	pthread_exit(NULL);
}

void *thread_program_running(thread_info_places *args){
	char infoTextTime[100], infoTextLive[100];
	if(args == NULL){
		nprint("ES NULL\n", -1, -1, -1, -1);
		exitGame(0);
	} 

	while(1){
		if(liveEvil == 0){
			printInsidePlace(args->textRect, "Congratulations!\nYou have beaten the final evil, Hitler, and you should be proud of yourself, not too many people achive it.", placeGetFgColor(args->textRect));
			_move_cursor_to(placeGetLastRow(args->textRect)+1, 1);
			win = 1;
			pthread_exit(NULL);
		}else if( maxTime <= 0){
			printInsidePlace(args->textRect, "Sorry!\nYou weren't fast enought to beat final evil, Hitler, but don't worry, you have another oportunity. Press any letter to continue.", placeGetFgColor(args->textRect));
			_move_cursor_to(placeGetLastRow(args->textRect)+1, 1);
			win = 0;
			pthread_exit(NULL);
		}

		sprintf(infoTextTime, "   Time left: %.02lf secs\n   Hitler's life: %d", maxTime, liveEvil);
		sprintf(infoTextLive, "   Hitler's life: %d", liveEvil);
		printInsidePlaceRows(args->infoRect, infoTextTime, OR_FG, 4, 2);
		maxTime-=0.25;
		sleep(250);
	}
}

int finalGame(Place *map, Place *textRect, Place *infoRect, Image *amok){
	pthread_t evil_thread, shoot_thread, running_thread;
	location dir;
	thread_near_args args;
	thread_info_places infoArgs;
	thread_shoot_args shootArgs;

	char *mapStr;
	Status result;
	/*Set up the images and places*/
	Image *evil = createImage("Images/hitler.txt", 3, 20 , OR_BG, CYAN_FG, map);
	result = imageMoveTo(amok, 100, 40);
	mapStr = fileToStr("Maps/square1.txt");
	if(mapStr == NULL) return -1;
	result = setUpPlace(map, mapStr);
	free(mapStr);
	if(result != OK){
		nprint("CUIDADO, NO CARGA", -1, -1, NUM_ROWS -1, NUM_COLS-1);
		return -1;
	}

	printPlace(map);
	printPlace(textRect);
	printPlace(infoRect);
	imagePrint(amok);
	imagePrint(evil);

	/*Game itself*/
	Image *imgs[] = {amok, evil};
	args.img = imgs;
	args.numImg = 2;
	args.pos = 0;

	int times = 0;
	dir.x = 0;
	dir.y = 0;

	shootArgs.shooter = amok;
	shootArgs.place = map;
	shootArgs.evil = evil;
	shootArgs.move = -1;

	infoArgs.textRect = textRect;
	infoArgs.infoRect = infoRect;

	pthread_create(&running_thread, NULL, thread_program_running, &infoArgs);
	pthread_create(&evil_thread, NULL, thread_evil_move, evil);

	while(win == -1){
		dir = _read_key();
		if(dir.x == 2){
			pthread_create(&shoot_thread, NULL, thread_shoot, &shootArgs);
			dir.x = 0;
			dir.y = 0;
		}else if(dir.x !=0 || dir.y != 0){
			imageMove(amok, dir.x, dir.y);
			imagePrint(evil);
			dir.x = 0;
			dir.y = 0;
			times++;
		}
	}

	pthread_cancel(running_thread);
	pthread_cancel(evil_thread);

	freeImage(evil);
	return win;
}
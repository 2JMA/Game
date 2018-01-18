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

/* From nprint */
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
	if (args==NULL) return;

	while(1){
		for(i=0; i<args->numImg; i++){
			if(args->moveType[i] == 0){
				result = imageMove((Image *)args->images[i], args->moveDir[i].x, args->moveDir[i].y);

				if(result != OK){
					multLocation(&(args->moveDir[i]), -1);
				}
			}
	

			p = imagesNear(args->amok, (Image *)args->images[i]);
			if(p == INSIDE || p == NEAR){
				printInsidePlace(args->textRect, "OOH, YOU HAVE BEEN KILLED", placeGetFgColor(args->textRect));
				_move_cursor_to(placeGetLastRow(args->textRect)+1, 1);
				exit(0);
				/*pthread_exit(NULL);*/
			}
		}
		sleep(args->speed);
	}

}

void thread_function_move_recta2(Image* im){
	int i;
	if (im==NULL) return;

	while(1){
		for(i=1; i<=30; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}

		for(i=1; i<=30; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}
	}
}

void thread_function_move_cuadrado1(Image* im){
	int i;
	if (im==NULL) return;

	while(1){
		for(i=1; i<=30; i++){
			imageMove(im, 1, 0);
			sleep(200);
		}

		for(i=1; i<=10; i++){
			imageMove(im, 0, 1);
			sleep(200);
		}

		for(i=1; i<=30; i++){
			imageMove(im, -1, 0);
			sleep(200);
		}

		for(i=1; i<=10; i++){
			imageMove(im, 0, -1);
			sleep(200);
		}
	}
}

void main(){
	int MAX_X, MAX_Y;
	char line[MAX_LINE];
	pthread_t guardThread;
	thread_guard_args guardArgs;
	location l1, l2, l3;

	_term_init();
	_init_screen();

	Place *place = createPlace(1, 1, "Maps/square1.txt", OR_BG, YELLOW_FG, '#', ' ');
	Place *textRect = createPlace(placeGetLastRow(place)+1, placeGetFirstColumn(place), "Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	Place *infoRect = createPlace(placeGetFirstRow(place), placeGetLastColumn(place)+1, "Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	printPlace(textRect);
	printPlace(infoRect);
	Image *iBear = createImage("Images/prueba.txt", 2, 2 , OR_BG, RED_FG, place);
	Image *im1 = createImage("Images/1.txt", 6, 10 , OR_BG, CYAN_FG, place);
	Image *im2 = createImage("Images/2.txt", 16, 40 , OR_BG, CYAN_FG, place);
	Image *im3 = createImage("Images/3.txt", 8, 15 , OR_BG, CYAN_FG, place);

	location dir;
	Position near1,near2,near3;
	int times = 0, i=0, fwin=0, op=0;

	/*LEVEL 1*/
	while(i<2){
		imageMoveTo(iBear, 2, 2);
		imageMoveTo(im1, 6, 10);
		imageMoveTo(im2, 16, 40);
		imageMoveTo(im3, 8, 15);
		printPlace(place);
		imagePrint(iBear);
		imagePrint(im1);
		imagePrint(im2);
		imagePrint(im3);
		printInsidePlace(textRect, "LEVEL 1\n", OR_BG);	

		l1.x=1;
		l1.y=0;
		l2.x=-1;
		l2.y=0;
		l3.x=-1;
		l3.y=1;

		int moveType[] = {0, 0, 0};
	    Image *images[] = {im1, im2, im3};
	    location moveDir[] = {l1, l2, l3};
		guardArgs.amok = iBear;
		guardArgs.numImg = 3;
		guardArgs.moveType = moveType;
		guardArgs.images = images;
		guardArgs.moveDir = moveDir;
		guardArgs.textRect = textRect;
		guardArgs.speed = 80;

		pthread_create(&guardThread, NULL, thread_function_move, &guardArgs);

		while(times < 200){

			dir = _read_key();
			imageMove(iBear, dir.x, dir.y);
			times++;	

			/*if ((near1==2)||(near2==2)||(near3==2)){
				pthread_cancel(guardThread);					
				printInsidePlace(textRect,"OOH YOU HAVE LOST\nASDJHSD", OR_BG);
				sleep(200);
				printInsidePlace(textRect,"HAHAHA\nASDJHSD", OR_BG);
				_move_cursor_to(29, 2);
				/*printf("%d %d",i, fwin);
				
				sleep(2000);*/
				/*break;

			}
			else if ((getImageY(iBear)==43) && (getImageX(iBear)==106)){
				pthread_cancel(guardThread);
				printPlace(place);
				printInsidePlace(textRect,"CONGRATS! YOU PASSED THIS LEVEL\n", OR_BG);
				sleep(2000);
				fwin=1;
				break;
			}*/

		}
		/*if (fwin==1) break;
		else if (i<1) {
			printInsidePlace(textRect,"DONT WORRY, YOU CAN TRY AGAIN\n", OR_BG);
			sleep(2);
		}
		else if (i==1) {
			printInsidePlace(textRect,"YOU DONT HAVE MORE OPORTUNITIES, LETS GO TO THE NEXT LEVEL\n", OR_BG);
			sleep(2000);
		}
		i++;*/
	}
	pthread_cancel(guardThread);

	/*LEVEL 2*/
	times= 0;
	i=0;
	fwin=0;
	op=0;
	printInsidePlace(textRect,"LEVEL 2\n", OR_BG);
	/*		
	while(i<2){
		nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
		nprint("LEVEL 2\n", OR_BG, OR_BG, 28, 2);
		imageMoveTo(iBear, 11, 11);
		imageMoveTo(im1, 15, 22);
		imageMoveTo(im2, 18, 40);
		imageMoveTo(im3, 14, 15);
		printPlace(place);
		imagePrint(iBear);
		imagePrint(im1);
		imagePrint(im2);
		imagePrint(im3);

		pthread_create(&p1,NULL,thread_function_move_recta1, im1);
		pthread_create(&p2,NULL,thread_function_move_recta2, im2);
		pthread_create(&p3,NULL,thread_function_move_cuadrado1, im3);
		while(times < 200){

			dir = _read_key();
			imageMove(iBear, dir.x, dir.y);
			times++;

			near1 = imagesNear(im1, iBear);
			near2 = imagesNear(im2, iBear);
			near3 = imagesNear(im3, iBear);
			_move_cursor_to(29, 2);
			nprint("                           \n", OR_BG, OR_BG, 29, 2);
			_move_cursor_to(29, 2);
			printf("%d, %d", getImageY(iBear),getImageX(iBear));
			if ((near1==2)||(near2==2)||(near3==2)){
				pthread_cancel(p1);
				pthread_cancel(p2);
				pthread_cancel(p3);
				printPlace(place);
				nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
				nprint("OOH YOU HAVE LOST\n", OR_BG, OR_BG, 28, 2);
				sleep(2000);
				break;

			}
			else if ((getImageY(iBear)==26) && (getImageX(iBear)==52)){
				pthread_cancel(p1);
				pthread_cancel(p2);
				pthread_cancel(p3);
				printPlace(place);
				nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
				nprint("CONGRATS! YOU PASSED THIS LEVEL\n", OR_BG, OR_BG, 28, 2);
				sleep(2000);
				fwin=1;
				break;
			}

		}
		if (fwin==1) break;
		else if (i<1) {
			nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
			nprint("DONT WORRY, YOU CAN TRY AGAIN\n", OR_BG, OR_BG, 28, 2);
			sleep(2000);
			}
		else if (i==1) {
			nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
			nprint("YOU DONT HAVE MORE OPORTUNITIES, LETS GO TO THE NEXT LEVEL\n", OR_BG, OR_BG, 28, 2);
			sleep(2000);
		}
		i++;
	}
	pthread_cancel(p1);
	pthread_cancel(p2);
	pthread_cancel(p3);
	nprint("                                                                         \n", OR_BG, OR_BG, 28, 2);
	nprint("YOU WON THE WHOLE GAME\n", OR_BG, OR_BG, 28, 2);
	sleep(2000);

	LEVEL 2
	times = 0;
	i=0;
	fwin=0;
	op=0;
	while(i<2){
		nprint("                                             \n", OR_BG, CYAN_BG, 28, 2);
		nprint("LEVEL 2\n", OR_BG, CYAN_BG, 28, 2);
		imageMoveTo(iBear, 11, 11);
		imageMoveTo(im1, 15, 22);
		imageMoveTo(im2, 18, 40);
		imageMoveTo(im3, 14, 15);
		printPlace(place);
		imagePrint(iBear);
		imagePrint(im1);
		imagePrint(im2);
		imagePrint(im3);

		pthread_create(&p1,NULL,thread_function_move_recta1, im1);
		pthread_create(&p2,NULL,thread_function_move_recta2, im2);
		pthread_create(&p3,NULL,thread_function_move_cuadrado1, im3);
		while(times < 200){

			dir = _read_key();
			imageMove(iBear, dir.x, dir.y);
			times++;

			near1 = imagesNear(im1, iBear);
			near2 = imagesNear(im2, iBear);
			near3 = imagesNear(im3, iBear);
			_move_cursor_to(29, 2);
			printf("%d, %d", getImageY(iBear),getImageX(iBear));


			if ((near1==2)||(near2==2)||(near3==2)){
				pthread_cancel(p1);
				pthread_cancel(p2);
				pthread_cancel(p3);
				printPlace(place);
				nprint("                                             \n", OR_BG, CYAN_BG, 28, 2);
				nprint("OOH YOU HAVE LOST\n", OR_BG, CYAN_BG, 28, 2);
				sleep(2000);
				break;

			}
			else if ((getImageY(iBear)==26) && (getImageX(iBear)==52)){
				pthread_cancel(p1);
				pthread_cancel(p2);
				pthread_cancel(p3);
				printPlace(place);
				nprint("                                             \n", OR_BG, CYAN_BG, 28, 2);
				nprint("CONGRATS! YOU WON\n", OR_BG, CYAN_BG, 28, 2);
				sleep(2000);
				fwin=1;
				break;
			}

		}
		if (fwin==1) break;
		printPlace(place);
		if (i<1) {
			nprint("                                             \n", OR_BG, CYAN_BG, 28, 2);
			nprint("DONT WORRY, YOU CAN TRY AGAIN\n", OR_BG, CYAN_BG, 28, 2);
			sleep(2000);
			}
		if (i==1) op=1;
		i++;
	}
	*/
	_move_cursor_to(0, 0);
	if (op=1) {
		nprint("                                             \n", OR_BG, CYAN_BG, 28, 2);
		nprint("YOU DONT HAVE MORE OPORTUNITIES, LETS GO TO THE NEXT LEVEL\n", OR_BG, CYAN_BG, 28, 2);
		}
		
	pthread_cancel(guardThread);
	printPlace(place);

	freeImage(iBear);
	freeImage(im1);
	freeImage(im2);
	freeImage(im3);
	freePlace(place);
	_term_reset();
	return;

}

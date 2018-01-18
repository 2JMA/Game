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
#include "mainImpossible.h"

#define NUM_LEVELS 3


void thread_function_move(thread_guard_args* args){
	int i;
	PlaceAvailable result;
	Position p;
	if (args==NULL) return;

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
		if (args->level==1){
			printInsidePlace(args->textRect, "LEVEL 1", OR_BG);
		}else if (args->level==2){
			printInsidePlace(args->textRect, "LEVEL 2", OR_BG);
		}
		args->res=-1;
	}else if (args->res==1){
		if (args->level==1){
			args->res = -1;
			args->numImg = 5;
			args->speed = 50;
			args->level=2;
			printInsidePlace(args->textRect, "LEVEL 2", OR_BG);
		}
		else if (args->level==2){
			args->res = -1;
			args->numImg = 7;
			args->speed = 30;
			args->level=3;
			printInsidePlace(args->textRect, "LEVEL 3", OR_BG);
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

int mainImpossible(Place *map, Place *textRect, Place *infoRect, character *Camok){
	char *mapStr;
	pthread_t guardThread;
	thread_guard_args guardArgs;
	location l1, l2, l3, l4, l5, l6, l7;
	PlaceAvailable result;
	Image* amok = charGetImage(Camok); 

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
	/*Instructions*/
	printInsidePlaceRows(infoRect, "CAN YOU SEE WHAT'S THERE?\nTHOSE ARE NAZI BOOTS!\nTRY TO GET THEM, THEY WILL BE VERY USEFULL IN THE NEAR FUTURE.\nBUT BE CAREFOULL AND DON'T GET COUGHT BY THE GUARDS.\nGOOD LUCK!", OR_FG, 4, 2);


	/*Game itself*/
	Image *im1 = createImage("Images/guard.txt", 6, 10 , OR_BG, CYAN_FG, map);
	Image *im2 = createImage("Images/guard.txt", 16, 40 , OR_BG, CYAN_FG, map);
	Image *im3 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im4 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im5 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im6 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	Image *im7 = createImage("Images/guard.txt", 8, 15 , OR_BG, CYAN_FG, map);
	
	location dir;
	int times = 0;

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
	printInsidePlace(textRect, "CONGRATS, YOU WON", OR_BG);
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
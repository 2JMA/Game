#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include "types.h"
#include "character.h"
#include "objects.h"
#include "image.h"
#include "place.h"
#include "nprint.h"
#include "utils.h"
#include "mainQuestions.h"



int questionGame(Place* map, Place* textRect, Place* infoRect, character* amok){
	character **chars, **chars2, *mChar;
	Image *ai, *bi, *ci, *di;
	char* final, *qu_map, correct, answer;
	PlaceAvailable area;
	int i, f, k=0;

	qu_map=fileToStr("Maps/FinalLabyrinthMap.txt");
	setUpPlace(map, qu_map);

	ai=createImage( "Images/guard.txt", 4, 187, OR_BG, RED_FG, map);
	bi=createImage( "Images/guard.txt", 17, 9, OR_BG, RED_FG, map);
	ci=createImage( "Images/guard.txt", 29, 187, OR_BG, RED_FG, map);
	di=createImage( "Images/guard.txt", 42, 9, OR_BG, RED_FG, map);


	area = AVAILABLE;

	chars=(character**)malloc(sizeof(character*)*6);

	chars[0]=amok;

	chars[1]=iniCharacter("a", ai, 2, NULL, NULL, "When did the war start?\na) 1940\nb) 1939\nc) 1941\n", NULL);
	chars[2]=iniCharacter("b", bi, 3, NULL, NULL, "Which is our favourite philosopher?\na) Hegel\nb) Kant\nc) Heidegger\n", NULL);
	chars[3]=iniCharacter("c", ci, 4, NULL, NULL, "Which is the correct pronunciation of 'pizza'?\na) Pipsa\nb) Picsa\nc) Pitsa\n", NULL);
	chars[4]=iniCharacter("d", di, 5, NULL, NULL, "Which was the first possible answer of the first question?\na) 1941\nb) 1939\nc) 1940\n", NULL);
	chars[5]=NULL;

	chars2=(character**)malloc(sizeof(character*)*5);
	for(i=0; i<5; i++){
		chars2[i]=NULL;
	}

	printPlace(map);
	printPlace(textRect);
	printPlace(infoRect);
	imagePrint(charGetImage(amok));
	imagePrint(ai);
	imagePrint(bi);
	imagePrint(ci);
	imagePrint(di);

	printInsidePlaceRows(infoRect, "objects: nazi boots, nazi hat, nazi uniform", OR_BG, 3, 1);
	printInsidePlaceRows(infoRect, "", OR_BG, 3, 1);
	printInsidePlaceRows(infoRect, "move: arrow keys\n talk: t\n exchange: e", OR_BG, 3, 2);
	printInsidePlaceRows(infoRect, "Talk to people and try to trade your items with them.", OR_BG, 3, 3);

	location dir;
	int times = 0;
	while(times < 1500){

		dir = _read_key();
		printPlace(map);

		for(i=0; i<5; i++){
			if(chars[i]){
				imagePrint(charGetImage(chars[i]));
			}
		}

		if(dir.x==-2 && dir.y==-2){
			printInsidePlace(textRect, "The command doesn't exist or can be used in this moment.\n", placeGetFgColor(textRect));
		}else{
			area=imageMove(charGetImage(amok), dir.x, dir.y);
			imageMove(ai, 0, 0);
			imageMove(bi, 0, 0);
			imageMove(ci, 0, 0);
			imageMove(di, 0, 0);
		}

		if(area==DOOR){
			charFreeCharacters(chars);

			final=fileToStr("Maps/square1.txt");

			ai=createImage( "Images/FinalSceneImg.txt", 2, 2, OR_BG, WHITE_FG, map);

			setUpPlace(map, final);

			printPlace(map);
			imagePrint(ai);
			printPlace(textRect);

			sleep(10000);
			return 0;
		}

		f=0;
		mChar=charGetNextTo(chars);
		for(i=0; i<4; i++){
			if(chars2[i]==mChar){
				f=1;
			}
		}
		if((mChar!=NULL)&&(f==0)){
			chars2[k]=mChar;
			k++;
			charPrintInfo2(mChar, textRect);
			switch(charGetType(mChar)){
				case(2):
					correct='b';
					break;
				case(3):
					correct='c';
					break;
				case(4):
					correct='a';
					break;
				case(5):
					correct='c';
					break;
			}
			do{
				scanf("%c", &answer);
			}while(answer!='a' && answer!='b' && answer!='c');
			if(answer==correct){
				printInsidePlace(textRect, "Correct!! You may continue.\n", placeGetFgColor(textRect));
			}
			else{
				printInsidePlace(textRect, "Incorrect answer. Foolish jew, come with me and take a shower.\n", placeGetFgColor(textRect));
				sleep(5000);
				for(i=1; i<5; i++){
					if(chars[i]) freeImage(charGetImage(chars[i]));
				}

				for(i=0; i<5; i++){
					chars2[i]=NULL;
				}
				k=0;

				moveChar(3, 3, amok);
				ai=createImage( "Images/guard.txt", 4, 187, OR_BG, RED_FG, map);
				bi=createImage( "Images/guard.txt", 17, 9, OR_BG, RED_FG, map);
				ci=createImage( "Images/guard.txt", 29, 187, OR_BG, RED_FG, map);
				di=createImage( "Images/guard.txt", 42, 9, OR_BG, RED_FG, map);

				charSetImage(chars[1], ai);
				charSetImage(chars[2], bi);
				charSetImage(chars[3], ci);
				charSetImage(chars[4], di);

				imagePrint(charGetImage(amok));
				imagePrint(ai);
				imagePrint(bi);
				imagePrint(ci);
				imagePrint(di);
			}
		}

		times++;
	}


	printInsidePlace(textRect, "you have depleted the given 1500 moves.\n", placeGetFgColor(textRect));
	sleep(5000);
	imageClear(ai);
	imageClear(bi);
	imageClear(ci);
	imageClear(di);
	charFreeCharacters(chars);

	return 0;
}

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
  		switch(choice){
			case('t'):
				dir.x=1;
				dir.y=1;
				break;
			case('e'):
				dir.x=2;
				dir.y=2;
				break;
			default:
				dir.x=-2;
				dir.y=-2;
		}
  	}
 	return dir;   
}


int main(){
	character **chars, **chars2, *mChar;
	Image *player, *ai, *bi, *ci, *di;
	Place* map, *text, *info;
	char* final, correct, answer;
	PlaceAvailable area;
	int i, f, k=0;

	_term_init();
	_init_screen();

	map=createPlace(1, 1, "Maps/FinalLabyrinthMap.txt", OR_BG, WHITE_FG, '#', ' ');
	text = createPlace(placeGetLastRow(map)+1, placeGetFirstColumn(map), "Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	info = createPlace(placeGetFirstRow(map), placeGetLastColumn(map)+1, "Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	player=createImage( "Images/amok.txt", 3, 3, OR_BG, CYAN_FG, map);

	ai=createImage( "Images/guard.txt", 4, 187, OR_BG, RED_FG, map);
	bi=createImage( "Images/guard.txt", 17, 9, OR_BG, RED_FG, map);
	ci=createImage( "Images/guard.txt", 29, 187, OR_BG, RED_FG, map);
	di=createImage( "Images/guard.txt", 42, 9, OR_BG, RED_FG, map);


	area = AVAILABLE;

	chars=(character**)malloc(sizeof(character*)*6);

	chars[0]=iniCharacter("amok", player, 1, NULL, NULL, NULL, NULL);

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
	printPlace(text);
	printPlace(info);
	imagePrint(player);
	imagePrint(ai);
	imagePrint(bi);
	imagePrint(ci);
	imagePrint(di);

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
			printInsidePlace(text, "The command doesn exist or can be used in this moment.\n", placeGetFgColor(text));
		}else{
			area=imageMove(player, dir.x, dir.y);
			imageMove(ai, 0, 0);
			imageMove(bi, 0, 0);
			imageMove(ci, 0, 0);
			imageMove(di, 0, 0);
		}

		if(area==DOOR){
			for(i=0; i<5; i++){
				freeCharacter(chars[i]);
			}
			free(chars);

			final=fileToStr("Maps/square1.txt");

			ai=createImage( "Images/FinalSceneImg.txt", 2, 2, OR_BG, WHITE_FG, map);

			setUpPlace(map, final);

			printPlace(map);
			imagePrint(ai);
			printPlace(text);

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
			charPrintInfo2(mChar, text);
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
				printInsidePlace(text, "Correct!! You may continue.\n", placeGetFgColor(text));
			}
			else{
				printInsidePlace(text, "Incorrect answer. Foolish jew, come with me and take a shower.\n", placeGetFgColor(text));
				sleep(5000);
				for(i=0; i<5; i++){
					if(chars[i]) freeImage(charGetImage(chars[i]));
				}

				for(i=0; i<5; i++){
					chars2[i]=NULL;
				}
				k=0;

				player=createImage( "Images/amok.txt", 3, 3, OR_BG, CYAN_FG, map);
				ai=createImage( "Images/guard.txt", 4, 187, OR_BG, RED_FG, map);
				bi=createImage( "Images/guard.txt", 17, 9, OR_BG, RED_FG, map);
				ci=createImage( "Images/guard.txt", 29, 187, OR_BG, RED_FG, map);
				di=createImage( "Images/guard.txt", 42, 9, OR_BG, RED_FG, map);

				charSetImage(chars[0], player);
				charSetImage(chars[1], ai);
				charSetImage(chars[2], bi);
				charSetImage(chars[3], ci);
				charSetImage(chars[4], di);

				imagePrint(player);
				imagePrint(ai);
				imagePrint(bi);
				imagePrint(ci);
				imagePrint(di);
			}
		}

		times++;
	}


	printInsidePlace(text, "you have depleted the given 1500 moves.\n", placeGetFgColor(text));
	sleep(5000);
	imageClear(player);
	imageClear(ai);
	imageClear(bi);
	imageClear(ci);
	imageClear(di);
	freePlace(map);
	freePlace(text);
	freePlace(info);
	for(i=0; i<5; i++){
		freeCharacter(chars[i]);
	}
	free(chars);

	_term_reset();
	return 0;
}
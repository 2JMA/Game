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



int exchange(character** chars, Place *text){
	object *a, *b, **arr;
	character *mChar;
	if(chars==NULL) return -1;
	
	mChar=charGetNextTo(chars);
	if(mChar==NULL){
		printInsidePlace(text, "There is noone near.\n", placeGetFgColor(text));
		return 0;
	}

	a=charGetNeeds(mChar);
	if(a==NULL){
		printInsidePlace(text, "I don't need anything in this moment.\n", placeGetFgColor(text));
		return 0;
	}

	if(objectGetPicked(a)){
				
		arr=charGetObject(mChar);
		b=arr[0];
		
		objectSetPicked(b, 1);
		objectSetPickable(b, 0);


		objectSetPicked(a, 0);
		objectSetPickable(a, 0);

		charAddObject(mChar, a);
		charAddObject(chars[0], b);
		charDropObject(chars[0], a);
		charDropObject(mChar, b);

		charSetNeeds(mChar, NULL);

		if(strcmp(objectGetName(b),"sombrero nazi")==0){
			return 1;
		}

		printInsidePlace(text, "Object received.\n", placeGetFgColor(text));
		
		return 0;
	}
	
	printInsidePlace(text, "You don't have the necessary object.\n", placeGetFgColor(text));
	return 0;
}
int exchangesGame(Place *map, Place *textRect, Place *infoRect, character *amok){
	character **chars;
	Image *ai, *bi, *ci, *di, *obj1, *obj2, *obj3, *obj4, *obj5;
	object **ob_arr1, **ob_arr2, **ob_arr3, **ob_arr4, *foto, *pan, *cigarrillos, *cuchillo, *sombrero_nazi;
	int i, ret=0;

	_term_init();
	_init_screen();

	ai=createImage( "Images/jew.txt", 48, 11, OR_BG, RED_FG, map);
	bi=createImage( "Images/jew.txt", 10, 72, OR_BG, RED_FG, map);
	ci=createImage( "Images/jew.txt", 25, 163, OR_BG, RED_FG, map);
	di=createImage( "Images/guard.txt", 42, 177, OR_BG, RED_FG, map);
	obj1=createImage( "Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj2=createImage( "Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj3=createImage( "Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj4=createImage( "Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj5=createImage( "Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);


	pan=iniObject("pan", 2, 0, 0, obj2, 0, NULL);
	cigarrillos=iniObject("cigarrillos", 3, 0, 0, obj3, 0, NULL);
	cuchillo=iniObject("cuchillo", 4, 0, 0, obj4, 0, NULL);
	sombrero_nazi=iniObject("sombrero nazi", 5, 0, 0, obj5, 0, NULL);

	ob_arr1=(object**)malloc(sizeof(object*)*10);
	ob_arr1[0]=pan;
	for(i=1; i<10; i++){
		ob_arr1[i]=NULL;
	}

	ob_arr2=(object**)malloc(sizeof(object*)*10);
	ob_arr2[0]=cigarrillos;
	for(i=1; i<10; i++){
		ob_arr2[i]=NULL;
	}

	ob_arr3=(object**)malloc(sizeof(object*)*10);
	ob_arr3[0]=cuchillo;
	for(i=1; i<10; i++){
		ob_arr3[i]=NULL;
	}

	ob_arr4=(object**)malloc(sizeof(object*)*10);
	ob_arr4[0]=sombrero_nazi;
	for(i=1; i<10; i++){
		ob_arr4[i]=NULL;
	}
	
	chars=(character**)malloc(sizeof(character*)*6);

	chars[0]=amok;

	chars[1]=iniCharacter("a", ai, 2, ob_arr1, foto, "If you have a photograph, I'll give tou some bread.", NULL);
	chars[2]=iniCharacter("b", bi, 3, ob_arr2, pan, "If you have bread I'll give you a pack of cigarrettes.", NULL);
	chars[3]=iniCharacter("c", ci, 4, ob_arr3, cigarrillos, "If you give me cigarrettes i'll give you a knife.", NULL);
	chars[4]=iniCharacter("d", di, 5, ob_arr4, cuchillo, "If you get me a knife ill give you my nazi hat.", NULL);
	chars[5]=NULL;

	printPlace(map);
	printPlace(textRect);
	printPlace(infoRect);
	imagePrint(charGetImage(amok));
	imagePrint(ai);
	imagePrint(bi);
	imagePrint(ci);
	imagePrint(di);

	location dir;
	int times = 0;
	while(times < 1500){

		dir = _read_key();
		printPlace(map);
		imagePrint(charGetImage(amok));
		imagePrint(ai);
		imagePrint(bi);
		imagePrint(ci);
		imagePrint(di);
		if(dir.x==2 && dir.y==2){
			ret=exchange(chars, textRect);

			if(ret==-1){
				imageClear(ai);
				imageClear(bi);
				imageClear(ci);
				imageClear(di);
				charFreeCharacters(chars);
				freeObject(foto);
				freeObject(cigarrillos);
				freeObject(cuchillo);
				freeObject(pan);
				freeObject(sombrero_nazi);
				free(ob_arr1);
				free(ob_arr2);
				free(ob_arr3);
				free(ob_arr4);
				_term_reset();
				return -1;
			} 

			if(ret==1){
				printInsidePlace(textRect, "YOU WON!!\n", placeGetFgColor(textRect));
				sleep(5000);
				imageClear(charGetImage(amok));
				imageClear(ai);
				imageClear(bi);
				imageClear(ci);
				imageClear(di);
				charFreeCharacters(chars);
				freeObject(foto);
				freeObject(cigarrillos);
				freeObject(cuchillo);
				freeObject(pan);
				freeObject(sombrero_nazi);
				free(ob_arr1);
				free(ob_arr2);
				free(ob_arr3);
				free(ob_arr4);
				_term_reset();
				return 0;
			}
		}else if(dir.x==1 && dir.y==1){
			charPrintInfo(chars, textRect);
		}else if(dir.x==-1 && dir.y==-1){
			imageClear(charGetImage(amok));
			imageClear(ai);
			imageClear(bi);
			imageClear(ci);
			imageClear(di);
			charFreeCharacters(chars);
			freeObject(foto);
			freeObject(cigarrillos);
			freeObject(cuchillo);
			freeObject(pan);
			freeObject(sombrero_nazi);
			free(ob_arr1);
			free(ob_arr2);
			free(ob_arr3);
			free(ob_arr4);
			_term_reset();
			return -1;
		}else if(dir.x==-2 && dir.y==-2){
			printInsidePlace(textRect, "The command does not exist or cannot be used in this moment.\n", placeGetFgColor(textRect));
		}else{
			imageMove(charGetImage(amok), dir.x, dir.y);
			imageMove(ai, 0, 0);
			imageMove(bi, 0, 0);
			imageMove(ci, 0, 0);
			imageMove(di, 0, 0);
		}
		times++;
	}
	printInsidePlace(textRect, "You have depleted the 1500 moves.\n", placeGetFgColor(textRect));
	sleep(5000);
	imageClear(charGetImage(amok));
	imageClear(ai);
	imageClear(bi);
	imageClear(ci);
	imageClear(di);
	charFreeCharacters(chars);
	freeObject(foto);
	freeObject(cigarrillos);
	freeObject(cuchillo);
	freeObject(pan);
	freeObject(sombrero_nazi);
	free(ob_arr1);
	free(ob_arr2);
	free(ob_arr3);
	free(ob_arr4);
	_term_reset();
	return 0;
}

int main(){
	Place* map, *textRect, *infoRect;
	character *amok;
	object* foto, **ob_arr;
	Image* player, *foto_i;
	int res, i;

	map=createPlace(1, 1, "Maps/MapExchanges.txt", OR_BG, WHITE_FG, ',', ' ');
	textRect=createPlace(placeGetLastRow(map)+1, placeGetFirstColumn(map),"Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	infoRect=createPlace(placeGetFirstRow(map), placeGetLastColumn(map)+1,"Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	player=createImage( "Images/amok.txt", 3, 3, OR_BG, CYAN_FG, map);
	foto_i=createImage("Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, WHITE_FG, map);
	foto=iniObject("photograph", 1, 1, 0, foto_i, 0, NULL);
	ob_arr=(object**)malloc(sizeof(object*)*10);
	ob_arr[0]=foto;
	for(i=1; i<10; i++){
		ob_arr[i]=NULL;
	}
	amok=iniCharacter("amok", player, 1, ob_arr, NULL, NULL, NULL);

	res=exchangesGame(map, textRect, infoRect, amok);
	return res;
}
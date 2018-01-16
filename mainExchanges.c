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

#define CYAN_BG 46
#define RED_BG 41
#define MAGENTA_BG 45

#define CYAN_FG 36
#define RED_FG 31
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



int exchange(character** chars){
	object *a, *b;
	character *mChar;
	if(chars==NULL) return -1;
	
	mChar=charGetNextTo(chars);
	if(mChar==NULL){
		nprint("No hay nadie al lado\n", OR_BG, CYAN_BG, 1, 1);
		return 0;
	}

	a=charGetNeeds(mChar);
	if(a==NULL){
		nprint("No tiene nada que intercambiar\n", OR_BG, CYAN_BG, 1, 1);	
		return 0;
	}

	if(objectGetPicked(a)){

				
		b=charGetObject(mChar);
		objectSetPicked(b, 1);
		objectSetPickable(b, 0);
		objectSetLocation(b, X_ELSE, Y_ELSE);

		objectSetPicked(a, 0);
		objectSetPickable(a, 0);
		objectSetLocation(a, X_ELSE, Y_ELSE);
		charSetObject(mChar, a);
		charSetNeeds(mChar, NULL);

		if(strcmp(objectGetName(b),"sombrero nazi")==0){
			return 1;
		}

		nprint("Objeto conseguido.\n", OR_BG, CYAN_BG, 1, 1);
		
		return 0;
	}
	
	nprint("NO tienes el objeto necesario.\n", OR_BG, CYAN_BG, 1, 1);
	return 0;
}

int main(){
	character **chars;
	Image *player, *ai, *bi, *ci, *di, *obj1, *obj2, *obj3, *obj4, *obj5;
	object *foto, *pan, *cigarrillos, *cuchillo, *sombrero_nazi;
	Place* map;
	int ret=0, i;

	_term_init();
	_init_screen();

	map=createPlace(1, 1, "./Maps/map.txt", OR_BG, WHITE_FG, '#', '.');

	player=createImage( "./Images/prueba.txt", 13, 40, OR_BG, CYAN_FG, map);

	ai=createImage( "./Images/1.txt", 3, 3, OR_BG, RED_FG, map);
	bi=createImage( "./Images/2.txt", 3, 50, OR_BG, RED_FG, map);
	ci=createImage( "./Images/3.txt", 22, 3, OR_BG, RED_FG, map);
	di=createImage( "./Images/prueba.txt", 22, 49, OR_BG, RED_FG, map);
	obj1=createImage( "./Images/1.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj2=createImage( "./Images/1.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj3=createImage( "./Images/1.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj4=createImage( "./Images/1.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);
	obj5=createImage( "./Images/1.txt", X_ELSE, Y_ELSE, OR_BG, RED_FG, map);


	foto=iniObject("foto", 1, 1, 0, obj1, 0, NULL);
	pan=iniObject("pan", 2, 0, 0, obj2, 0, NULL);
	cigarrillos=iniObject("cigarrillos", 3, 0, 0, obj3, 0, NULL);
	cuchillo=iniObject("cuchillo", 4, 0, 0, obj4, 0, NULL);
	sombrero_nazi=iniObject("sombrero nazi", 5, 0, 0, obj5, 0, NULL);

	chars=(character**)malloc(sizeof(character*)*6);

	chars[0]=iniCharacter("amok", player, 1, foto, NULL, NULL);

	chars[1]=iniCharacter("a", ai, 2, pan, foto, "Si tienes una foto te doy pan.");
	chars[2]=iniCharacter("b", bi, 3, cigarrillos, pan, "Si tienes pan te doy cigarrillos.");
	chars[3]=iniCharacter("c", ci, 4, cuchillo, cigarrillos, "Si tienes cigarrillos te doy un cuchillo.");
	chars[4]=iniCharacter("d", di, 5, sombrero_nazi, cuchillo, "Si tienes un cuchillo te doy un sombrero nazi.");
	chars[5]=NULL;

	printPlace(map);
	imagePrint(player);
	imagePrint(ai);
	imagePrint(bi);
	imagePrint(ci);
	imagePrint(di);

	location dir;
	int times = 0;
	while(times < 500){

		dir = _read_key();
		printPlace(map);
		imagePrint(player);
		imagePrint(ai);
		imagePrint(bi);
		imagePrint(ci);
		imagePrint(di);
		if(dir.x==2 && dir.y==2){
			ret=exchange(chars);

			if(ret==-1){
				imageClear(player);
				imageClear(ai);
				imageClear(bi);
				imageClear(ci);
				imageClear(di);
				freePlace(map);
				for(i=0; i<5; i++){
					freeCharacter(chars[i]);
				}
				free(chars);
				freeObject(foto);
				freeObject(cigarrillos);
				freeObject(cuchillo);
				freeObject(pan);
				freeObject(sombrero_nazi);
				_term_reset();
				return -1;
			} 

			if(ret==1){
				nprint("HAS GANADO!\n", OR_BG, CYAN_FG, 1, 26);
				sleep(5000);
				imageClear(player);
				imageClear(ai);
				imageClear(bi);
				imageClear(ci);
				imageClear(di);
				freePlace(map);
				for(i=0; i<5; i++){
					freeCharacter(chars[i]);
				}
				free(chars);
				freeObject(foto);
				freeObject(cigarrillos);
				freeObject(cuchillo);
				freeObject(pan);
				freeObject(sombrero_nazi);
				_term_reset();
				return 0;
			}
		}else if(dir.x==1 && dir.y==1){
			charPrintInfo(chars);
		}else if(dir.x==-1 && dir.y==-1){
			imageClear(player);
			imageClear(ai);
			imageClear(bi);
			imageClear(ci);
			imageClear(di);
			freePlace(map);
			for(i=0; i<5; i++){
				freeCharacter(chars[i]);
			}
			free(chars);
			freeObject(foto);
			freeObject(cigarrillos);
			freeObject(cuchillo);
			freeObject(pan);
			freeObject(sombrero_nazi);
			_term_reset();
			return -1;
		}else if(dir.x==-2 && dir.y==-2){
			nprint("No existe ese comando.\n", OR_BG, CYAN_BG, 1, 1);
		}else{
			imageMove(player, dir.x, dir.y);
			imageMove(ai, 0, 0);
			imageMove(bi, 0, 0);
			imageMove(ci, 0, 0);
			imageMove(di, 0, 0);
		}
		times++;
	}
	nprint("Han pasado los 500 movimientos.\n", OR_BG, CYAN_FG, 1, 26);
	sleep(5000);
	imageClear(player);
	imageClear(ai);
	imageClear(bi);
	imageClear(ci);
	imageClear(di);
	freePlace(map);
	for(i=0; i<5; i++){
		freeCharacter(chars[i]);
	}
	free(chars);
	freeObject(foto);
	freeObject(cigarrillos);
	freeObject(cuchillo);
	freeObject(pan);
	freeObject(sombrero_nazi);
	_term_reset();
	return 0;
}
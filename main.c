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
#include "mainExchanges.h"
#include "mainImpossible.h"
#include "mainFinal.h"
#include "mainQuestions.h"



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

int main(){
	 _term_init();
	 _init_screen();

	Place* map, *textRect, *infoRect;
	character *amok;
	object* foto, **ob_arr;
	Image* player, *foto_i, *ini, *ini2, *ini3, *ini4, *ini5, *ini6;
	char buffer;
	int res, i;

	map=createPlace(1, 1, "Maps/cover.txt", OR_BG, OR_FG, '#', ' ');
	printPlace(map);
	scanf("%c", &buffer);

	map=createPlace(1, 1, "Maps/square1.txt", OR_BG, OR_FG, '#', ' ');
	textRect=createPlace(placeGetLastRow(map)+1, placeGetFirstColumn(map),"Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	infoRect=createPlace(placeGetFirstRow(map), placeGetLastColumn(map)+1,"Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	printPlace(map);
	printPlace(textRect);
	printPlace(infoRect);
	ini=createImage("Images/amokfree.txt", 18, 54, OR_BG, OR_FG, map);
	imagePrint(ini);
	printInsidePlace(textRect, "Hi, my name is Amok Ocilite, and I'm jew. I've been in this concentration camp since forever, Tis is a torture, I miss my family, i want to break free.", OR_FG);
	scanf("%c", &buffer);

	ini2=createImage("Images/AgainstGermany.txt", 12, 71, OR_BG, OR_FG, map);
	imagePrint(ini2);
	printInsidePlace(textRect, "This camp is full of nazi soldiers, but I think I've found a way to sneak out. If i manage to get a full nazi soldier suit, i may be able to escape without being noticed. I have to try it! I have nothing to lose!", OR_FG);
	scanf("%c", &buffer);

	ini3=createImage("Images/Chapter1.txt", 13, 65, OR_BG, OR_FG, map);
	imagePrint(ini3);
	printInsidePlace(textRect, "Lets start by bribing the guard.", OR_FG);
	scanf("%c", &buffer);

	map=createPlace(1, 1, "Maps/square1.txt", OR_BG, OR_FG, '#', ' ');
	textRect=createPlace(placeGetLastRow(map)+1, placeGetFirstColumn(map),"Maps/square3.txt", OR_BG, CYAN_FG, '#', ' ');
	infoRect=createPlace(placeGetFirstRow(map), placeGetLastColumn(map)+1,"Maps/square2.txt", OR_BG, RED_FG, '#', ' ');
	player=createImage( "Images/amok.txt", 3, 3, OR_BG, CYAN_FG, map);
	foto_i=createImage("Images/jew.txt", X_ELSE, Y_ELSE, OR_BG, OR_FG, map);
	foto=iniObject("photograph", 1, 1, 0, foto_i, 0, NULL);
	ob_arr=(object**)malloc(sizeof(object*)*10);
	ob_arr[0]=foto;
	for(i=1; i<10; i++){
		ob_arr[i]=NULL;
	}
	amok=iniCharacter("amok", player, 1, ob_arr, NULL, NULL, NULL);

	printInsidePlaceRows(infoRect, "clothes:", OR_BG, 4, 1);
	printInsidePlaceRows(infoRect, "move: arrow keys\n talk: t\n exchange: e\n shoot: space bar", OR_BG, 4, 3);
	res=exchangesGame(map, textRect, infoRect, amok);
	if(res==-1) return -1;
	printInsidePlaceRows(infoRect, "clothes: nazi hat", OR_BG, 4, 1);


	map = createPlace(1, 1, "Maps/square1.txt", OR_BG, OR_FG, '#', ' ');
	printPlace(map);
	ini4=createImage("Images/Chapter2.txt", 13, 65, OR_BG, OR_FG, map);
	imagePrint(ini4);
	printInsidePlace(textRect, "Now it's time to sneak past the guards.", OR_FG);
	scanf("%c", &buffer);


	mainImpossible(map, textRect, infoRect, amok);
	printInsidePlaceRows(infoRect, "clothes: nazi hat, nazi boots", OR_BG, 4, 1);


	map = createPlace(1, 1, "Maps/square1.txt", OR_BG, OR_FG, '#', ' ');
	printPlace(map);
	ini5=createImage("Images/Chapter3.txt", 13, 65, OR_BG, OR_FG, map);
	imagePrint(ini4);
	printInsidePlace(textRect, "Lets kill fuckin' Hitler.", OR_FG);
	scanf("%c", &buffer);

	do{
		res=finalGame(map, textRect, infoRect, amok);
	}while(res!=1);
	printInsidePlaceRows(infoRect, "clothes: nazi hat, nazi boots, nazi uniform", OR_BG, 4, 1);


	map = createPlace(1, 1, "Maps/square1.txt", OR_BG, OR_FG, '#', ' ');
	printPlace(map);
	ini6=createImage("Images/Chapter4.txt", 13, 65, OR_BG, OR_FG, map);
	imagePrint(ini4);
	printInsidePlace(textRect, "Take care on your decisions.", OR_FG);
	scanf("%c", &buffer);

	res=questionGame(map, textRect, infoRect, amok);
	if(res==-1) return -1;

	freeImage(ini);
	freeImage(ini2);
	freeImage(ini3);
	freeImage(ini4);
	freeImage(ini5);
	freeImage(ini6);
	freePlace(map);
	freePlace(textRect);
	freePlace(infoRect);

	exitGame(0);
	return 0;
}
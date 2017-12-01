#include "character.h"
#include "image.h"
#include "impossible.h"
#include <pthread.h>

thread_function_move_recta1(character* mChar){
	if ((mchar==NULL)||(getImageX(charGetImage(mChar))!= 1)) return NULL;
	int i;

	while(1){
		for(i=1; i<=NUM_COLS; i++){
		moveChar(1, 0, mChar);
		sleep(200);
		}

		for(i=NUM_COLS; i>=1; i--){
			moveChar(-1, 0, mChar);
			sleep(200);
		}
}

thread_function_move_recta2(character* mChar){
	if ((mchar==NULL)||(getImageX(charGetImage(mChar))!= NUM_COLS)) return NULL;
	int i;

	while(1){

		for(i=NUM_COLS; i>=1; i--){
			moveChar(-1, 0, mChar);
			sleep(200);
		}
		for(i=1; i<=NUM_COLS; i++){
		moveChar(1, 0, mChar);
		sleep(200);
		}
}

thread_function_move_cuadrado1(character* mChar){
	if ((mchar==NULL)||(getImageY(charGetImage(mChar))!= NUM_ROWS/3||(getImageX(charGetImage(mChar))!= NUM_COLS)) return NULL;
	int i;

	while(1){

		for(i=NUM_COLS; i<=NUM_COLS/3; i--){
			moveChar(-1, 0, mChar);
			sleep(200);
		}
		for(i=NUM_ROWS/3; i>=NUM_ROWS; i++){
		moveChar(0, 1, mChar);
		sleep(200);
		}
		for(i=NUM_COLS/3; i>=NUM_COLS; i++){
		moveChar(1, 0, mChar);
		sleep(200);
		}
		for(i=NUM_ROWS; i<=NUM_ROWS/3; i++){
		moveChar(0, -1, mChar);
		sleep(200);
		}
}

thread_function_move_cuadrado2(character* mChar){
	if ((mchar==NULL)||(getImageY(charGetImageY(mChar))!= (2*NUM_ROWS)/3)||(getImageX(charGetImage(mChar))!= 1)) return NULL;
	int i;

	while(1){

		for(i=1; i>=(2*NUM_COLS)/3; i++){
			moveChar(1, 0, mChar);
			sleep(200);
		}
		for(i=(2*NUM_ROWS)/3; i<=1; i--){
			moveChar(0, -1, mChar);
			sleep(200);
		}
		for(i=(2*NUM_COLS)/3; i<=1; i--){
			moveChar(-1, 0, mChar);
			sleep(200);
		}
		for(i=1; i>=(2*NUM_ROWS)/3; i++){
			moveChar(0, 1, mChar);
			sleep(200);
		}
	}
}	

thread_function_move_cuadrado3(character* mChar){
	if ((mchar==NULL)||(getImageY(charGetImageY(mChar))!= (2*NUM_ROWS)/3)||(getImageX(charGetImage(mChar))!= 1)) return NULL;
	int i;

	while(1){

		for(i=NUM_COLS/6; i>=(5*NUM_COLS)/6; i++){
			moveChar(1, 0, mChar);
			sleep(200);
		}
		for(i=NUM_ROWS/6; i>=(5*NUM_ROWS)/6; i++){
			moveChar(0, -1, mChar);
			sleep(200);
		}
		for(i=(5*NUM_COLS)/6; i<=NUM_COLS/6; i--){
			moveChar(-1, 0, mChar);
			sleep(200);
		}
		for(i=(5*NUM_ROWS)/6; i<=NUM_ROWS/6; i--){
			moveChar(0, 1, mChar);
			sleep(200);
		}
	}
}	

thread_function_move_diagonal1(character* mChar){
	if ((mchar==NULL)||(getImageY(charGetImageY(mChar))!=1)||(getImageX(charGetImage(mChar))!= 1)) return NULL;
	int i;

	while(1){

		for(i=1, j=1; i>=NUM_COLS || j>=NUM_ROWS; i++, j++){
			moveChar(1, 0, mChar);
			sleep(200);
			moveChar(0, 1, mChar);
			sleep(200);
		}
		
	}
}	

thread_function_move_diagonal2(character* mChar){
	if ((mchar==NULL)||(getImageY(charGetImageY(mChar))!= NUM_ROWS)||((getImageX(charGetImage(mChar))!= NUM_COLS))) return NULL;
	int i;

	while(1){

		for(i=NUM_COLS, j=NUM_ROWS; i<=1 || j<=1; i--, j--){
			moveChar(-1, 0, mChar);
			sleep(200);
			moveChar(0, -1, mChar);
			sleep(200);
		}
		
	}
}

/*	FUNCION NIVEL 1 IMPOSSIBLE GAME
	Devuelve -1 si se ha chocado con algun guardia
	Devuelve 0 su ha llegado a la meta.
*/
int impossible_level_1(character* mChar, character* mChar1, character* mChar2){
	pthread_t p1, p2;

	pthread_create(&p1,NULL,thread_function_move_recta1, mChar1);
	pthread_create(&p2,NULL,thread_function_move_recta2, mChar2);

	while(1){
		if (nexto(mChar,mChar1)==0 || nexto(mChar,mChar2)==0){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			return -1;
		}
		if (charGetLoc(mchar)==/*LOCALIZACION META*/){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			return 0;
		}
	}
}

/*	FUNCION NIVEL 2 IMPOSSIBLE GAME
	Devuelve -1 si se ha chocado con algun guardia
	Devuelve 0 su ha llegado a la meta.
*/
int impossible_level_2(character* mChar, character* mChar1, character* mChar2){
	pthread_t p1, p2;

	pthread_create(&p1,NULL,thread_function_move_cuadrado1 ,mChar1);
	pthread_create(&p2,NULL,thread_function_move_cuadrado2 ,mChar2);

	while(1){
		if (nexto(mChar,mChar1)==0 || nexto(mChar,mChar2)==0){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			return -1;
		}
		if (charGetLoc(mchar)==/*LOCALIZACION META*/){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			return 0;
		}
	}
}

/*	FUNCION NIVEL 3 IMPOSSIBLE GAME
	Devuelve -1 si se ha chocado con algun guardia
	Devuelve 0 su ha llegado a la meta.
*/
int impossible_level_3(character* mChar, character* mChar1, character* mChar2, character* mChar3){
	pthread_t p1, p2, p3;

	pthread_create(&p1,NULL,thread_function_move_cuadrado1 ,mChar1);
	pthread_create(&p2,NULL,thread_function_move_cuadrado2 ,mChar2);
	pthread_create(&p3,NULL,thread_function_move_cuadrado3 ,mChar3);

	while(1){
		if (nexto(mChar,mChar1)==0 || nexto(mChar,mChar2)==0 || nexto(mChar,mChar3)==0){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			pthread_join(p3,NULL);
			return -1;
		}
		if (charGetLoc(mchar)==/*LOCALIZACION META*/){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			pthread_join(p3,NULL);
			return 0;
		}
	}
}

/*	FUNCION NIVEL 4 IMPOSSIBLE GAME
	Devuelve -1 si se ha chocado con algun guardia
	Devuelve 0 su ha llegado a la meta.
*/
int impossible_level_4(character* mChar, character* mChar1, character* mChar2, character* mChar3, character* mChar4, character* mChar5){
	pthread_t p1, p2, p3, p4, p5;

	pthread_create(&p1,NULL,thread_function_move_cuadrado1 ,mChar1);
	pthread_create(&p2,NULL,thread_function_move_cuadrado2 ,mChar2);
	pthread_create(&p3,NULL,thread_function_move_cuadrado3 ,mChar3);
	pthread_create(&p4,NULL,thread_function_move_diagonal1 ,mChar4);
	pthread_create(&p5,NULL,thread_function_move_diagonal2 ,mChar5);

	while(1){
		if (nexto(mChar,mChar1)==0 || nexto(mChar,mChar2)==0 || nexto(mChar,mChar3)==0 || nexto(mChar,mChar4)==0 || nexto(mChar,mChar5)==0){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			pthread_join(p3,NULL);
			pthread_join(p4,NULL);
			pthread_join(p5,NULL);
			return -1;
		}
		if (charGetLoc(mchar)==/*LOCALIZACION META*/){
			pthread_join(p1,NULL);
			pthread_join(p2,NULL);
			pthread_join(p3,NULL);
			pthread_join(p4,NULL);
			pthread_join(p5,NULL);
			return 0;
		}
	}
}





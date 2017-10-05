#include "rectangle.h"
#include <stdlib.h>
#include <stdio.h>

#define CYAN_FG 36
#define RED_FG 31
#define CYAN_BG 46
#define RED_BG 41



void main(){
	printf("%s\n", "Prueba");

	sc_rectangle* test;
	test = win_new(1,1, 10, 15, CYAN_BG, RED_FG);
	printf("%s\n", "Prueba");
}
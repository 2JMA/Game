#include "nprint.h"
#include <stdlib.h>
#include <stdio.h>


#define MAX_LINE 301
#define CYAN_FG 36
#define RED_FG 31
#define CYAN_BG 46
#define RED_BG 41
#define WHITE_BG 47
#define WHITE_FG 37
#define BLACK_BG 40
#define BLACK_FG 30



void main(){
	char line[MAX_LINE];
	char texto[] = "Eran uno dos y tres los pequeños mosqueteros, y el pequeño dartacan siempre iba con ellos.\n A ver\n";
	FILE *f = NULL;
	f = fopen("draw.txt", "r");

	if(f == NULL){
		printf("Error al abrir el archivo");
		return;
	}

	initScreen();

	int i;
	for(i=0; i<171; i++){
		fgets(line, MAX_LINE, f);
		nprint(line, i, 0, BLACK_BG, CYAN_FG);
	}
	

	/*printf("\e[1;1H\e[2J");
	win_cls(test);
	win_write_line_at(test, 5, 5, texto);*/
	fclose(f);
}
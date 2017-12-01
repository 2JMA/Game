#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

char *fileToStr(char *file){
	char *string1, *string;
	FILE *f;

	string1 = (char*) malloc(sizeof(string1[0])*50);
	string = (char*) malloc(sizeof(string[0])*1000);

	if(string1==NULL || string==NULL){
		free(string1);
		free(string);
		return NULL;
	}

	f = fopen(file, "r");
	fgets(string1,50,f);
	strcpy(string,string1);

	while(!feof(f)){
		fgets(string1,50,f);
		strcat(string,string1);
	}
	fclose(f);
	free(string1);
	return string;	

}
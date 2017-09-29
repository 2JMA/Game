#include "character.h"
#include <string.h>

typedef struct _Character{
	char* name;
	char* image;
	location *loc;
}character;

character* initCharacter(char *name, char *image, location *loc){
	if(name == NULL || image == NULL || loc == NULL) return NULL;

	character* mChar = (character*)malloc(sizeof(character));
	if (mChar==NULL)
		return NULL;
	char* mChar->name=(char*)malloc((strlen(name)+1)*sizeof(char));
	if (mChar->name==NULL){
		free(mChar);
		return NULL
	}
	char* mChar->image = (char*) malloc((strlen(image)+1)*sizeof(char));
	if (mChar->image==NULL){
		free(mChar->name);
		free(mChar);
		return NULL;
	}

	location* mChar->loc=(location*)mallooc(sizeof(loc));
	if (mChar->loc==NULL){
		free(mChar->name);
		free(mChar->image);
		free(mChar);
		return NULL;
	}
	return mChar;
}

Status freeCharacter(character *mChar){
	if(mChar == NULL) return ERROR;

	if(mChar->name != NULL)
		free(mChar->name);

	if(mChar->image != NULL)
		free(mChar->image);

	if(mChar->loc != NULL)
		free(mChar->loc);
	free(mChar);

	return OK;
}

Status changeCharLocation(character *mChar, location *loc){
	if(mChar == NULL || loc == NULL) return ERROR;

	mChar->loc->lat = loc->lat;
	mChar->loc->lng = loc->lng;
}

Status moveChar(int x, int y){
	if(mChar == NULL || loc == NULL) return ERROR;

	mChar->loc->lat += x;
	mChar->loc->lng += y ;
}
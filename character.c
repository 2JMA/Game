#include "character.h"
#include <string.h>
#include <stdlib.h>

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
	mChar->name =(char*)malloc((strlen(name)+1)*sizeof(char));
	if (mChar->name==NULL){
		free(mChar);
		return NULL;
	}
	mChar->image = (char*) malloc((strlen(image)+1)*sizeof(char));
	if (mChar->image==NULL){
		free(mChar->name);
		free(mChar);
		return NULL;
	}

	mChar->loc=(location*)malloc(sizeof(loc));
	if (mChar->loc==NULL){
		free(mChar->name);
		free(mChar->image);
		free(mChar);
		return NULL;
	}
	strcpy(mChar->name, name);
	strcpy(mChar->image, image);
	mChar->loc->lat = loc->lat;
	mChar->loc->lng = loc->lng;
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

Status moveChar(location* loc, character *mChar){
	if(mChar == NULL || loc == NULL || mChar->loc == NULL) return ERROR;

	mChar->loc->lat += loc->lat;
	mChar->loc->lng += loc->lng;
}
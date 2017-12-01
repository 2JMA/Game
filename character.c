#include "character.h"
#include <string.h>
#include <stdlib.h>

struct _Character{
	int type;
	char* name;
	char* image;
	int x;
	int y;
	object* obj;
	/*they have only 1 object so it's object* and not object** */
	/*if it has no object this field is NULL*/
	object* needs;
	/*they have only 1 object needed so it's object* and not object** */
	/*if it has no object needed this field is NULL*/
	char* info;	
};

character* iniCharacter(char *name, char *image, int x, int y, int type, object* obj, object* needs, char* info){
	if(name == NULL || image == NULL) return NULL;

	character* mChar = (character*)malloc(sizeof(character));
	if (mChar==NULL)
		return NULL;
	mChar->name =strdup(name);
	if (mChar->name==NULL){
		free(mChar);
		return NULL;
	}
	mChar->image = strdup(image);
	if (mChar->image==NULL){
		free(mChar->name);
		free(mChar);
		return NULL;
	}
	if(info==NULL){
		info = strdup("cero");
	}
	mChar->info=strdup(info);
	mChar->needs = needs;
	mChar->obj = obj;
	mChar->type = type;
	mChar->y = y;
	mChar->x = x;
	
	return mChar;
}

Status freeCharacter(character *mChar){
	if(mChar == NULL) return ERROR;

	if(mChar->name != NULL)
		free(mChar->name);

	if(mChar->image != NULL)
		free(mChar->image);

	free(mChar);

	return OK;
}

Status changeCharLocation(character *mChar, int x, int y){
	if(mChar == NULL) return ERROR;
	imageMoveTo(mChar->image, x, y);

	return OK;
}

Status moveChar(int x, int y, character *mChar){
	if(mChar == NULL) return ERROR;
	imageMove(mChar->image, x, y);

	return OK;
}


Status charChangeName(character* mChar, char* name){
	if((mChar==NULL)||(name==NULL)) return ERROR;

	if(mChar->name) free(mChar->name);
	mChar->name=strdup(name);
	if(mChar->name==NULL) return ERROR;
	
	return OK;
}

char* charGetName(character* mChar){
	if((mChar==NULL)||(mChar->name==NULL)) return NULL;

	return strdup(mChar->name);
}

int charGetType(character* mChar){
	if(mChar==NULL) return -1;
	return mChar->type;
}

object* charGetObject(character* mChar){
	if(mChar==NULL) return NULL;
	return mChar->obj;
}

object* charGetNeeds(character* mChar){
	if(mChar==NULL) return NULL;
	return mChar->needs;
}

char* charGetInfo(character* mChar){
	if(mChar==NULL) return NULL;
	return mChar->info;
}

int* charGetLoc(character* mChar){
	int loc[2];
	loc[0]=mChar->x;
	loc[1]=mChar->y;
	return loc;
}

character* charGetNextTo(character** chars){
	if(chars==NULL) return NULL;

	int i;
	for(i=1; chars[i]; i++){
		if(NextTo(chars[0], chars[i])) return chars[i];
	}
	return NULL;
}

Status charPrintInfo(character** chars){
	if(chars==NULL) return ERROR;
	int i;
	character* mChar;
	char* s;
	mChar=charGetNextTo(chars);
	if(mChar==NULL){
		IMPRIMIR("No hay nadie al lado");
		return OK;
	}
	switch(charactergetType(mChar)){
		case():
			s=charGetInfo(mChar);
			if(strcmp(s,"cero")==0){
				IMPRIMIR("No tiene nada que decir");
				return OK;
			}
			IMPRIMIR("%s", s);
			return OK;
	}		
	return ERROR;
}



character** getCharactersFromFile(char* file, object** objects){
	if(file==NULL) return NULL;

	character** c;
	object* obj, *need;
	char *line, *line1, *name, *image, *token, *info = NULL;
	int type, i, j, num_c, x, y, object=-1, needs=-1;
	FILE* pf;
	line=(char*)malloc(sizeof(char)*100);
	if(line==NULL) return NULL;
	line1=(char*)malloc(sizeof(char)*10);
	if(line1==NULL){
		free(line);
		return NULL;
	}
	pf=fopen(file, "r");
	fgets(line1, 10 , pf);
	num_c=atoi(line1);
	free(line1);
	c=(character**)malloc(sizeof(character*)*(num_c+1));
	if(c==NULL){
		free(line);
		return NULL;
	}
	for(j=0; j<num_c; j++){
		fgets(line, 200, pf);
		token=strtok(line,",");
		for(i=0; token; i++){
			switch(i){
				case(0):
					name=strdup(token);
					if(name==NULL){
						free(line);
						for(i=0;i<j;i++){
							freeCharacter(c[i]);
						}
						free(c);
						fclose(pf);
						return NULL;
					}
					break;
				case(1):
					type=atoi(token);
					break;
				case(2):
					image=strdup(token);
					if(image==NULL){
						free(name);
						free(line);
						for(i=0;i<j;i++){
							freeCharacter(c[i]);
						}
						free(c);
						fclose(pf);
						return NULL;
					}
					break;
				case(3):
					x=atoi(token);
					break;
				case(4):
					y=atoi(token);
					break;
				case(5):
					object=atoi(token);
					break;
				case(6):
					needs=atoi(token);
					break;
				case(7):
					info=strdup(token);
					if(info==NULL){
						free(image);
						free(name);
						free(line);
						for(i=0;i<j;i++){
							freeCharacter(c[i]);
						}
						free(c);
						fclose(pf);
						return NULL;
					}		
			}
			token=strtok(NULL,",");
		}

		if(object==-1) obj=NULL;
		else{
			obj=objectSearch(object, objects);
			object=-1;
		}
		
		if(needs==-1) need=NULL;
		else{
			need=objectSearch(needs, objects);
			needs=-1;
		}
		c[j]=iniCharacter(name, image, x, y, type, obj, need, info);
		if(c[j]==NULL){
			free(info);
			free(name);
			free(image);
			free(line);
			for(i=0;i<j;i++){
				freeCharacter(c[i]);
			}
			free(c);
			fclose(pf);
			return NULL;
		}
		free(info);
		free(name);
		free(image);
	}
	c[num_c]=NULL;
	free(line);
	fclose(pf);
	return c;
}

	


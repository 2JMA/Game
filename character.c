#include "character.h"
#include "image.h"
#include "nprint.h"
#include <string.h>
#include <stdlib.h>
#define CYAN_BG 46

struct _Character{
	int type;
	char* name;
	Image* image;
	object** obj;
	/*they can have up to 10 objects it ends with NULL*/
	/*if it has no object this field is NULL*/
	object* needs;
	/*they have only 1 object needed so it's object* and not object** */
	/*if it has no object needed this field is NULL*/
	char* info;	
	resource** res;
	/*it ends with NULL*/
};

character* iniCharacter(char *name, Image *image, int type, object** obj, object* needs, char* info, resource** res){
	character* mChar;
	if(name == NULL || image == NULL) return NULL;

	mChar = (character*)malloc(sizeof(character));
	if (mChar==NULL)
		return NULL;
	mChar->name =strdup(name);
	if (mChar->name==NULL){
		free(mChar);
		return NULL;
	}
	mChar->image = image;
	if (mChar->image==NULL){
		free(mChar->name);
		free(mChar);
		return NULL;
	}
	if(info==NULL){
		mChar->info = strdup("cero");
	}else{
		mChar->info=strdup(info);
		if(mChar->info==NULL){
			freeImage(mChar->image);
			free(mChar->name);
			free(mChar);
			return NULL;
		}
	}
	mChar->needs = needs;
	mChar->obj = obj;
	mChar->type = type;
	mChar->res = res;
	return mChar;
}

Status freeCharacter(character *mChar){
	if(mChar == NULL) return ERROR;

	if(mChar->name != NULL)
		free(mChar->name);

	if(mChar->info != NULL)
		free(mChar->info);

	if(mChar->image != NULL)
		freeImage(mChar->image);

	free(mChar);

	return OK;
}

Status changeCharLocation(character *mChar, int x, int y){
	if(mChar == NULL) return ERROR;
	imageMoveTo(mChar->image, x, y);

	return OK;
}

PlaceAvailable moveChar(int x, int y, character *mChar){
	if(mChar == NULL) return ERROR;
	return imageMove(mChar->image, x, y);
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

Status charSetRes(character* mChar, resource** res){
	if(mChar==NULL) return ERROR;
	mChar->res=res;
	return OK;
}

resource** charGetRes(character* mChar){
	if(mChar==NULL) return NULL;
	return mChar->res;
}

Status charSetObject(character* mChar, object** obj){
	if(mChar==NULL) return ERROR;
	mChar->obj=obj;
	return OK;
}

Status charAddObject(character* mChar, object* obj){
	if(mChar==NULL||obj==NULL) return ERROR;

	object** array;
	int i;

	array=charGetObject(mChar);
	for(i=0; i<10; i++){
		if(array[i]==NULL){
			array[i]=obj;
			array[i+1]=NULL;
			break;
		}
	}
	return OK;
}

Status charDropObject(character* mChar, object* obj){
	if(mChar==NULL||obj==NULL) return ERROR;

	object** array;
	int i, f=0;

	array=charGetObject(mChar);
	for(i=0; array[i]!=NULL; i++){
		if(array[i]==obj){
			f=1;
		}
		if(f==1){
			array[i]=array[i+1];
		}
	}
	return OK;
}

Status charSetNeeds(character* mChar, object* obj){
	if(mChar==NULL) return ERROR;
	mChar->needs=obj;
	return OK;
}

object** charGetObject(character* mChar){
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


int charGetX(character* mChar){
	if((mChar==NULL)||(mChar->image==NULL)) return X_ELSE;
	return getImageX(mChar->image);
}

int charGetY(character* mChar){
	if((mChar==NULL)||(mChar->image==NULL)) return Y_ELSE;
	return getImageY(mChar->image);
}

Image* charGetImage(character* mChar){
	if(mChar==NULL) return NULL;
	return mChar->image;
}

Status charSetImage(character* mChar, Image* image){
	if(mChar==NULL||image==NULL) return ERROR;
	mChar->image=image;
	return OK;
}

character* charGetNextTo(character** chars){
	if(chars==NULL) return NULL;

	int i;
	Position result;
	for(i=1; chars[i]!=NULL; i++){
		result=imagesNear(chars[0]->image, chars[i]->image);
		if(result==NEAR||result==INSIDE) return chars[i];
	}
	return NULL;
}

Status charPrintInfo(character** chars, Place* place){
	character* mChar;
	char* s;
	if(chars==NULL) return ERROR;
	
	mChar=charGetNextTo(chars);
	if(mChar==NULL){
		printInsidePlace(place, "There is noone near to talk to..\n", placeGetFgColor(place));
		return OK;
	}
	s=charGetInfo(mChar);
	if(s==NULL) return ERROR;
	if(strcmp(s,"cero")==0){
		printInsidePlace(place, "I have nothing to say.\n",placeGetFgColor(place));
		return OK;
	}
	printInsidePlace(place, s, placeGetFgColor(place));
	return OK;
}

Status charPrintInfo2(character* mChar, Place* place){
	char* s;
	if(mChar==NULL) return ERROR;

	s=charGetInfo(mChar);
	if(s==NULL) return ERROR;
	if(strcmp(s,"cero")==0){
		printInsidePlace(place, "I have nothing to say.\n",placeGetFgColor(place));
		return OK;
	}
	printInsidePlace(place, s, placeGetFgColor(place));
	return OK;
}

Status charFreeCharacters(character** chars){
	if(chars==NULL) return ERROR;
	int i;
	for(i=1; chars[i]; i++){
		freeCharacter(chars[i]);
	}
	return OK;
}



/*character** getCharactersFromFile(char* file, object** objects){
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
*/
	


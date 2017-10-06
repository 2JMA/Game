#include <stdlib.h>
#include <string.h>
#include "objects.h"


struct _Object{
	int type;
	int weight;
	Bool picked;
	Bool pickable;
	char* image;
	location *loc;
	resource **resources;
};

object *IniObject(int type, int weight, Bool picked, Bool pickable, char * image, location *loc, resource** resources){

	object *obj;

	if (type==-1 || weight==-1 || image==NULL || loc==NULL || resources==NULL) return NULL;

	obj= (object*) malloc (sizeof(object));
	if (obj==NULL) return NULL;
	obj->type = type;
	obj->weight = weight;
	obj->picked = picked;
	obj->pickable = pickable;
	obj->image = (char*)malloc(3*sizeof(char));
	if(obj->image == NULL){
		free(obj);
		return NULL;
	}
	obj->loc= (location*)malloc(sizeof(location));
	if(obj->loc == NULL){
		free(obj->image);
		free(obj);
		return NULL;
	}
	obj->resources=(resource**)malloc(sizeof(resource*));
	if(obj->resources == NULL){
		free(obj->loc);
		free(obj->image);
		free(obj);
		return NULL;
	}

	strcpy(obj->image, image);
	obj->loc->lat = loc->lat;
	obj->loc->lng = loc->lng;

	return obj;
}

void freeObject (object *obj){

	if(obj==NULL) return;

	if(obj->image != NULL) free(obj->image);
	if(obj->loc != NULL)free(obj->loc);
	if(obj->resources != NULL)free(obj->resources); 
	free(obj);
	return;

}

Status moveObject(object *obj, location *loc){

	if(obj==NULL || loc==NULL) return ERROR ;

	obj->loc=loc;

	return OK;

}


Status PickObject (object *obj){

	if(obj==NULL) return ERROR;

	if (obj->picked == FALSE) obj->picked = TRUE;
	else if (obj->picked == TRUE) obj->picked = FALSE;
	

	return OK; 
}








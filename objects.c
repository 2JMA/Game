#include <stdlib>
#include "objects.h"
#include "resources.h"

struct _Object{
	int type;
	int weight;
	BOOL picked;
	BOOL pickable;
	char* image;
	location *loc;
	resource *resources;

}object;

Location *Inilocation(int lat, int lng){

	Location *loc;

	loc= (Location*)malloc(sizeof(Location));
	if (loc==NULL) return NULL;

	loc->lat= lat;
	loc->lng= lng;

	return loc;
}


Object *IniObject(int type, int weight, BOOL picked, BOOL pickable, char * image, Location *loc, resources** resources){

	Object *obj;

	if (type==-1 || weight==-1 || image==NULL || loc==NULL || resources==NULL) return NULL;

	obj= (obj*) malloc (sizeof(object));
	if (obj==NULL) return NULL;
	obj->type = type;
	obj->weight = weight;
	obj->picked = picked;
	obj->pickable = pickable;
	obj->image = image;
	obj->loc= loc;
	obj->resources=resources;

	return obj;

}

void freeObject (Object *obj){

	if(obj==NULL) return;

	free(obj->image);
	free(obj->loc);
	free(obj->resources); //AHORA SE MIRA SHABALEH CON LA CALMA VIVA ESPAÑA VOLEM VOTAR VIVA AMOK OCILITE
	free(obj);
	return;

}

Status moveObject(Object *obj, Location *loc){

	if(obj==NULL || loc==NULL) return ERROR ;

	obj->loc=loc;

	return OK;

}


Status PickObject (Object *obj){

	if(obj==NULL) return ERROR;

	if (obj->picked == FALSE) obj->picked = TRUE;
	else if (obj->picked == TRUE) obj->picked = FALSE;
	

	return OK; 
}








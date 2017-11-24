# ifndef OBJECTS_H
# define  OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "resources.h"

typedef struct _Object object;

/*memory*/

object *iniObject(char* name, int type, Bool picked, Bool pickable, char * image, int lat, int lng, int num_res, int* resources);

void freeObject (object *obj);

/*general*/

Status moveObject(object *obj, int lat, int lng);

Status pickObject (object *obj);

/*setters*/

Status objectSetType(object* obj, int type);

Status objectSetName(object* obj, char* name);

Status objectSetPicked(object* obj, Bool picked);

Status objectSetPickable(object* obj, Bool pickable);

Status objectSetImage(object* obj, char* image);

Status objectSetLocation(object* obj, int lat, int lng);

Status objectSetResources(object* obj, int* res, int num_res);

/*getters*/

int objectGetType(object* obj);

char* objectGetName(object* obj);

Bool objectGetPicked(object* obj);

Bool objectGetPickable(object* obj);

char* objectGetImage(object* obj);

int* objectGetLocation(object* obj);

int* objectGetResources(object* obj);

int objectGetnum_res(object* obj);

object* objectSearch(int type, object** obj);

/*read from file*/

object** getObjectsFromFile(char* file);

# endif

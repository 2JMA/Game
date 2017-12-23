# ifndef OBJECTS_H
# define  OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "image.h"

typedef struct _Object object;

/*memory*/

object *iniObject(char* name, int type, Bool picked, Bool pickable, Image* image, int num_res, int* resources);

void freeObject (object *obj);

/*general*/

Status pickObject (object *obj);

/*setters*/

Status objectSetType(object* obj, int type);

Status objectSetName(object* obj, char* name);

Status objectSetPicked(object* obj, Bool picked);

Status objectSetPickable(object* obj, Bool pickable);

Status objectSetImage(object* obj, Image* image);

Status objectSetLocation(object* obj, int lat, int lng);

Status objectSetResources(object* obj, int* res, int num_res);

/*getters*/

int objectGetType(object* obj);

char* objectGetName(object* obj);

Bool objectGetPicked(object* obj);

Bool objectGetPickable(object* obj);

Image* objectGetImage(object* obj);

int objectGetX(object* obj);

int objectGetY(object* obj);

int* objectGetResources(object* obj);

int objectGetnum_res(object* obj);

object* objectSearch(int type, object** obj);

/*read from file*/

/*object** getObjectsFromFile(char* file);*/

# endif

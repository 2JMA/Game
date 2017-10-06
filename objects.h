# ifndef OBJECTS_H
# define  OBJECTS_H

#include "types.h"
#include "resources.h"

typedef struct _Object object;

object *IniObject(int type, int weight, Bool picked, Bool pickable, char * image, location *loc, resource** resources);

void freeObject (object *obj);

Status moveObject(object *obj, location *loc);

Status PickObject (object *obj);

# endif

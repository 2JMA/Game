# ifndef RESOURCES_H
# define  RESOURCES_H

#include "types.h"
#include <stdlib.h>

typedef struct _Resource resource;

/*memory*/

resource* initResource(char* name, int type, int max, int min, int value);

void freeResource(resource* r);

/*changers/setters*/

Status changeName(resource* r, char* name);

Status changeMax(resource* r, int max);

Status changeMin(resource* r, int min);

Status changeValue(resource* r, int value);

Status varyValue(resource* r, int value);

/*getters*/

char* resourceGetName(resource* r);

int getType(resource *r);

int getMin(resource *r);

int getMax(resource *r);

int getValue(resource *r);

/*read from file*/

resource** getResourcesFromFile(char* file);

# endif
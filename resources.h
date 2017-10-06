# ifndef RESOURCES_H
# define  RESOURCES_H

#include "types.h"
#include <stdlib.h>

typedef struct _Resource resource;

resource* initResource(int type, int max, int min, int value);

Status freeResource(resource* r);

Status changeMax(resource* r, int max);

Status changeMin(resource* r, int min);

Status changeValue(resource* r, int value);

Status varyValue(resource* r, int value);

int getType(resource *r);

int getMin(resource *r);

int getMax(resource *r);

int getValue(resource *r);

# endif
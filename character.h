# ifndef CHARACTER_H
# define  CHARACTER_H

#include "types.h"

typedef struct _Character character;

character* initCharacter(char *name, char *image, location *loc);

Status freeCharacter(character *mChar);

Status changeCharLocation(character *mChar, location *loc);


# endif
# ifndef CHARACTER_H
# define  CHARACTER_H

#include "types.h"
#include "objects.h"
#include "image.h"

typedef struct _Character character;

character* iniCharacter(char *name, Image *image, int type, object* obj, object* needs, char* info);

Status freeCharacter(character *mChar);

Status changeCharLocation(character *mChar, int lat, int lng);

Status moveChar(int lat, int lng, character *mChar);

Status charChangeName(character* mChar, char* name);

char* charGetName(character* mChar);

int charGetType(character* mChar);

Status charSetObject(character* mChar, object* obj);

Status charSetNeeds(character* mChar, object* obj);

object* charGetObject(character* mChar);

object* charGetNeeds(character* mChar);

char* charGetInfo(character* mChar);

int charGetX(character* mChar);

int charGetY(character* mChar);

character* charGetNextTo(character** chars);

Status charPrintInfo(character** chars, Place* place);

/*character** getCharactersFromFile(char* file, object** objects);*/

# endif
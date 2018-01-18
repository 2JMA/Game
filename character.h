# ifndef CHARACTER_H
# define  CHARACTER_H

#include "types.h"
#include "objects.h"
#include "image.h"
#include "resources.h"
#include "place.h"

typedef struct _Character character;

character* iniCharacter(char *name, Image *image, int type, object** obj, object* needs, char* info, resource** res);

Status freeCharacter(character *mChar);

Status changeCharLocation(character *mChar, int lat, int lng);

PlaceAvailable moveChar(int lat, int lng, character *mChar);

Status charChangeName(character* mChar, char* name);

char* charGetName(character* mChar);

int charGetType(character* mChar);

Status charSetObject(character* mChar, object** obj);

Status charAddObject(character* mChar, object* obj);

Status charDropObject(character* mChar, object* obj);

Status charSetNeeds(character* mChar, object* obj);

Status charSetRes(character* mChar, resource** res);

resource** charGetRes(character* mChar);

object** charGetObject(character* mChar);

object* charGetNeeds(character* mChar);

char* charGetInfo(character* mChar);

int charGetX(character* mChar);

int charGetY(character* mChar);

Status charSetImage(character* mChar, Image* image);

Image* charGetImage(character* mChar);

character* charGetNextTo(character** chars);

Status charPrintInfo(character** chars, Place* place);

Status charPrintInfo2(character* mChar, Place* place);

Status charFreeCharacters(character** chars);

/*character** getCharactersFromFile(char* file, object** objects);*/

# endif
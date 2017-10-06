#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include "nprint.h"

#define OR_BG 40
#define OR_FG 37

/*
  Prints on the screen the style for writing on this window,
  foreground colour, background colour, and font style.
 */
static void _prepare_font(int bgColor, int fgColor) {
    printf("%c[%d;%dm", 27, bgColor, fgColor);
    return;
}
  
/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 */
void _move_to(int r, int c) {
    if (r < 0) r = 0;
    if (c < 0) c = 0;
    printf("%c[%d;%dH", 27, r, c);
    return;
}

void cleanScreen(){
  printf("\e[1;1H\e[2J");
}

void initScreen(){
    _prepare_font(OR_BG, OR_FG);
    cleanScreen();
}
  
int nprint(char *str, int r, int c, int bgColor, int fgColor){
    int n;
    _prepare_font(bgColor, fgColor);
    _move_to(r, c);
    n = printf("%s", str);
    fflush(stdout);
    _prepare_font(OR_BG, OR_FG);
    return n;
}
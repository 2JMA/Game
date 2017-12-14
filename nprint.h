#ifndef NPRINT_H
#define NPRINT_H
 
#include <stdio.h>
#include <stdlib.h>    

/*Default bg and fg colors*/
#define OR_BG 40
#define OR_FG 37

int nprint(char *text, int bgColor, int fgColor, int r, int c);
void _prepare_font(int bgColor, int fgColor);
void _move_cursor_to(int r, int c);
void _clean_screen();
void _init_screen();

#endif  /* NPRINT_H */


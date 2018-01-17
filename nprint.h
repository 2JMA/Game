#ifndef NPRINT_H
#define NPRINT_H
 
#include <stdio.h>
#include <stdlib.h>    

/*Default bg and fg colors*/

#define OR_BG 40
#define OR_FG 37

#define MAX_LINE 301

#define CYAN_FG 36
#define RED_FG 31
#define MAGENTA_FG 35
#define YELLOW_FG 33
#define WHITE_FG 37
#define BLACK_FG 30
#define GREEN_FG 32
#define BLUE_FG 34

#define CYAN_BRIGHT_FG 96
#define RED_BRIGHT_FG 91
#define MAGENTA_BRIGHT_FG 95
#define YELLOW_BRIGHT_FG 93
#define WHITE_BRIGHT_FG 97
#define BLACK_BRIGHT_FG 90
#define GREEN_BRIGHT_FG 92
#define BLUE_BRIGHT_FG 94


#define CYAN_BG 46
#define RED_BG 41
#define MAGENTA_BG 45
#define YELLOW_BG 43
#define WHITE_BG 47
#define BLACK_BG 40
#define GREEN_BG 42
#define BLUE_BG 44

#define CYAN_BRIGHT_BG 106
#define RED_BRIGHT_BG 101
#define MAGENTA_BRIGHT_BG 105
#define YELLOW_BRIGHT_BG 103
#define WHITE_BRIGHT_BG 107
#define BLACK_BRIGHT_BG 100
#define GREEN_BRIGHT_BG 102
#define BLUE_BRIGHT_BG 104


int nprint(char *text, int bgColor, int fgColor, int r, int c);
void _prepare_font(int bgColor, int fgColor);
void _move_cursor_to(int r, int c);
void _clean_screen();
void _init_screen();

#endif  /* NPRINT_H */


#include "time.h"
#include <mc9s12dp512.h>


void usleep(int wait) {
 int i;
 
 for(i = 0; i < wait; i++) {
 }
  
}

extern int Count;
typedef unsigned long time_t;

time_t clock(void) {
 
 return (Count << 16) + TCNT; 
  
}

time_t time(void * in) {
  return TCNT;
}

int lcd_line, lcd_col;
int cursor_line, cursor_col;
int headline, tailline;

#define TERM_LINES 50
#define TERM_COLS 21
#define SCREEN_LINES 8

char scrollback[TERM_LINES][TERM_COLS + 1];

void term_init(void) {
  int i, j;
 
  LCD_Init();
  LCD_Clear(0);
  
  lcd_line = lcd_col = 1;
  cursor_line = cursor_col = 0;
  headline = 0;
  tailline = SCREEN_LINES - 1; 
 
  for (i=0; i < TERM_LINES; i++) 
    for (j=0; j < TERM_COLS + 1; j++) 
      scrollback[i][j] = 0;
  
}

void print_line(char * line){
  int i;
  
  for(i=0; i < TERM_COLS; i++){
    LCD_OutChar(*(line++));
  }
}

void print_lines(int screen_pos, int start_line, int num_lines) {
  int i;
  
  LCD_GoTo(screen_pos, 1);
  for(i=0; i <num_lines; i++) {
    print_line(scrollback[start_line + i]);
  }
  
}

void print_screen(void) {
  if (headline < tailline)
    print_lines(1, headline, tailline - headline);
  else {
    int num = SCREEN_LINES - tailline;
    print_lines(1, headline, num);
    print_lines(num + 1, 0, SCREEN_LINES - num );
  }
}

void term_clear_line(int term_line) {
  int i;
  
  for (i=0; i < TERM_COLS; i++) {
    scrollback[term_line][i] = 0;
  }
  if (term_line >= headline && term_line <= tailline) {
    LCD_GoTo(term_line - headline + 1, 1);
    print_line(scrollback[term_line]);
    if (term_line == tailline) {
      cursor_col = 0;
    }
  }
}

void term_new_line(void) {
  int i; 
  
  cursor_line++;
  
  if (cursor_line > tailline) {
    if (tailline >= headline + SCREEN_LINES) headline++; 
    tailline++;
    if (headline >= TERM_LINES) headline = 0;
  }
  if (cursor_line >= TERM_LINES) {
    if (cursor_line == tailline) tailline = 0;
    cursor_line = 0;
  }
  
  term_clear_line(cursor_line);
  
}

void term_clear_screen(void) {
  int i;
  LCD_Clear(0);
  
  lcd_line = lcd_col = 1;
  
  for(i = 0; i < SCREEN_LINES; i++) {
    term_new_line();
  }

}

void term_clear_scrollback(void) {
    int i, j;
                           
    for (i=0; i < TERM_LINES; i++) 
      for (j=0; j < TERM_COLS + 1; j++) 
        scrollback[i][j] = 0;

    headline = 0;
    tailline = SCREEN_LINES - 1;
    cursor_line = cursor_col = 0;
    
    LCD_Clear(0);
    lcd_line = lcd_col = 1;
}


void cursor_math(signed int line_off, signed int col_off) {
  cursor_col += col_off;
  if (cursor_col >= TERM_COLS) {
    cursor_line += (cursor_col / TERM_COLS);
    cursor_col %= (TERM_COLS); 
  }
  
  cursor_line += line_off;
  
  if (cursor_line > tailline) {
    if (tailline >= headline + SCREEN_LINES) headline += cursor_line - tailline; 
    tailline += cursor_line - tailline;
    if (headline >= TERM_LINES) headline = 0;
  }
  if (cursor_line >= TERM_LINES) {
    if (cursor_line == tailline) tailline = 0;
    cursor_line = 0;
  }    
  
  lcd_col = cursor_col + 1;
  if (headline > tailline) lcd_line = cursor_line + TERM_LINES - headline ;
  else lcd_line = cursor_line - headline + 1;
  
  LCD_GoTo(lcd_line, lcd_col);
}

void term_print_char(char c) {
  LCD_OutChar(c);
  
  //Adjust cursors
  cursor_math(0,1);
  
}

void term_print_string(char * str) {
  int i;
  int len = strlen(str);
  
  for(i=0; i < len; i++){
    term_print_char(str[i]);
  }
}

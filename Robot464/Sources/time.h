// Time.h placeholder, ill deal with this later

#define CLOCKS_PER_SEC 23809523

void usleep(int wait);

typedef unsigned long time_t;

time_t clock(void);

time_t time(void * in);

void term_print_char(char c);

void term_print_string(char * str);

void term_clear_screen(void);

void term_clear_scrollback(void);

void term_new_line(void);

void print_screen(void);
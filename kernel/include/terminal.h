#ifndef __TERMINAL_H__
#define __TERMINAL_H__
#include "defs.h"
void init_terminal(void);
void terminal_putc(char c);

extern bool _is_terminal_ready;
#endif
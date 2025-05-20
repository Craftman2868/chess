#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

// extern bool partial_redraw;
extern bool redraw;
extern unsigned int ticks;

void begin();
void end();
bool step();
void draw();

#endif /* MAIN_H */

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define DEBUGGING false

#define TICK_TIME 50  // ms
#define TEXT_SCALE 2  // times

// extern bool partial_redraw;
extern bool redraw;
extern unsigned int ticks;

void begin();
void end();
bool step();
void draw();

#endif /* MAIN_H */

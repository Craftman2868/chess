#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define DEBUGGING false

#define TICK_TIME 10  // ms  (set to 10 for the emulator, too slow otherwise, 50 or 100 for real hardware)

// extern bool partial_redraw;
extern bool redraw;
extern unsigned int ticks;

void begin();
void end();
bool step();
void draw();

#endif /* MAIN_H */

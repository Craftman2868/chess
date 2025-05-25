#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define DEBUGGING false

#define TICK_TIME 10  // 50  // ms  (set to 10 for the emulator, too slow otherwise, 50 or 100 for real hardware)
#define TEXT_SCALE 2  // times

// extern bool partial_redraw;
extern bool redraw;
extern unsigned int ticks;

void begin();
void end();
bool step();
void draw();

#endif /* MAIN_H */

#ifndef SCREENS_H
#define SCREENS_H

#include "main.h"

// menu.c
void step_menu();
void draw_menu();

// game.c
void step_game();
void draw_game();

#if DEBUGGING
// debug.c
void step_debug();
void draw_debug();
#endif  // DEBUGGING

#endif /* SCREENS_H */

#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "main.h"

enum {
#if DEBUGGING
    SCREEN_DEBUG,
#endif  // DEBUGGING
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_END,
} typedef app_screen_t;

extern bool running;

void set_screen(app_screen_t screen);

#endif /* APP_H */

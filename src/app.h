#ifndef APP_H
#define APP_H

enum {
    DEBUG,
    MENU,
    GAME,
} typedef app_screen_t;

void set_screen(app_screen_t screen);

#endif /* APP_H */

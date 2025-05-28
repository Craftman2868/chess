#include "app.h"
#include "main.h"

#include <stdbool.h>
#include <keypadc.h>
#include "screens.h"
#include "input.h"
#include "menu.h"

// App

app_screen_t current_screen;


void screen_begin() {
    switch (current_screen)
    {
#if DEBUGGING
    case SCREEN_DEBUG:
        // No begin function
        break;
#endif  // DEBUGGING
    case SCREEN_MENU:
        begin_menu();
        break;

    case SCREEN_GAME:
        begin_game();
        break;
    
    // case SCREEN_END:
    //     begin_end();
    //     break;

    default:
        break;
    }
}

void set_screen(app_screen_t screen)
{
    current_screen = screen;

    redraw = true;
    
    screen_begin();
}

// Main

bool running;

void main_menu_callback(short item);

void begin()
{
    static const char *items[] = {"Play", "Quit"};

    running = true;

#if DEBUGGING
    set_screen(SCREEN_DEBUG);

    return;
#endif  // DEBUGGING

    open_menu("Chess", items, sizeof(items) / sizeof(char *), main_menu_callback);
}

void main_menu_callback(short item)
{
    switch (item)
    {
    case 0:  // Play
        set_screen(SCREEN_GAME);
        break;

    case 1:  // Quit
        running = false;
        break;

    default:
        break;
    }
}

void end()
{
}

bool step()
{
    update_inputs();

    if (kb_On)
        return false;  // Debugging interrupt signal (to be removed)

    switch (current_screen)
    {
#if DEBUGGING
    case SCREEN_DEBUG:
        step_debug();
        break;
#endif  // DEBUGGING
    case SCREEN_MENU:
        step_menu();
        break;

    case SCREEN_GAME:
        step_game();
        break;
    
    case SCREEN_END:
        step_end();
        break;

    default:
        running = false;  // Prevent an endless loop
        break;
    }

    return running;
}

void draw()
{
    switch (current_screen)
    {
#if DEBUGGING
    case SCREEN_DEBUG:
        draw_debug();
        break;
#endif  // DEBUGGING
    case SCREEN_MENU:
        draw_menu();
        break;

    case SCREEN_GAME:
        draw_game();
        break;
    
    case SCREEN_END:
        draw_end();
        break;

    default:
        break;
    }
}

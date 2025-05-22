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
    case DEBUG:
        // No begin function
        break;
#endif  // DEBUGGING
    case MENU:
        // No begin function (yet)
        break;

    case GAME:
        begin_game();
        break;

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
    static const char *items[] = {"Play", "Quit", "A", "B", "C", "D", "E"};

    running = true;

#if DEBUGGING
    set_screen(DEBUG);

    return;
#endif  // DEBUGGING

    open_menu("Chess", items, sizeof(items) / sizeof(char *), main_menu_callback);
}

void main_menu_callback(short item)
{
    switch (item)
    {
    case 0: // Play
        set_screen(GAME);
        break;

    case 1: // Quit
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
    case DEBUG:
        step_debug();
        break;
#endif  // DEBUGGING
    case MENU:
        step_menu();
        break;

    case GAME:
        step_game();
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
    case DEBUG:
        draw_debug();
        break;
#endif  // DEBUGGING
    case MENU:
        draw_menu();
        break;

    case GAME:
        draw_game();
        break;

    default:
        break;
    }
}

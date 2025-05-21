#include "app.h"
#include "main.h"

#include <stdbool.h>
#include <keypadc.h>
#include "screens.h"
#include "menu.h"

// App

app_screen_t current_screen;

void set_screen(app_screen_t screen)
{
    current_screen = screen;

    redraw = true;
}

// Main

bool running;

void main_menu_callback(short item);

void begin()
{
    static const char *items[] = {"Play", "Quit", "A", "B", "C", "D", "E"};
    running = true;

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
    kb_Scan();

    if (kb_On)
        return false;  // Debugging interrupt signal (to be removed)

    switch (current_screen)
    {
    case MENU:
        step_menu();
        break;

    case GAME:
        // Not implemented yet

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
    case MENU:
        draw_menu();
        break;

    case GAME:
        // Not implemented yet

    default:
        break;
    }
}

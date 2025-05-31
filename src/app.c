#include "app.h"
#include "main.h"

#include <stdbool.h>
#include <keypadc.h>
#include "screens.h"
#include "input.h"
#include "game.h"
#include "menu.h"
#include "save.h"
#include "save_menu.h"

// App

app_screen_t current_screen;


void screen_begin() {
    switch (current_screen)
    {
    case SCREEN_MENU:
        begin_menu();
        break;

    case SCREEN_GAME:
        begin_game();
        break;
    
    case SCREEN_END:
        begin_end();
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


void main_menu_callback(short item);

short main_menu_item_count;

void open_main_menu()
{
    static const char *items[3];

    if (get_new_id() != -1)
    {
        items[0] = "New game";
        items[1] = "Manage saves";
        items[2] = "Quit";
        main_menu_item_count = 3;
    }
    else
    {
        items[0] = "Manage saves";
        items[1] = "Quit";
        main_menu_item_count = 2;
    }

    open_menu("Chess", items, main_menu_item_count, main_menu_callback);
}

void main_menu_callback(short item)
{
    if (item == -1)  // Back
    {
        running = false;
        return;
    }

    if (main_menu_item_count == 2)
        item++;

    switch (item)
    {
    case 0:  // New game
        run_new_game();
        break;
    
    case 1:  // Manage saves
        open_save_menu();
        break;

    case 2:  // Quit
        running = false;
        break;

    default:
        break;
    }
}

// Main

bool running;

void begin()
{
    running = true;

#if DEBUGGING
    set_screen(SCREEN_DEBUG);

    return;
#endif  // DEBUGGING

    open_main_menu();
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

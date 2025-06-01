#include "menu.h"
#include "screens.h"

#include <keypadc.h>
#include <graphx.h>
#include "main.h"
#include "app.h"
#include "input.h"


#define TEXT_SCALE 2  // times

#define DISPLAY_ITEM_COUNT 6  // Maximum number of menu item that can be displayed
#define MENU_MARGIN_TOP 30  // px
#define MENU_MARGIN_LEFT 42  // px
#define MENU_MARGIN_TITLE 20  // px
#define MENU_MARGIN_TITLE_LEFT 64  // px
#define TEXT_MARGIN 5  // px
#define ITEM_WIDTH 200  // px

#define DEFAULT_TEXT_HEIGHT 8  // px  (can't be changed)
#define TEXT_HEIGHT (DEFAULT_TEXT_HEIGHT * TEXT_SCALE)
#define ITEM_HEIGHT (TEXT_HEIGHT + TEXT_MARGIN * 2)
#define MENU_ITEMS_MARGIN_TOP (MENU_MARGIN_TOP + TEXT_HEIGHT + MENU_MARGIN_TITLE)

static struct
{
    const char *title;
    const char **items;
    short item_count;
    short current_item;
    short offset;
    union
    {
        void (*callback)(short);
        void (*advanced_callback)(short, input_key_t);
    };
    bool advanced;
} menu;

void init_menu(const char *title, const char *items[], short item_count)
{
    menu.title = title;
    menu.items = items;
    menu.item_count = item_count;
    menu.current_item = 0;
    menu.offset = 0;
}

void open_menu(const char *title, const char *items[], short item_count, void (*callback)(short))
{
    init_menu(title, items, item_count);

    menu.callback = callback;
    menu.advanced = false;

    set_screen(SCREEN_MENU);
}

void open_menu_advanced(const char *title, const char *items[], short item_count, void (*callback)(short, input_key_t))
{
    init_menu(title, items, item_count);

    menu.advanced_callback = callback;
    menu.advanced = true;

    set_screen(SCREEN_MENU);
}

// Because the native '%' behave weirdly with negative numbers
inline short mod(short a, short b)
{
    short r = a % b;
    return r < 0 ? r + b : r;
}

void menu_select_item(short item)
{
    if (item == menu.current_item)
        return;

    menu.current_item = mod(item, menu.item_count);
    if (menu.current_item < menu.offset)
        menu.offset = menu.current_item;
    else if (menu.current_item >= menu.offset + DISPLAY_ITEM_COUNT)
        menu.offset = menu.current_item - DISPLAY_ITEM_COUNT + 1;

    redraw = true;
}

void menu_up()
{
    menu.current_item = mod(menu.current_item - 1, menu.item_count);
    if (menu.current_item < menu.offset)
        menu.offset--;
    else if (menu.current_item >= menu.offset + DISPLAY_ITEM_COUNT)
        menu.offset = menu.item_count - DISPLAY_ITEM_COUNT;

    redraw = true;
}

void menu_down()
{
    menu.current_item = mod(menu.current_item + 1, menu.item_count);
    if (menu.current_item >= menu.offset + DISPLAY_ITEM_COUNT)
        menu.offset++;
    else if (menu.current_item < menu.offset)
        menu.offset = 0;

    redraw = true;
}

void menu_select()
{
    if (menu.callback != NULL)
        menu.callback(menu.current_item);
}

void menu_back()
{
    if (menu.callback != NULL)
        menu.callback(-1);
}

void handle_advanced(input_key_t key)
{
    if (menu.advanced)
    {
        if (menu.advanced_callback != NULL)
            menu.advanced_callback(menu.current_item, key);
        return;
    }

    switch (key)
    {
    case kb_KeyLeft:
    case kb_KeyClear:
        menu_back();
        break;
    case kb_KeyRight:
    case kb_KeyEnter:
        menu_select();
        break;
    default:
        break;
    }
}

void handle_event(input_event_t event)
{
    if (event.type != EV_KEY_DOWN && event.type != EV_KEY_REPEAT)
    {
        return;
    }

    switch (event.key)
    {
    case kb_KeyUp:
        menu_up();
        break;
    case kb_KeyDown:
        menu_down();
        break;
    default:
        handle_advanced(event.key);
        break;
    }
}

void menu_update()
{
    if (menu.advanced)
    {
        menu.advanced_callback(menu.current_item, 0);
    }
    else
    {
        menu.callback(-2);
    }
}

void begin_menu()
{
    gfx_SetTextScale(TEXT_SCALE, TEXT_SCALE);
}

void step_menu()
{
    input_event_t event;

    while (get_event(&event))
    {
        handle_event(event);
    }
}

void draw_menu()
{
    short item;

    gfx_FillScreen(0xFF);

    gfx_SetColor(0xF7);
    gfx_SetTextBGColor(0xFF);
    gfx_SetTextFGColor(0x20);

    gfx_PrintStringXY(menu.title, MENU_MARGIN_TITLE_LEFT, MENU_MARGIN_TOP);

    gfx_SetTextBGColor(0xF7);

    for (short i = 0; i < DISPLAY_ITEM_COUNT; i++)
    {
        item = menu.offset + i;

        if (item >= menu.item_count)
            break;

        if (item == menu.current_item)
        {
            gfx_SetColor(0x20);
            gfx_SetTextBGColor(0x20);
            gfx_SetTextFGColor(0xF7);
        }
        else if (item == menu.current_item + 1)
        {
            gfx_SetColor(0xF7);
            gfx_SetTextBGColor(0xF7);
            gfx_SetTextFGColor(0x20);
        }

        gfx_FillRectangle(MENU_MARGIN_LEFT, MENU_ITEMS_MARGIN_TOP + i * ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT);

        gfx_PrintStringXY(menu.items[item], MENU_MARGIN_LEFT + TEXT_MARGIN, MENU_ITEMS_MARGIN_TOP + TEXT_MARGIN + i * ITEM_HEIGHT);
    }
}

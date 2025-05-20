#include "menu.h"
#include "screens.h"

#include <keypadc.h>
#include <graphx.h>
#include <sys/rtc.h>
#include "main.h"
#include "app.h"


#define DISPLAY_ITEM_COUNT 4 // Number of menu item displayed
#define ITEM_HEIGHT 12       // px

struct
{
    char *title;
    char **items;
    unsigned short item_count;
    unsigned short current_item;
    unsigned short offset;
    void (*callback)(unsigned short);
} menu;

void open_menu(char *title, char *items[], unsigned short item_count, void (*callback)(unsigned short))
{
    menu.title = title;
    menu.items = items;
    menu.item_count = item_count;
    menu.current_item = 0;
    menu.offset = 0;
    menu.callback = callback;
    set_screen(MENU);

    redraw = true;
}

void step_menu()
{
    if (kb_Data[7] & kb_Up)
    {
        menu.current_item = (menu.current_item - 1) % menu.item_count;
        if (menu.current_item < menu.offset)
            menu.offset--;
        else if (menu.current_item >= menu.offset + DISPLAY_ITEM_COUNT)
            menu.offset = menu.item_count - DISPLAY_ITEM_COUNT;

        redraw = true;
    }
    else if (kb_Data[7] & kb_Down)
    {
        menu.current_item = (menu.current_item + 1) % menu.item_count;
        if (menu.current_item >= menu.offset + DISPLAY_ITEM_COUNT)
            menu.offset++;
        else if (menu.current_item < menu.offset)
            menu.offset = 0;

        redraw = true;
    }
    else if (kb_Data[7] & kb_Right || kb_Data[6] & kb_Enter)
    {
        if (menu.callback != NULL)
        {
            menu.callback(menu.current_item);
        }
    }
}

void draw_menu()
{
    unsigned short item;

    gfx_FillScreen(0xFF);

    gfx_SetColor(0xF7);
    gfx_SetTextBGColor(0xF7);
    gfx_SetTextFGColor(0x20);

    gfx_PrintStringXY(menu.title, 48, 10);

    for (unsigned short i = 0; i < DISPLAY_ITEM_COUNT; i++)
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

        gfx_FillRectangle(22, 22 + i * ITEM_HEIGHT, 100, ITEM_HEIGHT);
        // gfx_PrintStringXY(menu.items[item], 24, 22 + i * ITEM_HEIGHT + 2);
    }
}

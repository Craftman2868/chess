#include "save_menu.h"

#include <stdint.h>
#include <string.h>
#include "app.h"
#include "game.h"
#include "menu.h"
#include "save.h"
#include "input.h"
#include "yesno.h"

char item_names[MAX_SAVE_COUNT][16];
int8_t item_ids[MAX_SAVE_COUNT];
short item_count;

void add_new_game_item()
{
    strcpy(item_names[item_count], "New game");
    item_ids[item_count] = -1;
    item_count++;
}

void make_items()
{
    int8_t id = -1;
    short i = 0;

    while ((id = next_save(id)) != -1)
    {
        strcpy(item_names[i], "Save #");
        item_names[i][5] = '0' + id;
        item_ids[i] = id;
        i++;
    }

    item_count = i;

    if (item_count < MAX_SAVE_COUNT)
    {
        add_new_game_item();
    }
}

void save_menu_callback(short i, input_key_t key);

void open_save_menu()
{
    make_items();

    static char *items[MAX_SAVE_COUNT];

    for (uint8_t i = 0; i < item_count; i++)  // Yes, it's completely stupid but it's the only way I found to make it work
        items[i] = item_names[i];

    open_menu_advanced("Manage saves", (const char **)items, item_count, save_menu_callback);
}

void delete_yesno_callback(yesno_t answer);

int8_t id;
char yesno_title[] = "Delete save #?";

void save_menu_callback(short i, input_key_t key)
{
    id = item_ids[i];

    switch (key)
    {
    case kb_KeyRight:
    case kb_KeyEnter:
        if (id == -1)
        {
            if (!run_new_game())
                open_main_menu();
            return;
        }

        if (!run_save(id))
            open_main_menu();
        break;
    
    case kb_KeyLeft:
    case kb_KeyClear:
        open_main_menu();
        break;
    
    case kb_KeyDel:
        if (id == -1)
            break;
        yesno_title[12] = '0' + id;
        open_yesno(yesno_title, delete_yesno_callback);
        break;
    default:
        break;
    }
}

void delete_yesno_callback(yesno_t answer)
{
    if (answer == YES)
        delete_save(id);

    open_save_menu();
}

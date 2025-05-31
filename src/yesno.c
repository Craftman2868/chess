#include "yesno.h"

#include "menu.h"

const char *yes_no_items[] = {"Yes", "No"};

void open_yesno(char *title, void (*callback)(yesno_t))
{
    open_menu(title, yes_no_items, 2, (void (*)(short)) callback);
}

#ifndef MENU_H
#define MENU_H

#include "input.h"

void open_menu(const char *title, const char *items[], short item_count, void (*callback)(short));
void open_menu_advanced(const char *title, const char *items[], short item_count, void (*callback)(short, input_key_t));

#endif /* MENU_H */

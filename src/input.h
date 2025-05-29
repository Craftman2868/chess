#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <keypadc.h>

#include "main.h"


enum
{
    EV_NULL,
    EV_KEY_DOWN,
    EV_KEY_UP,
    EV_KEY_REPEAT,
#if DEBUGGING
    EV_DEBUG,
#endif  // DEBUGGING
} typedef input_event_type_t;

typedef kb_lkey_t input_key_t;

#define kb_KeyOn ((input_key_t) 1);

struct
{
    input_event_type_t type;
    union
    {
        input_key_t key;  // EV_KEY_...
#if DEBUGGING
        char *msg;  // EV_DEBUG
#endif  // DEBUGGING
    };
} typedef input_event_t;

void update_inputs();

void clear_events();
bool get_event(input_event_t *event);

#endif /* INPUT_H */

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

enum
{
    EV_NULL,
    EV_KEY_DOWN,
    EV_KEY_UP,
    EV_KEY_REPEAT,
    EV_DEBUG,
} typedef input_event_type_t;

// special value: 0, 0 = Key On
struct
{
    uint8_t group, key;
} typedef input_key_t;

struct
{
    input_event_type_t type;
    union
    {
        input_key_t key;  // EV_KEY_...
        char *msg;  // EV_DEBUG
    };
} typedef input_event_t;

void update_inputs();

void clear_events();
bool get_event(input_event_t *event);

#endif /* INPUT_H */

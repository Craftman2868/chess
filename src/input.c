#include "input.h"

#include <keypadc.h>
#include "main.h"

#define KEY_REPEAT_INTERVAL 4  // ticks
#define EVENT_QUEUE_LENGTH 32

uint8_t last_kb[8];
bool last_on = false;
input_key_t key_repeat_last;
unsigned int key_repeat_ticks;


struct
{
    input_event_t data[EVENT_QUEUE_LENGTH];
    uint8_t head, length;
} typedef event_queue_t;

static event_queue_t event_queue;

void queue_add_event(event_queue_t *queue, input_event_t event)
{
    if (queue->length >= EVENT_QUEUE_LENGTH)
        return;

    queue->data[queue->head] = event;
    queue->head = (queue->head + 1) % EVENT_QUEUE_LENGTH;
    queue->length++;
}

bool queue_get_event(event_queue_t *queue, input_event_t *event)
{
    if (queue->length == 0)
        return false;

    *event = queue->data[(queue->head - queue->length) % EVENT_QUEUE_LENGTH];
    queue->length--;

    return true;
}

void queue_clear_events(event_queue_t *queue)
{
    queue->length = 0;
}

void update_inputs()
{
    uint8_t kb, diff;
    input_event_t event;
    input_key_t key_repeat = {0, 0};

    kb_Scan();

    if (kb_Data[6] & kb_Enter)
    {
        event.type = EV_DEBUG;
        event.msg = "Enter";
        queue_add_event(&event_queue, event);
    }

    // return;

    for (int i = 1; i < 8; i++)
    {
        kb = kb_Data[i];
        diff = kb ^ last_kb[i];

        for (int j = 0; j < 8; j++)
        {
            if (diff & (1 << j))
            {
                event.type = (kb & (1 << j)) ? EV_KEY_DOWN : EV_KEY_UP;
                event.key.group = i;
                event.key.key = (1 << j);

                queue_add_event(&event_queue, event);
            }
            else if (kb & (1 << j))
            {
                key_repeat.group = i;
                key_repeat.key = (1 << j);
            }
        }

        last_kb[i] = kb;
    }

    if (last_on != kb_On)
    {
        event.type = kb_On ? EV_KEY_DOWN : EV_KEY_UP;
        event.key.group = 0;
        event.key.key = 0;

        queue_add_event(&event_queue, event);
    }

    if (key_repeat.group == 0 && key_repeat.key == 0)
    {
        return;
    }

    if (key_repeat.group == key_repeat_last.group && key_repeat.key == key_repeat_last.key)
    {
        key_repeat_ticks++;
        if (key_repeat_ticks >= KEY_REPEAT_INTERVAL)
        {
            event.type = EV_KEY_REPEAT;
            event.key = key_repeat;

            queue_add_event(&event_queue, event);
            key_repeat_ticks = 0;
        }
    }
    else
    {
        key_repeat_last = key_repeat;
        key_repeat_ticks = 0;
    }
}

void clear_events()
{
    queue_clear_events(&event_queue);
}

bool get_event(input_event_t *event)
{
    return queue_get_event(&event_queue, event);
}

#include "main.h"

#if DEBUGGING

#include "screens.h"

#include <graphx.h>
#include "input.h"

#define DEBUG_DISPLAY_EVENT_COUNT 8

input_event_t events[DEBUG_DISPLAY_EVENT_COUNT];
uint8_t head = 0;

void step_debug()
{
    input_event_t event;

    while (get_event(&event))
    {
        events[head] = event;
        head = (head + 1) % DEBUG_DISPLAY_EVENT_COUNT;
        redraw = true;
    }
}

void draw_debug()
{
    gfx_FillScreen(0xFF);
    gfx_SetTextBGColor(0xFF);
    gfx_SetTextFGColor(0x00);
    gfx_SetColor(0x00);

    for (uint8_t i = 0; i < DEBUG_DISPLAY_EVENT_COUNT; i++)
    {
// #define EV (events[(DEBUG_DISPLAY_EVENT_COUNT-i+head) % DEBUG_DISPLAY_EVENT_COUNT])
#define EV (events[i])
        if ((i+1) % DEBUG_DISPLAY_EVENT_COUNT == head) {
            gfx_FillCircle(10, 30 + i * 20, 5);
        }

        if (EV.type == EV_NULL)
            continue;

#define PRINT_STR(s, x) gfx_PrintStringXY(s, x, 20 + i * 20 + 2)
#define PRINT_UINT(d, n, x) gfx_SetTextXY(x, 20 + i * 20 + 2); gfx_PrintUInt(d, n)

        if (EV.type == EV_DEBUG)
        {
            PRINT_STR("DEBUG", 20);
            PRINT_STR(EV.msg, 130);
            continue;
        }

        if (EV.type == EV_KEY_DOWN)
        {
            PRINT_STR("DOWN", 20);
        }
        else if (EV.type == EV_KEY_UP)
        {
            PRINT_STR("UP", 20);
        }
        else if (EV.type == EV_KEY_REPEAT)
        {
            PRINT_STR("REPEAT", 20);
        }
        PRINT_UINT(EV.key, 2, 130);
#undef EV
#undef PRINT_STR
#undef PRINT_UINT
    }
}

#endif  // DEBUGGING

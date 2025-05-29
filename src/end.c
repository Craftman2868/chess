#include "screens.h"

#include <graphx.h>
#include "app.h"
#include "input.h"
#include "chess.h"


unsigned int end_ticks;

void begin_end()
{
    end_ticks = ticks;
}

void step_end()
{
    input_event_t event;

    while (get_event(&event))
    {
        if (event.type != EV_KEY_DOWN && event.type != EV_KEY_REPEAT)
            continue;
        
        if (event.key != kb_KeyClear && ticks - end_ticks < 20)
            continue;

        open_main_menu();  // Go back to the main menu
    }
}

// Should only be called once
void draw_end()
{
    gfx_BlitScreen();
    gfx_SetTextFGColor(2);  // Grey
    gfx_SetTextBGColor(1);  // White

    gfx_SetTextScale(2, 2);

    gfx_SetColor(1);  // White

    switch (game_state)
    {
    case CHECKMATE:
        gfx_FillRectangle(65, 95, 150, 40);
        gfx_PrintStringXY("CHECKMATE", 70, 100);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY(winner == WHITE ? "WHITE won" : "BLACK won", 110, 120);
        break;
    case DRAW_STALEMATE:
        gfx_FillRectangle(96, 95, 84, 40);
        gfx_PrintStringXY("DRAW", 107, 100);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Stalemate", 105, 120);
        break;
    case DRAW_MATERIAL:
        gfx_FillRectangle(60, 95, 157, 40);
        gfx_PrintStringXY("DRAW", 107, 100);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Insufficient material", 65, 120);
        break;
    default:
        break;
    }
}

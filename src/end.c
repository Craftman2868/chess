#include "screens.h"

#include <graphx.h>
#include "app.h"
#include "input.h"
#include "chess.h"


void step_end()
{
    input_event_t event;

    while (get_event(&event))
    {
        if (event.type != EV_KEY_DOWN && event.type != EV_KEY_REPEAT)
            continue;
        
        set_screen(SCREEN_MENU);  // Go back to the main menu
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
    gfx_FillRectangle(65, 95, 150, 40);

    if (game_state == CHECKMATE)
    {
        gfx_PrintStringXY("CHECKMATE", 70, 100);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY(winner == WHITE ? "WHITE won" : "BLACK won", 110, 120);
    }
    else
    {
        gfx_PrintStringXY("DRAW", 110, 110);
    }
}

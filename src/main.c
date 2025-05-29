#include <stdbool.h>
#include <sys/timers.h>
#include <graphx.h>

#include "main.h"


bool redraw;
unsigned int ticks = 0;

int main(void)
{
    redraw = true;

    begin();
    gfx_Begin();

    gfx_SetDrawBuffer();

    while (step())
    {
        delay(TICK_TIME);
        ticks++;
        if (!redraw)
            continue;
        redraw = false;
        draw();
        gfx_SwapDraw();
    }

    gfx_End();
    end();

    return 0;
}

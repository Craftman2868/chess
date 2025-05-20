#include <stdlib.h>
#include <sys/timers.h>
#include <graphx.h>

#include "main.h"

#define TICK_TIME 50  // ms

// bool partial_redraw;
bool redraw;
unsigned int ticks = 0;


/* Main function, called first */
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
        // if (partial_redraw)
        //     gfx_BlitScreen();
        if (!redraw)
            continue;
        redraw = false;
        draw();
        gfx_SwapDraw();
    }

    gfx_End();
    end();

    /* Return 0 for success */
    return 0;
}

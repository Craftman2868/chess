#include "screens.h"
#include "chess.h"

#include <stdint.h>
#include <keypadc.h>
#include <graphx.h>
#include "app.h"
#include "input.h"


piece_t board[8*8];
color_t turn;

#define BOARD(x, y) board[y*8+x]

void init_board() {
    BOARD(0, 0) = (piece_t){ROOK,   BLACK};
    BOARD(0, 1) = (piece_t){KNIGHT, BLACK};
    BOARD(0, 2) = (piece_t){BISHOP, BLACK};
    BOARD(0, 3) = (piece_t){QUEEN,  BLACK};
    BOARD(0, 4) = (piece_t){KING,   BLACK};
    BOARD(0, 5) = (piece_t){BISHOP, BLACK};
    BOARD(0, 6) = (piece_t){KNIGHT, BLACK};
    BOARD(0, 7) = (piece_t){ROOK,   BLACK};

    BOARD(7, 0) = (piece_t){ROOK,   WHITE};
    BOARD(7, 1) = (piece_t){KNIGHT, WHITE};
    BOARD(7, 2) = (piece_t){BISHOP, WHITE};
    BOARD(7, 3) = (piece_t){QUEEN,  WHITE};
    BOARD(7, 4) = (piece_t){KING,   WHITE};
    BOARD(7, 5) = (piece_t){BISHOP, WHITE};
    BOARD(7, 6) = (piece_t){KNIGHT, WHITE};
    BOARD(7, 7) = (piece_t){ROOK,   WHITE};

    for (uint8_t i = 0; i < 8; i++) {
        BOARD(1, i) = (piece_t){PAWN, BLACK};
        BOARD(6, i) = (piece_t){PAWN, WHITE};
    }
}

void init_game() {
    init_board();

    turn = WHITE;
}

void step_game() {
    input_event_t event;

    while (get_event(&event))
    {
        if (event.type == EV_KEY_DOWN)
        {
            if (event.key.group == 6 && event.key.key == kb_Clear)
            {
                running = false;
            }
        }
    }
}

void draw_game() {
    gfx_FillScreen(0xFF);
    gfx_SetTextBGColor(0xFF);
    gfx_SetTextFGColor(0x20);
    
    gfx_PrintStringXY("Chess Game...", 30, 30);
}

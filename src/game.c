#include "screens.h"
#include "chess.h"

#include <stdint.h>
#include <keypadc.h>
#include <graphx.h>
#include "app.h"
#include "input.h"

#include "gfx/gfx.h"

#define TILE_W 22  // px
#define TILE_H 22  // px
#define OF_X 50  // px  // Offset X
#define OF_Y 30  // px  // Offset Y


piece_t board[8*8];
color_t turn;

#define BOARD(x, y) board[y*8+x]

void init_board() {
    BOARD(0, 0) = (piece_t){ROOK,   BLACK};
    BOARD(1, 0) = (piece_t){KNIGHT, BLACK};
    BOARD(2, 0) = (piece_t){BISHOP, BLACK};
    BOARD(3, 0) = (piece_t){QUEEN,  BLACK};
    BOARD(4, 0) = (piece_t){KING,   BLACK};
    BOARD(5, 0) = (piece_t){BISHOP, BLACK};
    BOARD(6, 0) = (piece_t){KNIGHT, BLACK};
    BOARD(7, 0) = (piece_t){ROOK,   BLACK};

    BOARD(0, 7) = (piece_t){ROOK,   WHITE};
    BOARD(1, 7) = (piece_t){KNIGHT, WHITE};
    BOARD(2, 7) = (piece_t){BISHOP, WHITE};
    BOARD(3, 7) = (piece_t){QUEEN,  WHITE};
    BOARD(4, 7) = (piece_t){KING,   WHITE};
    BOARD(5, 7) = (piece_t){BISHOP, WHITE};
    BOARD(6, 7) = (piece_t){KNIGHT, WHITE};
    BOARD(7, 7) = (piece_t){ROOK,   WHITE};

    for (uint8_t i = 0; i < 8; i++) {
        BOARD(i, 1) = (piece_t){PAWN, BLACK};
        BOARD(i, 6) = (piece_t){PAWN, WHITE};
    }
}

void init_game() {
    init_board();

    turn = WHITE;
}

gfx_sprite_t *get_piece_sprite(piece_type_t piece, color_t color) {
    return chess_tiles[6*(!color)+piece-1];
}

void draw_piece(piece_t piece, uint8_t x, uint8_t y) {
    gfx_sprite_t *sprite = get_piece_sprite(piece.type, piece.color);

    gfx_TransparentSprite(sprite, OF_X + x * TILE_W, OF_Y + y * TILE_H);
}

void draw_board() {
    gfx_FillScreen(0x03);  // White

    gfx_SetColor(0x01);  // Grey

    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            if ((x + y) % 2 == 0) {
                gfx_FillRectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
            }
            if (BOARD(x, y).type != NONE) {
                draw_piece(BOARD(x, y), x, y);
            }
        }
    }
}


// screens.h functions

void begin_game() {
    init_game();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
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
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);

    draw_board();
}

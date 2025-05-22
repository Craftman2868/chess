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

piece_t board[8 * 8];
color_t turn;
pos_t selected;

#define POS(x, y) (y * 8 + x)
#define BOARD(x, y) board[POS(x, y)]

void init_board()
{
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

    for (uint8_t i = 0; i < 8; i++)
    {
        BOARD(i, 1) = (piece_t){PAWN, BLACK};
        BOARD(i, 6) = (piece_t){PAWN, WHITE};
    }
}

void init_game()
{
    init_board();

    selected.x = 0;
    selected.y = 7;

    turn = WHITE;
}

pos_t find_piece(piece_type_t piece, color_t color)
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            if (BOARD(x, y).type == piece && BOARD(x, y).color == color)
            {
                return (pos_t){x, y};
            }
        }
    }

    return (pos_t){0, 0}; // Not found
}

pos_t piece_moves[24];
uint8_t piece_moves_count;

#define add_move(x, y) piece_moves[piece_moves_count++] = (pos_t) { x, y }

void get_piece_moves(piece_t piece, uint8_t x, uint8_t y)
{
    piece_moves_count = 0;

    switch (piece.type)
    {
    case PAWN:
        if (piece.color == WHITE)
        {
            add_move(x, y - 1);
            if (y == 6)
                add_move(x, y - 2);
        }
        else
        {
            add_move(x, y + 1);
            if (y == 1)
                add_move(x, y + 2);
        }
        break;
    case KNIGHT:
        // TODO
        break;
    case BISHOP:
        // TODO
        break;
    case ROOK:
        // TODO
        break;
    case QUEEN:
        // TODO
        break;
    case KING:
        // TODO
        break;
    default:
        break;
    }
}

// Calculate the piece's potential attacks and store it in piece_moves
// Not checked:
//   - valid move
//   - is there a piece to attack on the target square
void get_piece_attacks(piece_t piece, uint8_t x, uint8_t y)
{
    if (piece.type == PAWN)
    {
        piece_moves_count = 0;
        if (piece.color == WHITE)
        {
            add_move(x - 1, y - 1);
            add_move(x + 1, y - 1);
        }
        else
        {
            add_move(x - 1, y + 1);
            add_move(x + 1, y + 1);
        }
        // TODO: en passant (maybe not here)
        return;
    }
    
    get_piece_moves(piece, x, y);
}

bool is_in_check(color_t color)
{
    pos_t king = find_piece(KING, color);

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            piece_t piece = BOARD(x, y);
            if (piece.color != color && piece.type != NONE)
            {
                // Check if the piece threaten the king
                get_piece_attacks(piece, x, y);

                for (uint8_t i = 0; i < piece_moves_count; i++)
                {
                    if (piece_moves[i].x == king.x && piece_moves[i].y == king.y)
                        return true;  // King is in check
                }
            }
        }
    }

    return false;
}

gfx_sprite_t *get_piece_sprite(piece_type_t piece, color_t color)
{
    return chess_tiles[6 * (!color) + piece - 1];
}

void draw_piece(piece_t piece, uint8_t x, uint8_t y)
{
    gfx_sprite_t *sprite = get_piece_sprite(piece.type, piece.color);

    gfx_TransparentSprite(sprite, OF_X + x * TILE_W, OF_Y + y * TILE_H);
}

gfx_sprite_t *get_potential_sprite(uint8_t n, color_t color)
{
    return chess_tiles[12 + 6 * (!color) + n];
}

void draw_potential(uint8_t n, color_t color, uint8_t x, uint8_t y)
{
    gfx_sprite_t *sprite = get_potential_sprite(n, color);

    gfx_TransparentSprite(sprite, OF_X + x * TILE_W, OF_Y + y * TILE_H);
}

bool potential_move[8*8];

void draw_board()
{
    gfx_FillScreen(1); // White

    gfx_SetColor(2); // Grey

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            if ((x + y) % 2 == 0)
            {
                gfx_FillRectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
            }
            if (BOARD(x, y).type != NONE)
            {
                draw_piece(BOARD(x, y), x, y);
            }
            else if (potential_move[POS(x, y)])
            {
                draw_potential(3, 1 - (x + y) % 2, x, y);
            }
            if (selected.x == x && selected.y == y)
            {
                gfx_SetColor(4); // Blue
                gfx_Rectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
                gfx_Rectangle(OF_X + x * TILE_W + 1, OF_Y + y * TILE_H + 1, TILE_W - 2, TILE_H - 2);
                gfx_SetColor(2); // Grey
            }
        }
    }
}

// Called when enter is pressed
void select()
{
}

// screens.h functions

void begin_game()
{
    init_game();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
}

void step_game()
{
    input_event_t event;

    while (get_event(&event))
    {
        if (event.type != EV_KEY_DOWN && event.type != EV_KEY_REPEAT)
            continue;

        switch (event.key.group)
        {
        case 6:
            if (event.key.key == kb_Clear)
                running = false;
            if (event.key.key == kb_Enter)
                select();
            break;
        case 7:
            switch (event.key.key)
            {
            case kb_Up:
                if (selected.y > 0)
                {
                    selected.y--;
                    redraw = true;
                }
                break;
            case kb_Down:
                if (selected.y < 7)
                {
                    selected.y++;
                    redraw = true;
                }
                break;
            case kb_Left:
                if (selected.x > 0)
                {
                    selected.x--;
                    redraw = true;
                }
                break;
            case kb_Right:
                if (selected.x < 7)
                {
                    selected.x++;
                    redraw = true;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    if (redraw)
    {
        if (BOARD(selected.x, selected.y).type != NONE)
            get_piece_moves(BOARD(selected.x, selected.y), selected.x, selected.y);
        else
            piece_moves_count = 0;
        
        for (uint8_t i = 0; i < 8 * 8; i++)
            potential_move[i] = false;

        for (uint8_t i = 0; i < piece_moves_count; i++)
        {
            pos_t move = piece_moves[i];
            potential_move[POS(move.x, move.y)] = true;
        }
    }
}

void draw_game()
{
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);

    draw_board();
}

#include "screens.h"
#include "chess.h"

#include <stdint.h>
#include <keypadc.h>
#include <graphx.h>
#include <string.h>  // memcpy
#include "app.h"
#include "input.h"

#include "gfx/gfx.h"

#include <debug.h>

#define TILE_W 22  // px
#define TILE_H 22  // px
#define OF_X 50  // px  // Offset X
#define OF_Y 30  // px  // Offset Y

piece_t board[8 * 8];
color_t turn;
bool king_moved[2];
bool in_check;
pos_t cursor;
pos_t selected;
uint8_t circle_size = 0;

#define POS_XY(x, y) ((y) * 8 + (x))
#define POS(p) (p.y * 8 + p.x)
#define BOARD(x, y) board[POS_XY(x, y)]
#define BOARD_POS(p) board[POS(p)]

#define cursor_piece BOARD_POS(cursor)

#define unselect() selected.x = 8
#define is_selected (selected.x < 8)  // && selected.y < 8)
#define selected_piece BOARD_POS(selected)

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

    cursor.x = 0;
    cursor.y = 7;

    unselect();

    turn = WHITE;

    in_check = false;
    king_moved[BLACK] = 0;
    king_moved[WHITE] = 0;
}

pos_t piece_moves[24];
uint8_t piece_moves_count;

#define check_in(x, y) (x < 8 && y < 8)  // >= 0 check not necessary because unsigned
#define check_in_s(x, y) (x >= 0 && x < 8 && y >= 0 && y < 8)

#define add_move(x, y) piece_moves[piece_moves_count++] = (pos_t) { x, y }  // do {piece_moves[piece_moves_count++] = (pos_t) { x, y }; dbg_printf("%d: %d, %d\n", __LINE__, x - 1, y - 1);} while (0)
#define add_move_in(x, y) \
    if (check_in(x, y)) \
        add_move(x, y)
#define add_move_if_piece(x, y, color_) \
    if (check_in(x, y) && (BOARD(x, y).type != NONE && BOARD(x, y).color != color_)) \
        add_move(x, y)
#define add_move_if_no_piece(x, y) \
    if (check_in(x, y) && BOARD(x, y).type == NONE) \
        add_move(x, y)
#define add_move_if_possible(x, y, color_) \
    if (check_in(x, y) && (BOARD(x, y).type == NONE || BOARD(x, y).color != color_)) \
        add_move(x, y)

void get_pawn_moves(piece_t piece, uint8_t x, uint8_t y)
{
    if (piece.color == WHITE)
    {
        add_move_if_piece(x - 1, y - 1, WHITE);
        add_move_if_piece(x + 1, y - 1, WHITE);
    }
    else
    {
        add_move_if_piece(x - 1, y + 1, BLACK);
        add_move_if_piece(x + 1, y + 1, BLACK);
    }
    // TODO: en passant (maybe not here)
    
    if (piece.color == WHITE)
    {
        if (check_in(x, y - 1) && BOARD(x, y - 1).type == NONE)
        {
            add_move(x, y - 1);
            if (y == 6)
                add_move_if_no_piece(x, y - 2);
        }
    }
    else
    {
        if (check_in(x, y + 1) && BOARD(x, y + 1).type == NONE)
        {
            add_move(x, y + 1);
            if (y == 1)
                add_move_if_no_piece(x, y + 2);
        }
    }
}

void get_knight_moves(piece_t piece, uint8_t x, uint8_t y)
{
    add_move_if_possible(x - 1, y - 2, piece.color);
    add_move_if_possible(x - 1, y + 2, piece.color);
    add_move_if_possible(x + 1, y - 2, piece.color);
    add_move_if_possible(x + 1, y + 2, piece.color);
    add_move_if_possible(x - 2, y - 1, piece.color);
    add_move_if_possible(x + 2, y - 1, piece.color);
    add_move_if_possible(x - 2, y + 1, piece.color);
    add_move_if_possible(x + 2, y + 1, piece.color);
}

void add_move_ray(uint8_t x, uint8_t y, int8_t kx, int8_t ky, color_t color)
{
    for (int8_t i = 1; i < 8; i++)
    {
        if (!check_in_s(x + kx * i, y + ky * i) || ((BOARD(x + kx * i, y + ky * i).type != NONE && BOARD(x + kx * i, y + ky * i).color == color)))
            break;

        add_move(x + kx * i, y + ky * i);

        if (BOARD(x + kx * i, y + ky * i).type != NONE)  // && BOARD(x + kx * i, y + ky * i).color != color)
            break;
    }
}

void get_bishop_moves(piece_t piece, uint8_t x, uint8_t y)
{
    add_move_ray(x, y, -1, -1, piece.color);
    add_move_ray(x, y,  1, -1, piece.color);
    add_move_ray(x, y, -1,  1, piece.color);
    add_move_ray(x, y,  1,  1, piece.color);
}

void get_rook_moves(piece_t piece, uint8_t x, uint8_t y)
{
    add_move_ray(x, y, -1,  0, piece.color);
    add_move_ray(x, y,  0, -1, piece.color);
    add_move_ray(x, y,  1,  0, piece.color);
    add_move_ray(x, y,  0,  1, piece.color);
}

void get_queen_moves(piece_t piece, uint8_t x, uint8_t y)
{
    for (int8_t kx = -1; kx < 2; kx++)
    {
        for (int8_t ky = -1; ky < 2; ky++)
        {
            add_move_ray(x, y, kx, ky, piece.color);
        }
    }
}
void get_king_moves(piece_t piece, uint8_t x, uint8_t y)
{
    for (int8_t kx = -1; kx < 2; kx++)
    {
        for (int8_t ky = -1; ky < 2; ky++)
        {
            add_move_if_possible(x+kx, y+ky, piece.color);
        }
    }

    if (!king_moved[piece.color])
    {
        if (BOARD(5, y).type == NONE && BOARD(6, y).type == NONE)
            add_move(6, y);
    }
}

void get_piece_moves(piece_t piece, uint8_t x, uint8_t y)
{
    piece_moves_count = 0;

    // dbg_printf("get_piece_moves({%d, %d}, %d, %d)\n", piece.type, piece.color, x, y);

    switch (piece.type)
    {
    case PAWN:
        get_pawn_moves(piece, x, y);
        break;
    case KNIGHT:
        get_knight_moves(piece, x, y);
        break;
    case BISHOP:
        get_bishop_moves(piece, x, y);
        break;
    case ROOK:
        get_rook_moves(piece, x, y);
        break;
    case QUEEN:
        get_queen_moves(piece, x, y);
        break;
    case KING:
        get_king_moves(piece, x, y);
        break;
    default:
        break;
    }

    // // Print moves
    // for (uint8_t i = 0; i < piece_moves_count; i++)
    // {
    //     pos_t move = piece_moves[i];
    //     dbg_printf("move %d: {%d, %d}\n", i, move.x, move.y);
    // }
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

bool is_any_threatened(pos_t *pos, uint8_t pos_count, color_t color)
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            piece_t piece = BOARD(x, y);

            if (piece.type != NONE && piece.color != color)
            {
                // Check if the piece threaten the sqare
                get_piece_moves(piece, x, y);

                for (uint8_t i = 0; i < piece_moves_count; i++)
                {
                    for (uint8_t j = 0; j < pos_count; j++)
                    {
                        if (piece_moves[i].x == pos[j].x && piece_moves[i].y == pos[j].y)
                            return true;
                    }
                }
            }
        }
    }

    return false;
}

bool is_threatened(pos_t pos, color_t color)
{
    return is_any_threatened(&pos, 1, color);
}

bool is_in_check(color_t color)
{
    pos_t king = find_piece(KING, color);

    return is_threatened(king, color);
}

void do_move(uint8_t x, uint8_t y, pos_t move)
{
    BOARD_POS(move) = BOARD(x, y);
    BOARD(x, y).type = NONE;

    if (BOARD_POS(move).type == KING
        && !king_moved[BOARD_POS(move).color]
        && move.x == 6
        && BOARD(7, y).type == ROOK)
    {  // Castling
        BOARD(5, y) = BOARD(7, y);
        BOARD(7, y).type = NONE;
    }
}

// bool try_move(piece_t piece, uint8_t x, uint8_t y, pos_t move)
// {
//     piece_t old_board[8 * 8];

//     memcpy(old_board, board, sizeof(old_board));

//     do_move(piece, x, y, move);

//     if (!is_in_check(piece.color))
//         return true;
    
//     memcpy(board, old_board, sizeof(board));

//     return false;
// }

bool is_move_valid(uint8_t x, uint8_t y, pos_t move)
{
    if (BOARD(x, y).type == KING && !king_moved[BOARD(x, y).color] && move.x == 6)
    {  // Castling
        return !is_any_threatened((pos_t []){{4, y}, {5, y}, {6, y}}, 3, BOARD(x, y).color);
    }

    piece_t old_board[8 * 8];
    bool res;

    memcpy(old_board, board, sizeof(old_board));

    do_move(x, y, move);

    res = !is_in_check(BOARD(x, y).color);

    memcpy(board, old_board, sizeof(board));

    return res;
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

void draw_check(uint8_t x, uint8_t y)
{
    gfx_sprite_t *sprite = chess_tiles[17];

    gfx_TransparentSprite(sprite, OF_X + x * TILE_W, OF_Y + y * TILE_H);
}

bool potential_moves[8*8];

void draw_board()
{
    gfx_FillScreen(1); // White

    gfx_SetColor(2); // Grey

    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            if ((x + y) % 2)
            {
                gfx_FillRectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
            }
            if (BOARD(x, y).type != NONE)
            {
                draw_piece(BOARD(x, y), x, y);
            }
            if (in_check && BOARD(x, y).type == KING)
            {
                draw_check(x, y);
            }
            if (potential_moves[POS_XY(x, y)])
            {
                draw_potential(circle_size, (x + y) % 2, x, y);
            }
            if (selected.x == x && selected.y == y)
            {
                gfx_SetColor(3); // Red
                gfx_Rectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
                gfx_Rectangle(OF_X + x * TILE_W + 1, OF_Y + y * TILE_H + 1, TILE_W - 2, TILE_H - 2);
                gfx_SetColor(2); // Grey
            }
            else if (cursor.x == x && cursor.y == y)
            {
                gfx_SetColor(4); // Blue
                gfx_Rectangle(OF_X + x * TILE_W, OF_Y + y * TILE_H, TILE_W, TILE_H);
                gfx_Rectangle(OF_X + x * TILE_W + 1, OF_Y + y * TILE_H + 1, TILE_W - 2, TILE_H - 2);
                gfx_SetColor(2); // Grey
            }
        }
    }
}

bool played_animation = false;

void end_turn()
{
    if (cursor_piece.type == KING)
    {
        king_moved[turn] = true;
    }
    turn = !turn;
    in_check = is_in_check(turn);
    played_animation = true;
}

// Called when enter is pressed
void select()
{
    if (!is_selected) {
        if (cursor_piece.type == NONE || cursor_piece.color != turn)
            return;

        selected = cursor;
        redraw = true;
        return;
    }

    if (cursor.x == selected.x && cursor.y == selected.y)
    {
        unselect();
        redraw = true;
        return;
    }

    if (potential_moves[POS(cursor)])
    {
        do_move(selected.x, selected.y, cursor);
        end_turn();
    }
    else
    {
        redraw = true;  // Do set redraw if played to not cancel played_animation, will be set later anyway
    }
    unselect();
}

void calc_potential_moves()
{
    if (is_selected)
        get_piece_moves(selected_piece, selected.x, selected.y);
    else if (cursor_piece.type != NONE && cursor_piece.color == turn)
        get_piece_moves(cursor_piece, cursor.x, cursor.y);
    else
        piece_moves_count = 0;

    // if (piece_moves_count == 0)  // animation to remove circles when possible (no other circles to draw)
    // {                            // causes display bugs then crash (severe memory corruption)
    //     played_animation = true;
    //     return;
    // }
    
    for (uint8_t i = 0; i < 8 * 8; i++)
        potential_moves[i] = false;

    if (piece_moves_count == 0)
    {
        return;
    }

    uint8_t piece_moves_count_temp = piece_moves_count;
    pos_t piece_moves_temp[piece_moves_count_temp];

    memcpy(piece_moves_temp, piece_moves, sizeof(pos_t) * piece_moves_count_temp);

    for (uint8_t i = 0; i < piece_moves_count_temp; i++)
    {
        pos_t move = piece_moves_temp[i];
        if (is_selected)
        {
            if (!is_move_valid(selected.x, selected.y, move))
                continue;
        }
        else
        {
            if (!is_move_valid(cursor.x, cursor.y, move))
                continue;
        }

        potential_moves[POS(move)] = true;
    }
}

// screens.h functions

void begin_game()
{
    init_game();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
}

void step_played_animation()
{
    if (circle_size == 3)
        potential_moves[POS(cursor)] = false;

    circle_size--;

    if (circle_size == 0)
    {
        played_animation = false;

        for (uint8_t i = 0; i < 8 * 8; i++)
            potential_moves[i] = false;
    }

    redraw = true;
}

void step_game()
{
    input_event_t event;
    pos_t old_s = selected;

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
                if (cursor.y > 0)
                {
                    cursor.y--;
                    redraw = true;
                }
                break;
            case kb_Down:
                if (cursor.y < 7)
                {
                    cursor.y++;
                    redraw = true;
                }
                break;
            case kb_Left:
                if (cursor.x > 0)
                {
                    cursor.x--;
                    redraw = true;
                }
                break;
            case kb_Right:
                if (cursor.x < 7)
                {
                    cursor.x++;
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

    if (played_animation)
    {
        if (!redraw)
        {
            step_played_animation();
            return;
        }
        played_animation = false;
    }

    if (redraw
        && (old_s.x != selected.x || old_s.y != selected.y || is_selected == false)
        && (cursor.x != selected.x || cursor.y != selected.y)
        && (cursor.x != old_s.x || cursor.y != old_s.y))
    {
        calc_potential_moves();
        circle_size = ticks % 2;
    }
    else if (circle_size < 3)
    {
        circle_size+=2;
        redraw = true;
        if (circle_size > 3)
            circle_size = 3;
    }
}

void draw_game()
{
    // gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    // gfx_SetTransparentColor(0);

    draw_board();
}

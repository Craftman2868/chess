#ifndef CHESS_H
#define CHESS_H

#include <stdint.h>
#include <stdbool.h>

enum {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
} typedef piece_type_t;

enum {
    BLACK,
    WHITE,
} typedef color_t;

extern color_t turn;

#define winner (!turn)

struct {
    piece_type_t type;
    color_t color;
} typedef piece_t;

struct {
    uint8_t x, y;
} typedef pos_t;

enum {
    RUNNING,
    CHECKMATE,
    DRAW_STALEMATE,
    DRAW_MATERIAL,
} typedef game_state_t;

extern game_state_t game_state;

bool run_new_game();
bool run_save(int8_t id);

#endif /* CHESS_H */

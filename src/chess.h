#ifndef CHESS_H
#define CHESS_H

#include <stdint.h>

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
    DRAW,
} typedef game_state_t;

#endif /* CHESS_H */

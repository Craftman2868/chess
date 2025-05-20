#ifndef CHESS_H
#define CHESS_H

enum {
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


extern piece_t board[8][8];

#endif /* CHESS_H */

#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>
#include <stdint.h>
#include "chess.h"


#define MAX_SAVE_ID 9  // < 10
#define MAX_SAVE_COUNT (MAX_SAVE_ID + 1)

struct __attribute__((packed)) {
    piece_t board[8 * 8];
    color_t turn:1;
    uint8_t king_moved:2;
    uint8_t rook_moved:6;
    uint8_t en_passant_x;  // 9 means no en passant (WARNING: do not use 8 as the unused value)
} typedef game_data_t;

game_data_t dump_game(piece_t board[], color_t turn, bool king_moved[], bool rook_moved[2][2], uint8_t en_passant_x);
void load_game(game_data_t *data, piece_t board[], color_t *turn, bool king_moved[], bool rook_moved[2][2], uint8_t *en_passant_x);

bool save_exists(int8_t id);

int8_t get_new_id();
int8_t next_save(int8_t id);

bool save_data(game_data_t *data, int8_t id);
bool load_data(game_data_t *data, int8_t id);

bool delete_save(int8_t id);

#endif /* SAVE_H */

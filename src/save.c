#include "save.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>  // memcpy
#include <fileioc.h>
#include "chess.h"


game_data_t dump_game(piece_t board[], color_t turn, bool king_moved[], bool rook_moved[2][2], uint8_t en_passant_x)
{
    game_data_t data;

    memcpy(data.board, board, sizeof(piece_t) * 8 * 8);
    data.turn = turn;

    data.king_moved |= king_moved[BLACK] << 0;
    data.king_moved |= king_moved[WHITE] << 1;
    data.rook_moved |= rook_moved[BLACK][0] << 0;
    data.rook_moved |= rook_moved[BLACK][1] << 1;
    data.rook_moved |= rook_moved[WHITE][0] << 2;
    data.rook_moved |= rook_moved[WHITE][1] << 3;

    data.en_passant_x = en_passant_x;

    return data;
}

void load_game(game_data_t *data, piece_t board[], color_t *turn, bool king_moved[], bool rook_moved[2][2], uint8_t *en_passant_x)
{
    memcpy(board, data->board, sizeof(piece_t) * 8 * 8);
    *turn = data->turn;

    data->king_moved |= king_moved[BLACK] << 0;
    data->king_moved |= king_moved[WHITE] << 1;
    data->rook_moved |= rook_moved[BLACK][0] << 0;
    data->rook_moved |= rook_moved[BLACK][1] << 1;
    data->rook_moved |= rook_moved[WHITE][0] << 2;
    data->rook_moved |= rook_moved[WHITE][1] << 3;

    king_moved[BLACK] = (data->king_moved >> 0) & 1;
    king_moved[WHITE] = (data->king_moved >> 1) & 1;
    rook_moved[BLACK][0] = (data->rook_moved >> 0) & 1;
    rook_moved[BLACK][1] = (data->rook_moved >> 1) & 1;
    rook_moved[WHITE][0] = (data->rook_moved >> 2) & 1;
    rook_moved[WHITE][1] = (data->rook_moved >> 3) & 1;

    *en_passant_x = data->en_passant_x;
}

char *make_name(int8_t id)
{
    static char name[] = "CHESS0";

    name[5] = '0' + id;

    return name;
}

bool save_exists(int8_t id)
{
    if (id < 0 || id > MAX_SAVE_ID)
        return false;

    int8_t f = ti_Open(make_name(id), "r");

    if (f == 0)
        return false;

    ti_Close(f);

    return true;
}

int8_t get_new_id()
{
    int8_t id = 0;

    while (save_exists(id))
        id++;

    if (id <= MAX_SAVE_ID)
        return id;

    return -1;
}

int8_t next_save(int8_t id)
{
    while (!save_exists(++id))
        if (id > MAX_SAVE_ID)
            return -1;

    return id;
}

bool save_data(game_data_t *data, int8_t id)
{
    int8_t f = ti_Open(make_name(id), "w");

    if (f == 0)
        return false;

    if (ti_Write(data, sizeof(game_data_t), 1, f) != 1)
    {
        ti_Close(f);
        return false;
    }

    if (ti_Close(f) == 0)
        return false;

    return true;
}

bool load_data(game_data_t *data, int8_t id)
{
    int8_t f = ti_Open(make_name(id), "r");

    if (f == 0)
        return false;

    if (ti_Read(data, sizeof(game_data_t), 1, f) != 1)
    {
        ti_Close(f);
        return false;
    }

    if (ti_Close(f) == 0)
        return false;

    return true;
}

bool delete_save(int8_t id)
{
    return ti_Delete(make_name(id)) != 0;
}

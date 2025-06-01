#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

bool run_new_game();
bool run_save(int8_t id);

extern int8_t game_id;

#endif /* GAME_H */

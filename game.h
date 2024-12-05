#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "food.h"
#include "vector.h"
#include <stdbool.h>
#include <curses.h>

typedef struct
{
  Snake game_snake;
  Food game_food;
  int score;
  bool is_running;
  int screen_width;
  int screen_height;
} Game;

// Membuat permainan baru
Game game_create(int screen_width, int screen_height);

// Memperbarui logika permainan
void game_update(Game *game);

// Memeriksa apakah permainan berakhir
bool is_game_over(Game *game);

// Me-restart permainan
void game_restart(Game *game);

// Menangani input pemain
void game_handle_input(Game *game, Vector2 new_dir);

#endif
// Nama Program : game.c
// Deskripsi    : Implementasi logika utama permainan,
//                bertujuan mengelola siklus dan aturan dasar permainan
// Pembuat      : Ikhsan Satriadi / 241511080
//                Rizky Satria Gunawan / 241511089

#include "game.h"

// Fungsi untuk membuat objek permainan baru
// screen_width: parameter input passing by value, tipe int, lebar layar
// screen_height: parameter input passing by value, tipe int, tinggi layar
// game_data: parameter input passing by reference, tipe GameData, data permainan
// Mengembalikan objek permainan baru bertipe Game
Game game_create(int screen_width, int screen_height, GameData *game_data)
{
  // Kamus data
  Game new_game; // Objek permainan baru

  // Inisialisasi dimensi layar
  new_game.screen_width = screen_width;
  new_game.screen_height = screen_height;

  // Buat ular di tengah layar
  new_game.game_snake = create_snake(screen_width / 2, screen_height / 2);

  // Buat makanan
  new_game.game_food = create_food(screen_width, screen_height, &new_game.game_snake);

  // Atur status awal
  new_game.current_score.score = 0;
  new_game.current_score.level = game_data->settings.level;
  new_game.hi_score = game_data->hi_score; // ambil dari game data
  new_game.settings = game_data->settings; // ambil dari game data
  new_game.is_running = true;
  new_game.is_pause = false;
  new_game.is_winning = false;

  return new_game;
}

// Prosedur untuk mengubah hi_score di game dan game_data
// game: parameter output passing by reference, tipe Game*, menunjuk ke objek permainan yang akan diperbarui
// game_data: parameter output passing by reference, tipe GameData*, menunjuk ke objek data permainan yang akan diperbarui
// current_score: parameter input passing by value, tipe Score
void set_game_hi_score(Game *game, GameData *game_data, Score current_score)
{ // Cek apakah skor saat ini melebihi hi-score
  if (current_score.score > game_data->hi_score.score)
  {
    game_data->hi_score = current_score; // perbarui hi-score di game data
    game->hi_score = current_score;      // perbarui hi-score di game
  }
}

// Prosedur untuk memperbarui leaderboard
// game: parameter output passing by reference, tipe Game*, menunjuk ke objek permainan yang akan diperbarui
// game_data: parameter output passing by reference, tipe GameData*, menunjuk ke objek data permainan yang akan diperbarui
// current_score: parameter input passing by value, tipe Score
// rank_index: parameter input passing by value, tipe int, lokasi index di leaderboard untuk skor baru
void update_leaderboard(Game *game, GameData *game_data, Score current_score, int rank_index)
{
  // Cek apakah rank_index valid
  if (rank_index < 0 || rank_index >= MAX_LEADERBOARD_SIZE)
    return;

  // Geser skor di leaderboard ke bawah dari rank_index
  for (int i = 9; i > rank_index; i--)
  {
    game_data->leaderboard[i] = game_data->leaderboard[i - 1]; // Geser skor
  }

  // Masukkan current_score ke leaderboard pada rank_index
  game_data->leaderboard[rank_index] = current_score;

  // Perbarui hi-score
  set_game_hi_score(game, game_data, game_data->leaderboard[0]);
}

// Fungsi untuk mengecek apakah skor dapat masuk ke leaderboard
// game_data: parameter output passing by reference, tipe GameData*, menunjuk ke objek data permainan
// current_score: parameter input passing by value, tipe Score, skor saat ini
// Mengembalikan index peringkat jika masuk leaderboard , -1 jika tidak masuk leaderboard
int in_leaderboard(GameData *game_data, Score current_score)
{
  // Cek setiap skor di leaderboard
  for (int i = 0; i < MAX_LEADERBOARD_SIZE; i++)
  {
    // Jika current_score lebih besar dari skor di leaderboard
    if (current_score.score > game_data->leaderboard[i].score)
    {
      return i; // Kembalikan indeks di mana current_score dapat dimasukkan
    }
  }
  // Jika tidak ada skor yang lebih kecil, kembalikan -1
  return -1;
}

// Prosedur untuk memperbarui logika permainan
// game: parameter input/output passing by reference, tipe Game*, menunjuk ke objek permainan yang akan diperbarui
void game_update(Game *game)
{
  // Gerakkan ular
  snake_move(&game->game_snake);

  // Periksa tabrakan dengan diri sendiri atau batas layar
  if (is_game_over(game))
  {
    game->is_running = false;
    return;
  }

  //* Cek apakah skor sudah mencapai maksimal skor, jika iya maka game berhenti dan menang
  if (game->current_score.score >= MAX_SNAKE_LENGTH)
  {
    game->is_running = false;
    game->is_winning = true; // menang!
    return;
  }

  // Cek tabrakan dengan makanan
  if (vector2_equals(game->game_food.position, game->game_snake.head))
  {
    // Tambah skor dan panjang ular
    if (snake_grow(&game->game_snake))
    {
      add_score(&game->current_score);
    }

    // Buat makanan baru
    game->game_food = create_food(
        game->screen_width,
        game->screen_height,
        &game->game_snake);
  }
}

// Fungsi untuk memeriksa apakah permainan telah berakhir (ular menabrak tembok atau ular itu sendiri)
// game: parameter input passing by reference, tipe Game*, menunjuk ke objek permainan yang akan diperiksa
// Mengembalikan true jika permainan berakhir, false jika tidak
bool is_game_over(Game *game)
{
  // Kamus
  Snake *snake = &game->game_snake; // Ambil reference snake dari game

  // Cek jika tabrakan dengan ular itu sendiri
  if (snake_collides_with_self(snake))
    return true;

  // Cek jika tabrakan dengan batas layar
  return !vector2_is_in_bounds(
      snake->head,
      game->screen_width,
      game->screen_height);
}

// Prosedur untuk merestart permainan
// game: parameter input/output passing by reference, tipe Game*, menunjuk ke objek permainan yang akan direset
// game_data: parameter output passing by reference, tipe GameData*, menunjuk ke objek data permainan
void game_restart(Game *game, GameData *game_data)
{
  *game = game_create(game->screen_width, game->screen_height, game_data); // assign game dengan game baru
}

// Prosedur untuk menangani input dari pengguna untuk mengubah arah pergerakan ular
// game: parameter input/output passing by reference, tipe Game*, menunjuk ke objek permainan yang akan diperbarui berdasarkan input
// new_dir: parameter input passing by value, tipe Vector2, arah baru untuk ular
void game_handle_input(Game *game, Vector2 new_dir)
{
  snake_change_direction(&game->game_snake, new_dir);
}
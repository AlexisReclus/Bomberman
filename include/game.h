#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new( int level);

//New level
struct game* game_new_level(struct game* game, int level);

// return 1 if the maps is set
int game_isset_map(struct game * game, int level);

// Load a level that is already set
struct game* game_load_level(struct game* game, int level);

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

// Return the first monster of the list
struct monster* game_get_monster_first(struct game* game);

// Set the first monster of the list
void game_set_monster_first(struct game * game, struct monster * monster);

// Set/Resume the pause
int game_init_pause(struct game * game);
void game_resume_pause(struct game * game, int time);

// Display the pause image
void game_display_pause(struct game* game);

int input_keyboard(struct game* game);

#endif /* GAME_H_ */

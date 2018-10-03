#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number,int life, int keys_number, int bomb_scope);
void   player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player* player, enum direction way);

// Get, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Get, Increase, Decrease the scope of bomb
int  player_get_bomb_scope(struct player * player);
void player_inc_bomb_scope(struct player * player);
void player_dec_bomb_scope(struct player * player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

//Display the bombs
void player_put_bomb(struct map *map, struct player *player);

// Test the number of keys to open the door
void player_open_door(struct map* map, struct player * player, int x, int y);

// See/Increase/Decrease number of key
void player_inc_nb_keys(struct player* player);
int player_get_nb_keys(struct player* player);
void player_dec_nb_keys(struct player* player);

// See/Increase/Decrease number of life
int player_get_nb_life(struct player* player);
void player_inc_nb_life(struct player* player);
void player_dec_nb_life(struct player* player) ;

//Functions for the player's shield
int player_is_invincible(struct player* player);
void player_set_shield(struct player * player, int shield);
int player_get_shield_time(struct player * player);
void player_set_shield_time(struct player * player, int shield_time);
void player_invincible_update(struct player * player);

// Bonus
void player_on_bonus (struct player * player, struct map *map);

#endif /* PLAYER_H_ */

#ifndef BOMB_H_
#define BOMB_H_

#include <game.h>
#include <map.h>

struct bomb;
//free the bombs
void bomb_free(struct bomb* bomb);
void bomb_list_free(struct map * map);

// Set the positions and the time of the bomb
void bomb_set(struct game* game,int pos_x,int pos_y);

// Set the least significant bits, function of time
void bomb_update (struct game * game);

// Launch/Display the explosion of the bomb
void bomb_explosion_display(struct game * game);

// Undisplay the explosion of the bomb on the map
void bomb_explosion_undisplay(struct game * game);

// Return the x-position of the bomb
int bomb_get_x(struct bomb *bomb);

// Return the y-position of the bomb
int bomb_get_y(struct bomb *bomb);

//Increase/decrease the scope of the bombs
void bomb_scope_inc(struct game* game);

// Add the pause time to the time of the bomb
void bomb_add_pause_time(struct game * game,int pause_time);

#endif /* BOMB_H_ */

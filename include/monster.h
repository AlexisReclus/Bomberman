/*
 * monster.h
 *
 *  Created on: 11 avr. 2017
 *      Author: AlexisR
 */

#ifndef SRC_MONSTER_H_
#define SRC_MONSTER_H_

#include <game.h>
#include <map.h>

struct monster;

// Creates a new monster
struct monster* monster_init();

//free the monsters
void monster_free(struct monster* monster);
void monster_list_free(struct game*game);


// Display the monster on the screen
void monster_display(struct game* game);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster* monster, enum direction way);

// Load the monster position from the map
void monster_from_map(struct game* game, struct map* map);

// Set a monster in the list
void monster_set(struct game* game, int pos_x, int pos_y);

//Movement of the monster
void monster_move(struct game* game, struct map* map);

//Random monster's direction
struct monster* monster_direction(struct monster* monster);

//Number random
int rand_a_b(int a, int b);

// Add the pause time to the time of the monsters
void monster_add_pause_time(struct game * game,int pause_time);

// Return 0 if a door is next to the empty cell else 1
int monster_hate_door(struct map * map,int x, int y);

// If monster is on a bomb
void monster_dead(struct game * game, int x, int y);

#endif /* SRC_MONSTER_H_ */

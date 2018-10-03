#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <bomb.h>
#include <monster.h>
#include <game.h>


struct monster {
	int x, y;
	enum direction current_direction;
	int speed;
	int time;
	struct monster * next;
	struct monster * previous;
};


/*struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->current_direction = SOUTH;
	monster->speed=700;
	monster->next=NULL;
	return monster;
}*/

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

void monster_list_free(struct game*game){
	struct monster * tmp = game_get_monster_first(game);
	struct monster * inter;

				if (game_get_monster_first(game)==NULL){
				}
				else {
					while (tmp->next!=NULL){
						inter=tmp;
						tmp=tmp->next;
						monster_free(inter);
					}
					monster_free(tmp);
				}
				return;
}


int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->current_direction = way;
}

void monster_from_map(struct game*game, struct map* map) {

	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
	      monster_set(game,i,j);
	    }
	  }
	}
}

void monster_set(struct game* game, int pos_x, int pos_y){
	struct monster * monster=malloc(sizeof(struct monster));
			monster->x=pos_x;
			monster->y=pos_y;
			monster->current_direction=0;
			monster->speed=1000;
			monster->time=SDL_GetTicks();
			monster->next=NULL;

			struct monster * tmp = game_get_monster_first(game);

			if (game_get_monster_first(game)==NULL){
				monster->previous=NULL;
				game_set_monster_first(game, monster);

			}
			else {
				while (tmp->next!=NULL){
					tmp=tmp->next;
				}
				tmp->next=monster;
				tmp->next->previous=tmp;
			}
}

void monster_display(struct game* game) {
	if (game_get_monster_first(game)==NULL){
			return;
		}

	struct monster * tmp= game_get_monster_first(game);

	while (tmp!=NULL){
		window_display_image(sprite_get_monster(tmp->current_direction),
			tmp->x * SIZE_BLOC, tmp->y * SIZE_BLOC);
			map_set_cell_type(game_get_current_map(game),tmp->x,tmp->y,CELL_MONSTER);
		tmp=tmp->next;
	}
}


int rand_a_b(int a, int b){
    return rand()%(b-a+1)+a;
}


struct monster* monster_direction (struct monster* monster){
	int prob = rand_a_b(0,3);
	switch (prob) {
	case 0 :
		monster->current_direction=NORTH;
		break;
	case 1 :
		monster->current_direction=SOUTH;
		break;
	case 2 :
		monster->current_direction=EAST;
		break;
	default :
		monster->current_direction=WEST;
		break;
	}
	return (monster);
}


static int monster_move_aux(struct player* player, struct monster* monster, struct map* map, int x, int y, enum direction shift) {
	if (!map_is_inside(map, x, y))
		return 0;


	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
        return 0;
		break;

	case CELL_BONUS:
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_PLAYER:
		if (player_is_invincible(player)==0){
				player_dec_nb_life(player);
				player_set_shield(player,1);
				player_set_shield_time(player,SDL_GetTicks());
		}
		return 1;
		break;

	case CELL_BOMB:
		if (map_get_cell_type(map, x, y)==CELL_BOMB_EX){
			return 1;
		}
		else{
			return 0;
		}
	case CELL_KEY:
		return 0;
		break;
	case CELL_DOOR:
		return 0;
		break;
	case CELL_EMPTY:
		return monster_hate_door(map,x,y); //if 1 -> moved if 0 -> a door is next to the empty cell
		break;
	default:
		// Monster has moved
			return 1;
		break;
	}

}

void monster_move(struct game* game,struct map* map) {
	if (game_get_monster_first(game)==NULL){
			return;
	}
	struct player* player = game_get_player(game);

	struct monster * monster = game_get_monster_first(game);

	int speed,x,y,move,time;

	while (monster!=NULL){
		int time_m=SDL_GetTicks();
		x=monster->x;
		y=monster->y;
		speed=monster->speed;
		time=monster->time;
		move=0;

	if ((time_m)>monster->time){
		monster_direction(monster);
	switch (monster->current_direction) {
	case NORTH:
		if (monster_move_aux(player, monster, map, x, y - 1,NORTH)) {
			monster->y--;
			move = 1;
			monster->time+=speed;
		}
		break;

	case SOUTH:
		if (monster_move_aux(player, monster, map, x, y + 1,SOUTH)) {
			monster->y++;
			move = 1;
			monster->time+=speed;
		}
		break;

	case WEST:
		if (monster_move_aux(player, monster, map, x - 1, y,WEST)) {
			monster->x--;
			move = 1;
			monster->time+=speed;
		}
		break;

	case EAST:
		if (monster_move_aux(player,monster, map, x + 1, y,EAST)) {
			monster->x++;
			move = 1;
			monster->time+=speed;
		}
		break;
	}
	}
	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}
	monster=monster->next;
	}
}

void monster_add_pause_time(struct game * game,int pause_time){
	struct monster * tmp = game_get_monster_first(game);

				if (game_get_monster_first(game)==NULL){
				}
				else {
					tmp->time+=pause_time;
					while (tmp->next!=NULL){
						tmp=tmp->next;
						tmp->time+=pause_time;
					}
				}
}

int monster_hate_door(struct map * map,int x, int y){
	enum cell_type up = CELL_EMPTY; // Initialisés à EMPTY pour le 2ème if
	enum cell_type down = CELL_EMPTY; // Initialisés à EMPTY pour le 2ème if
	enum cell_type left = CELL_EMPTY; // Initialisés à EMPTY pour le 2ème if
	enum cell_type right = CELL_EMPTY; // Initialisés à EMPTY pour le 2ème if

	if (map_is_inside(map,x,y+1)){
		up=map_get_cell_type(map,x,y+1);
	}
	if (map_is_inside(map,x,y-1)){
		down=map_get_cell_type(map,x,y-1);
		}
	if (map_is_inside(map,x-1,y)){
		left=map_get_cell_type(map,x-1,y);
		}
	if (map_is_inside(map,x+1,y)){
		right=map_get_cell_type(map,x+1,y);
		}

	if (up!=CELL_DOOR && down!=CELL_DOOR && left!=CELL_DOOR && right!=CELL_DOOR){
		return 1;
	}
	else{
	return 0;
	}
}

void monster_dead(struct game * game, int x, int y){
		struct monster * tmp= game_get_monster_first(game);

		while (tmp!=NULL){
			if (tmp->x==x && tmp->y==y){
				if (tmp->previous==NULL){
					if(tmp->next!=NULL){
						tmp->next->previous=NULL;
						game_set_monster_first(game, tmp->next);
						free(tmp);
						break;
					}
					else{
						game_set_monster_first(game, NULL);
						free(tmp);
						break;
					}

				}
				else{
					if (tmp->next!=NULL){
						tmp->next->previous=tmp->previous;
						tmp->previous->next=tmp->next;
						free(tmp);
						break;
					}
					else{
						tmp->previous->next=NULL;
						free(tmp);
						break;
					}
				}
			}
			else {
				tmp=tmp->next;
			}
		}
}

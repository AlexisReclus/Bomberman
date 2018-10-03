#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <monster.h>

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	int nb_keys;
	int nb_life;
	int bomb_scope;
	int shield;
	int shield_time;
};

struct player* player_init(int bomb_number,int life, int keys_number, int bomb_scope) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;
	player->nb_keys= keys_number;
	player->nb_life=life;
	player->bomb_scope=bomb_scope;
	player->shield=0;
	player->shield_time=-1;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_life(struct player* player) {
	assert(player);
	return player->nb_life;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	if (player->nb_life>0)
		player->nb_life -= 1;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if ((player_get_nb_bomb(player))>0)
		player->nb_bombs -= 1;
}

int player_get_bomb_scope(struct player* player) {
	assert(player);
	return player->bomb_scope;
}

void player_inc_bomb_scope(struct player* player) {
	assert(player);
	player->bomb_scope += 1;
}

void player_dec_bomb_scope(struct player* player) {
	assert(player);
	if ((player_get_bomb_scope(player))>1)
		player->bomb_scope -= 1;
}

int player_get_nb_keys(struct player* player) {
	assert(player);
	return player->nb_keys;
}

void player_inc_nb_keys(struct player* player) {
	assert(player);
	player->nb_keys += 1;
}
void player_dec_nb_keys(struct player* player) {
	assert(player);
		player->nb_keys -= 1;
}

int player_is_invincible(struct player* player){
	assert(player);
	return player->shield; //1 if is invincible else 0
}

void player_set_shield(struct player * player, int shield){
	assert(player);
	player->shield=shield;
}

int player_get_shield_time(struct player * player){
	assert(player);
	return player->shield_time;
}

void player_set_shield_time(struct player * player, int shield_time){
	assert(player);
	player->shield_time=shield_time;
}
void player_invincible_update(struct player * player){
	int time=SDL_GetTicks();
	if (player_is_invincible(player)==1){
		if ((time-player_get_shield_time(player))>3000){
			player_set_shield(player,0);
		}
	}
}


void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
	      player->x = i;
	      player->y = j;
	    }
	  }
	}
}


static int player_move_aux(struct player* player, struct map* map, int x, int y, enum direction shift) {
    int x_shifted=x;
    int y_shifted=y;
    int current=0;
	if (!map_is_inside(map, x, y))
		return 0;


	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if (map_get_scenery_type(map,x,y)==SCENERY_PRINCESS){
			return 11;
		}
		else{
			return 0;
		}
		break;

	case CELL_BOX:
            switch (shift){
                case NORTH:
                    y_shifted=y-1;
                    break;
                case SOUTH:
                    y_shifted=y+1;
                    break;
                case WEST:
                    x_shifted=x-1;
                    break;
                case EAST:
                    x_shifted=x+1;
                    break;
                default:
                    break;
            }
            
            if ((!map_is_inside(map,x_shifted,y_shifted) || map_get_cell_type(map,x_shifted,y_shifted))){
                return 0;
            }
            else{
                map_copy_box(map,x,y,x_shifted,y_shifted);
                return 1;
            }
		break;

	case CELL_BONUS:
		switch (map_get_bonus_type(map,x,y)) {
			case CELL_BONUS_LIFE:
				player_inc_nb_life(player);
				break;

			case CELL_BONUS_RANGEINC:
				player_inc_bomb_scope(player);
				break;

			case CELL_BONUS_RANGEDEC:
				player_dec_bomb_scope(player);
				break;

			case CELL_BONUS_BOMBINC:
				player_inc_nb_bomb(player);
				break;

			case CELL_BONUS_BOMBDEC:
				if ((player_get_nb_bomb(player))>1){
					player_dec_nb_bomb(player);
				}
				break;
			default:
				break;
			}
		break;

	case CELL_MONSTER:
		if (player_is_invincible(player)==0){
			player_dec_nb_life(player);
			player_set_shield(player,1);
			player_set_shield_time(player,SDL_GetTicks());
		}
		return 1;
		break;

	case CELL_PLAYER:
		break;

	case CELL_BOMB:
		if (map_get_bomb_type(map,x,y)==CELL_BOMB_EX){
			if (player_is_invincible(player)==0){
						player_dec_nb_life(player);
						player_set_shield(player,1);
						player_set_shield_time(player,SDL_GetTicks());
			}
			return 1;
		}
		else{
			return 0;
		}
		break;

	case CELL_KEY:
		player_inc_nb_keys(player);
		break;
	case CELL_DOOR:
		switch (map_get_door_type(map,x,y)) {
				case DOOR_OPENED:
					//NIVEAU ASSOCIÉ
					current=((map_get_door_level(map,x,y)/2)+1);
					return current;
					break;

				case DOOR_CLOSED:

					player_open_door(map,player,x,y);
					return 0;
					break;
				default:
						break;
			}

		return 0;
		break;

	default:
		break;
	}

	// Player has moved
	if(current>=2 && current <9){
		return(current);
	}
	else if (current==11){
		return current;
	}
	else{
		return 1;
	}
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;
	int current;

	switch (player->current_direction) {
	case NORTH:
		current=player_move_aux(player, map, x, y - 1,NORTH);
		if (current==1) {
				player->y--;
				move = 1;
		}
		break;

	case SOUTH:
		current=player_move_aux(player, map, x, y + 1,SOUTH);
		if (current==1) {
				player->y++;
				move = 1;
		}
		break;

	case WEST:
		current=player_move_aux(player, map, x - 1, y,WEST);
		if (current==1) {
				player->x--;
				move = 1;
		}
		break;

	case EAST:
		current=player_move_aux(player, map, x + 1, y,EAST);
		if (current==1) {
				player->x++;
				move = 1;
		}
		break;
	}

	if (move) {

		if (map_get_cell_type(map, x, y)!=CELL_BOMB && map_get_cell_type(map,x,y)!=CELL_MONSTER){
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
		}

		else if (map_get_cell_type(map,x,y)==CELL_MONSTER){
					map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
					map_set_cell_type(map, x, y, CELL_MONSTER);
				}
		else { // if this is a CELL_BOMB
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
		}
	}

	if (current>1 && current<9){
		return current;
	}
	if (current==11){ //Fin du jeu -> princesse
		return current;
	}
	else {
		return move;
	}
}



void player_display(struct player* player) {
	assert(player);
	if (player_is_invincible(player)==0){
			window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	}
	else{
		if (SDL_GetTicks() % 2 == 0){

		}
		else{
			window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}
	}
}


void player_open_door(struct map * map, struct player * player,int x, int y){
	assert(player);
	if (player->nb_keys>0){
		player_dec_nb_keys(player);
		map_open_door(map,x,y);
	}
}


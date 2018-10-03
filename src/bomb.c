#include <SDL/SDL.h>
#include <constant.h>
#include <window.h>
#include <misc.h>
#include <sprite.h>
#include <bomb.h>
#include <assert.h>
#include <monster.h>
#include <game.h>

struct bomb {
	int x;
	int y;
	int time;
	int scope;
	struct bomb * next;
};

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	free(bomb);
}

void bomb_list_free(struct map * map){
	struct bomb * tmp = map_get_bomb_first(map);
	struct bomb * inter;
					if (map_get_bomb_first(map)==NULL){
					}
					else {
						while (tmp->next!=NULL){
							inter=tmp;
							tmp=tmp->next;
							bomb_free(inter);
						}
						bomb_free(tmp);
					}
					return;
}

void bomb_set(struct game* game,int pos_x,int pos_y){
		struct map * map = game_get_current_map(game);
		struct bomb * bomb=malloc(sizeof(struct bomb));
		bomb->time=SDL_GetTicks();
		bomb->x=pos_x;
		bomb->y=pos_y;
		bomb->scope=player_get_bomb_scope(game_get_player(game));
		bomb->next=NULL;

		struct bomb * tmp = map_get_bomb_first(map);

		if (map_get_bomb_first(map)==NULL){
			map_set_bomb_first(map, bomb);
		}
		else {
			while (tmp->next!=NULL){
				tmp=tmp->next;
			}
			tmp->next=bomb;
		}
}

void bomb_update (struct game * game){
	struct map * map = game_get_current_map(game);


	if (map_get_bomb_first(map)==NULL){
		return;
	}


	map_set_nb_bomb(map,player_get_nb_bomb(game_get_player(game)));

	int time;
	struct bomb * tmp = map_get_bomb_first(map);

	while (tmp!=NULL){
	time=SDL_GetTicks(); //time take the number of milliseconds since the SDL library initialized

	if ((time>(tmp->time)) && (time<(tmp->time +1000))){
		map_set_bomb_type(game_get_current_map(game),tmp->x,tmp->y,CELL_BOMB_1);
		}
	if ((time>(tmp->time +1000)) && (time<(tmp->time +2000))){
		map_set_bomb_type(game_get_current_map(game),tmp->x,tmp->y,CELL_BOMB_2);
		}
	if ((time>(tmp->time +2000)) && (time<(tmp->time +3000))){
		map_set_bomb_type(game_get_current_map(game),tmp->x,tmp->y,CELL_BOMB_3);
		}
	if ((time>(tmp->time +3000)) && (time<(tmp->time +4000))){
		map_set_bomb_type(game_get_current_map(game),tmp->x,tmp->y,CELL_BOMB_4);
		}
	if (time>(tmp->time +4000)){
		bomb_explosion_display(game);
	}
	if (time>(tmp->time +4500)){
		bomb_explosion_undisplay(game);
		map_set_bomb_first(map, tmp->next);
		player_inc_nb_bomb(game_get_player(game));
	}
	tmp = tmp->next;

	}
}

void bomb_explosion_display(struct game * game){
	struct map * map = game_get_current_map(game);
	struct bomb * bomb = map_get_bomb_first(map);
	int scope = bomb->scope;
	struct player * player = game_get_player(game);

	map_set_bomb_type(game_get_current_map(game),bomb->x,bomb->y,CELL_BOMB_EX);

	int i=1;
			while (i<= scope && map_is_inside(map,bomb->x+i,bomb->y)){ //a droite
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_BOMB){
					break;
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_EMPTY){
					map_set_bomb_type(map,bomb->x+i,bomb->y,CELL_BOMB_EX);
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_SCENERY){
					break;
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_KEY){
					break;
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_PLAYER){
					map_set_bomb_type(map,bomb->x+i,bomb->y,CELL_BOMB_EX);
					if (player_is_invincible(player)==0){
									player_dec_nb_life(player);
									player_set_shield(player,1);
									player_set_shield_time(player,SDL_GetTicks());
							}
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_MONSTER){
					map_set_bomb_type(map,bomb->x+i,bomb->y,CELL_BOMB_EX);
					monster_dead(game,bomb->x+i, bomb->y);
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_BONUS){
					break;
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_BOX){
					map_set_cell_type(map,bomb->x+i,bomb->y,(0x50 | (map_get_bonus_type(map,bomb->x+i,bomb->y) & 0x0f)));
					break;
				}
				if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_DOOR){
					break;
				}
			i++;
			}
		i=1;
		while (i<= scope && map_is_inside(map,bomb->x-i,bomb->y)){ //a gauche
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_BOMB){
				break;
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_KEY){
				break;
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_EMPTY){
				map_set_bomb_type(map,bomb->x-i,bomb->y,CELL_BOMB_EX);
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_SCENERY){
				break;
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_PLAYER){
				map_set_bomb_type(map,bomb->x-i,bomb->y,CELL_BOMB_EX);
				if (player_is_invincible(player)==0){
								player_dec_nb_life(player);
								player_set_shield(player,1);
								player_set_shield_time(player,SDL_GetTicks());
						}
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_MONSTER){
				map_set_bomb_type(map,bomb->x-i,bomb->y,CELL_BOMB_EX);
				monster_dead(game,bomb->x-i, bomb->y);
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_BONUS){
				break;
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_BOX){
				map_set_cell_type(map,bomb->x-i,bomb->y,(0x50 | (map_get_bonus_type(map,bomb->x-i,bomb->y) & 0x0f)));
				break;
			}
			if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_DOOR){
				break;
			}
			i++;
		}
			i=1;
		while (i<= scope && map_is_inside(map,bomb->x,bomb->y-i)){ // en bas
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_BOMB){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_KEY){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_EMPTY){
				map_set_bomb_type(map,bomb->x,bomb->y-i,CELL_BOMB_EX);
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_SCENERY){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_PLAYER){
				map_set_bomb_type(map,bomb->x,bomb->y-i,CELL_BOMB_EX);
				if (player_is_invincible(player)==0){
								player_dec_nb_life(player);
								player_set_shield(player,1);
								player_set_shield_time(player,SDL_GetTicks());
						}
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_MONSTER){
				map_set_bomb_type(map,bomb->x,bomb->y-i,CELL_BOMB_EX);
				monster_dead(game,bomb->x, bomb->y-i);
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_BONUS){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_BOX){
				map_set_cell_type(map,bomb->x,bomb->y-i,(0x50 | ((map_get_bonus_type(map,bomb->x,bomb->y-i)) & 0x0f)));
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_DOOR){
				break;
			}
			i++;
		}

		i=1;
		while (i<= scope && map_is_inside(map,bomb->x,bomb->y+i)){ // en haut
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_BOMB){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_KEY){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_EMPTY){
				map_set_bomb_type(map,bomb->x,bomb->y+i,CELL_BOMB_EX);
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_SCENERY){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_PLAYER){
				map_set_bomb_type(map,bomb->x,bomb->y+i,CELL_BOMB_EX);
				if (player_is_invincible(player)==0){
								player_dec_nb_life(player);
								player_set_shield(player,1);
								player_set_shield_time(player,SDL_GetTicks());
						}
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_MONSTER){
				map_set_bomb_type(map,bomb->x,bomb->y+i,CELL_BOMB_EX);
				monster_dead(game,bomb->x, bomb->y+i);
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_BONUS){
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_BOX){
				map_set_cell_type(map,bomb->x,bomb->y+i,(0x50 | ((map_get_bonus_type(map,bomb->x,bomb->y+i)) & 0x0f)));
				break;
			}
			if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_DOOR){
				break;
			}
			i++;
		}
}


void bomb_explosion_undisplay(struct game * game){
	struct map * map = game_get_current_map(game);
	struct bomb * bomb = map_get_bomb_first(map);
	int scope = bomb->scope;

	map_set_cell_type(map,bomb->x,bomb->y, CELL_EMPTY);

	int i=1;
				while (i<= scope && map_is_inside(map,bomb->x+i,bomb->y)){ //a droite

					if (map_get_bomb_type(map,bomb->x+i, bomb->y)==CELL_BOMB_EX){
						map_set_cell_type(map,bomb->x+i,bomb->y, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_EMPTY){
					}

					if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_SCENERY){
						break;
					}
					if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_PLAYER){
						map_set_cell_type(map,bomb->x+i,bomb->y, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_BOX){
						break;
					}
					if (map_get_cell_type(map,bomb->x+i, bomb->y)==CELL_MONSTER){
						break;
					}
				i++;
				}

				i=1;
				while (i<= scope && map_is_inside(map,bomb->x-i,bomb->y)){ //a gauche

					if (map_get_bomb_type(map,bomb->x-i, bomb->y)==CELL_BOMB_EX){
						map_set_cell_type(map,bomb->x-i,bomb->y, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_EMPTY){
					}

					if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_SCENERY){
						break;
					}
					if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_PLAYER){
						map_set_cell_type(map,bomb->x-i,bomb->y, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_BOX){
						break;
					}
					if (map_get_cell_type(map,bomb->x-i, bomb->y)==CELL_MONSTER){
						break;
					}
					i++;
				}

				i=1;
				while (i<= scope && map_is_inside(map,bomb->x,bomb->y-i)){ // en bas

					if (map_get_bomb_type(map,bomb->x, bomb->y-i)==CELL_BOMB_EX){
						map_set_cell_type(map,bomb->x,bomb->y-i, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_EMPTY){
					}

					if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_SCENERY){
						break;
					}
					if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_PLAYER){
						map_set_cell_type(map,bomb->x,bomb->y-i, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_BOX){
						break;
					}
					if (map_get_cell_type(map,bomb->x, bomb->y-i)==CELL_MONSTER){
						break;;
					}
					i++;
				}
				i=1;
				while (i<= scope && map_is_inside(map,bomb->x,bomb->y+i)){ // en bas

					if (map_get_bomb_type(map,bomb->x, bomb->y+i)==CELL_BOMB_EX){
						map_set_cell_type(map,bomb->x,bomb->y+i, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_EMPTY){
					}

					if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_SCENERY){
						break;
					}
					if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_PLAYER){
						map_set_cell_type(map,bomb->x,bomb->y+i, CELL_EMPTY);
					}
					if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_BOX){
						break;
					}
					if (map_get_cell_type(map,bomb->x, bomb->y+i)==CELL_MONSTER){
						break;
					}
					i++;
				}
}
int bomb_get_x(struct bomb *bomb){
	return bomb->x;
}

int bomb_get_y(struct bomb *bomb){
	return bomb->y;
}

void bomb_add_pause_time(struct game * game,int pause_time){
	struct map * map = game_get_current_map(game);
	struct bomb * tmp = map_get_bomb_first(map);

			if (map_get_bomb_first(map)==NULL){
			}
			else {
				tmp->time+=pause_time;
				while (tmp->next!=NULL){
					tmp=tmp->next;
					tmp->time+=pause_time;
				}
			}
}


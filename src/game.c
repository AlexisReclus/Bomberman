#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>
#include <map.h>

struct game {
	struct map** maps;       // the game's map
	short current;
	struct player* player;
	struct monster* monster_first;   //Monster of the game
	int nb_bomb;
	int init_pause;
};

struct game* game_new(int level) {
	sprite_load(); // load sprites into process memory
	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct map*)*1);
	game->current = level;

	for (int i=0;i<8;i++){
		game->maps[i]=NULL;
	}

	game->maps[game->current] = map_get_level(game->current);
	map_set_nb_bomb(game_get_current_map(game),1);
	game->monster_first=NULL;
	game->player = player_init(1,2,0,1);
	game->init_pause=0;


	player_from_map(game->player, game->maps[game->current]); // get x,y of the player on the first map
	monster_from_map(game, game->maps[game->current]); // get x,y of the monsters
	return game;
}

struct game* game_new_level(struct game* game, int level) {
	game->current = level;
	game->monster_first=NULL;
	game->maps[game->current] = malloc(sizeof(struct map*));
	game->maps[game->current]=map_get_level(game->current);

	map_set_nb_bomb(game_get_current_map(game),player_get_nb_bomb(game_get_player(game)));

	player_from_map(game->player, game->maps[game->current]); // get x,y of the player on the first map
	monster_from_map(game,game->maps[game->current]);//get x,y of the monster

	return game;
}

int game_isset_map(struct game * game, int level){

	if (game->maps[level]==NULL){// tester si la map n'est pas deja chargée
		return 0;
	}
	else {
		return 1;
	}
}
struct game* game_load_level(struct game* game, int level) {

	game->monster_first=NULL;
	game->current = level;
	player_from_map(game->player, game->maps[game->current]); // get x,y of the player on the first map
	monster_from_map(game, game->maps[game->current]); // get x,y of the monsters

	return game;
}

void game_free(struct game* game) {
	assert(game);
	player_free(game->player);
	for (int i = 0; i < 7; i++){
	//if (game->maps[i]!=NULL){
		map_free(game->maps[i]);
	//}
	}
	//monster_list_free(game);
	//sprite_free();
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}


void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10 * SIZE_BLOC)/6;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_scope(game_get_player(game))), x, y);

	x=4* white_bloc + 6*SIZE_BLOC;
	window_display_image(sprite_get_banner_key(),x,y);

	x=4* white_bloc + 7*SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_keys(game_get_player(game))),x,y);

	x=5* white_bloc + 8*SIZE_BLOC;
	window_display_image(sprite_get_banner_flag(),x,y);

	x=5* white_bloc + 9*SIZE_BLOC;
	window_display_image(sprite_get_number(game->current),x,y);
}

void game_display(struct game* game) {
	assert(game);
int largeur = map_get_width(game->maps[game->current]);
int hauteur = map_get_height(game->maps[game->current]);

/* create the window's dimensions depending on the map's dimension*/
		if (SIZE_BLOC*largeur<10*SIZE_BLOC){
		window_create(10*SIZE_BLOC,
			SIZE_BLOC * hauteur + BANNER_HEIGHT + LINE_HEIGHT);
		}
		else{
			window_create(SIZE_BLOC * largeur,
					SIZE_BLOC * hauteur + BANNER_HEIGHT + LINE_HEIGHT);
		}

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	monster_move(game,game->maps[game->current]);
	monster_display(game);
	bomb_update(game);
	player_invincible_update(game->player);
	window_refresh();
}

int input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int nb_bomb=player_get_nb_bomb(game_get_player(game));
	int current;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				current=player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				current=player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				current=player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				current=player_move(player, map);
				break;
			case SDLK_SPACE:
				if (nb_bomb>0){
					map_set_cell_type(map,player_get_x(player),player_get_y(player), CELL_BOMB);
					bomb_set(game,player_get_x(player),player_get_y(player));
					player_dec_nb_bomb(player);
				}
				break;
			case SDLK_p:
				return 10;
				break;
			default:
				break;
			}

			break;
		}
	}

	if(current>=2 & current<9){
		return(current);
	}
	if (current==11){
		return current;
	}
	else {
		return 0;
	}
}

int game_update(struct game* game) {
	int live = player_get_nb_life(game->player); //If no life,end of the game

	int current;
	current=input_keyboard(game);

	if (2<=current & current<9){ // return the level
		return current;
	}
	else if (current==10){ // set the pause
			return (game_init_pause(game));// return 0 if player press 'p' 1 if ESCAPE or QUIT
		}
	else if (current==11){
		return current;
	}
	else if (current!=0 || live==0){
		return 1; // exit game
	}
	else
		return 0;
}

/* ------------- */

// Monsters

struct monster* game_get_monster_first(struct game* game){
	assert(game);
	return game->monster_first;
}

void game_set_monster_first(struct game * game, struct monster * monster){
	game->monster_first=monster;
}


int game_init_pause(struct game * game){
	int time = SDL_GetTicks(); //time take the number of milliseconds since the SDL library initialized
	int continuer = 1;
	SDL_Event event;

    while (continuer)
    {
    	game_display_pause(game);
        SDL_WaitEvent(&event);
        switch(event.type)
	    {
      		case SDL_QUIT:
        			return 1;

        case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                    	return 1;

                        case SDLK_p:
                        	continuer = 0;
                        	game_resume_pause(game,time);
                        	return 0;
                        	break;

                        default:
                        break;
                    }
                    break;

                    default:
                    break;
	    }
    }
    return 0;
}

void game_resume_pause(struct game * game, int time){
	int current_time =SDL_GetTicks();
	int pause_time =current_time - time ;
	bomb_add_pause_time(game,pause_time);
	monster_add_pause_time(game,pause_time);
}

void game_display_pause(struct game* game){
	// Display the pause image
	struct map* map=game_get_current_map(game);

	    	window_clear();
	    	window_display_image(sprite_get_pause(), (map_get_width(map)*23)/2, (map_get_height(map)*33)/2);
	    	window_refresh();
}

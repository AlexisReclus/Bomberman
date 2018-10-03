#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <bomb.h>
#include <monster.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
	struct bomb * bomb_first;
	int nb_bomb;


};


#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height, int level)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;
	map->bomb_first=NULL;


	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
    if ((x<0 || x>map_get_width(map)-1) || (y<0 || y>(map_get_height(map))-1)){
        return 0;  // Test if the indice x,y are not in the map
    }
	else
        return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	bomb_list_free(map);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum door_type map_get_door_type(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x01;
}

enum compose_type map_get_door_level(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0e;
}

enum compose_type map_get_bomb_type(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

enum compose_type map_get_bonus_type(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
		return map->grid[CELL(x,y)];
}

enum scenery_type map_get_scenery_type(struct map* map,int x,int y){
	assert(map && map_is_inside(map, x, y));
			return map->grid[CELL(x,y)] & 0x0f;
}

void map_set_bomb_type(struct map* map, int x, int y, enum compose_type type){
assert(map && map_is_inside(map, x, y));
map->grid[CELL(x,y)] = type;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void map_open_door(struct map * map, int x,int y){
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = map->grid[CELL(x,y)] | 0x01;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}


void display_bomb (struct map* map,int x, int y,unsigned char type){
	switch (type & 0x0f){
	case BOMB_1 :
		window_display_image(sprite_get_bomb(0),x,y);
		break;

	case BOMB_2 :
			window_display_image(sprite_get_bomb(1),x,y);
			break;

	case BOMB_3 :
			window_display_image(sprite_get_bomb(2),x,y);
			break;

	case BOMB_4 :
			window_display_image(sprite_get_bomb(3),x,y);
			break;

	case BOMB_EX :
			window_display_image(sprite_get_explosion(),x,y);
			break;

	}
}

/*void display_monster (struct map* map,int x, int y,unsigned char type){
	switch (type & 0x0f){
	case MONSTER_1 :
		window_display_image(sprite_get_monster(SOUTH),x,y);
		break;

	case MONSTER_2 :
			window_display_image(sprite_get_monster(NORTH),x,y);
			break;

	case MONSTER_3 :
			window_display_image(sprite_get_monster(EAST),x,y);
			break;

	case MONSTER_4 :
			window_display_image(sprite_get_monster(WEST),x,y);
			break;

	}
}*/

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
			window_display_image(sprite_get_princess(), x, y);
			break;
	}
}

void display_door(struct map* map, int x,int y,unsigned char type){
	switch (type & 0x01) {
		case DOOR_OPENED:
			window_display_image(sprite_get_door_opened(), x, y);
			break;

		case DOOR_CLOSED:
			window_display_image(sprite_get_door_closed(), x, y);
			break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
	      display_door(map,x,y,type);
	      break;
	    case CELL_BOMB:
	    	display_bomb(map,x,y,type);
	    	break;
	    case CELL_MONSTER:
	    	//display_monster(map,x,y,type);
	    	break;
	    }
	  }
	}


}


// ------

void map_copy_box(struct map *map,int x, int y, int new_x,int new_y){
    map->grid[CELL(new_x,new_y)]=map->grid[CELL(x,y)];
}


struct map * map_get_level(int level){
   assert ((level < 7)&&(level>=0));
    char way [150]; // MODIFIER LE MALLOC
	FILE* text_map;
    int largeur;
    int hauteur;

    sprintf(way,"data/map_%d.txt",level);

    text_map=fopen(way,"r");
    assert(text_map);

	fscanf(text_map,"%d:%d",&largeur,&hauteur);
	struct map* map =map_new(largeur,hauteur,level);

	for (int i=0;i<map->width * map->height; i++){
		fscanf(text_map,"%hhu",&map->grid[i]);
	}

	fclose(text_map);
return map;
}

/* BOMB */
struct bomb * map_get_bomb_first(struct map * map){
	return map->bomb_first;
}

void map_set_bomb_first(struct map * map, struct bomb * bomb){
	map->bomb_first=bomb;
}

int map_get_nb_bomb(struct map * map){
	return map->nb_bomb;
}

int map_set_nb_bomb(struct map * map, int nb){
	map->nb_bomb=nb;
	return map->nb_bomb;
}
int map_inc_nb_bomb(struct map * map){
	map->nb_bomb+=1;
	return map->nb_bomb;
}
int map_dec_nb_bomb(struct map * map){
	map->nb_bomb-=1;
	return map->nb_bomb;
}


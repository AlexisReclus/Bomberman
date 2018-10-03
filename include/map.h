#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,  		//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70, 	   	// 	0111 0000
	CELL_PLAYER=0x80 		// 	1000 0000
};


enum monster_type{
	MONSTER_1, //south
	MONSTER_2, //north
	MONSTER_3, //east
	MONSTER_4, //west
};

enum bomb_type{
	BOMB_1,
	BOMB_2,
	BOMB_3,
	BOMB_4,
	BOMB_EX

};
enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_LIFE,
	BONUS_MONSTER
};

enum scenery_type {
	SCENERY_TREE,      // 0
	SCENERY_STONE,     // 1
	SCENERY_PRINCESS   // 2
};

enum door_type{
	DOOR_CLOSED,	//0
	DOOR_OPENED	    //1
};

enum door_level{
	LEVEL1=0x02,
	LEVEL2=0x04,
	LEVEL3=0x06,
	LEVEL4=0x08,
	LEVEL5=0x0A,
	LEVEL6=0x0C,
	LEVEL7=0x0E
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
    CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,

	CELL_DOOR_CLOSED = CELL_DOOR | DOOR_CLOSED,
	CELL_DOOR_OPENED = CELL_DOOR | DOOR_OPENED,

	CELL_DOOR1_CLOSED = CELL_DOOR_CLOSED | LEVEL1,
	CELL_DOOR1_OPENED = CELL_DOOR_OPENED | LEVEL1,

	CELL_DOOR2_CLOSED = CELL_DOOR_CLOSED | LEVEL2,
	CELL_DOOR2_OPENED = CELL_DOOR_OPENED | LEVEL2,

	CELL_DOOR3_CLOSED = CELL_DOOR_CLOSED | LEVEL3,
	CELL_DOOR3_OPENED = CELL_DOOR_OPENED | LEVEL3,

	CELL_DOOR4_CLOSED = CELL_DOOR_CLOSED | LEVEL4,
	CELL_DOOR4_OPENED = CELL_DOOR_OPENED | LEVEL4,

	CELL_DOOR5_CLOSED = CELL_DOOR_CLOSED | LEVEL5,
	CELL_DOOR5_OPENED = CELL_DOOR_OPENED | LEVEL5,

	CELL_DOOR6_CLOSED = CELL_DOOR_CLOSED | LEVEL6,
	CELL_DOOR6_OPENED = CELL_DOOR_OPENED | LEVEL6,

	CELL_DOOR7_CLOSED = CELL_DOOR_CLOSED | LEVEL7,
	CELL_DOOR7_OPENED = CELL_DOOR_OPENED | LEVEL7,

	CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BONUS_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
	CELL_BONUS_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
	CELL_BONUS_LIFE     = CELL_BONUS | BONUS_LIFE,
	CELL_BONUS_MONSTER  = CELL_BONUS | BONUS_MONSTER,

	CELL_BOMB_1 = CELL_BOMB | BOMB_1,
	CELL_BOMB_2 = CELL_BOMB | BOMB_2,
	CELL_BOMB_3 = CELL_BOMB | BOMB_3,
	CELL_BOMB_4 = CELL_BOMB | BOMB_4,
	CELL_BOMB_EX = CELL_BOMB | BOMB_EX,

	CELL_MONSTER_1 = CELL_MONSTER | MONSTER_1,
	CELL_MONSTER_2 = CELL_MONSTER | MONSTER_2,
	CELL_MONSTER_3 = CELL_MONSTER | MONSTER_3,
	CELL_MONSTER_4 = CELL_MONSTER | MONSTER_4,

};

struct monster;
struct bomb;
struct map;


// Create a new empty map
struct map* map_new(int width, int height, int level);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

//Return the type of the door/bonus/bomb/scenery
enum door_type map_get_door_type(struct map* map, int x, int y);
enum compose_type map_get_bonus_type(struct map* map, int x, int y);
enum compose_type map_get_bomb_type(struct map* map, int x, int y);
enum scenery_type map_get_scenery_type(struct map* map,int x,int y);

//Return level of the map
enum compose_type map_get_door_level(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Set the door type
void map_open_door(struct map * map, int x,int y);

// Set the BOMB type
void map_set_bomb_type(struct map * map, int x,int y, enum compose_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();

// Display the map on the screen
void map_display(struct map* map);

// Set the type of the bomb DEJA PLUS HAUT
//void map_set_bomb_type(struct map* map, int x, int y, enum compose_type type);

//Display bomb in the map
void display_bomb (struct map*map,int x, int y,unsigned char type);

//DIsplay monster in the map
//void display_monster (struct map* map,int x, int y,unsigned char type);

//Copy the box in the next cell
void map_copy_box(struct map *map,int x, int y, int new_x,int new_y);

//Display Door opened or closed
void display_door(struct map* map, int x,int y,unsigned char type);

//Create new map
struct map *map_get_level(int level);

// Return the first bomb of the list
struct bomb * map_get_bomb_first(struct map * map);

// Set the first bomb of the list
void map_set_bomb_first(struct map * map, struct bomb * bomb);

// Get/set/increase/decrease the max number of bomb you can put on the map
int map_get_nb_bomb(struct map * map);
int map_set_nb_bomb(struct map * map, int nb);
int map_inc_nb_bomb(struct map * map);
int map_dec_nb_bomb(struct map * map);



#endif /* MAP_H_ */

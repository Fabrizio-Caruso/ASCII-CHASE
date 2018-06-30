
#ifndef _ATARI_MODE1_REDEFINED_CHARS_SETTINGS
#define _ATARI_MODE1_REDEFINED_CHARS_SETTINGS

#define _ATARI_MODE1_RED 0
#define _ATARI_MODE1_WHITE	64
#define _ATARI_MODE1_BLUE 128
#define _ATARI_MODE1_YELLOW 192

#define _PLAYER_DOWN_OFFSET 60
#define _PLAYER_UP_OFFSET 61
#define _PLAYER_RIGHT_OFFSET 62
#define _PLAYER_LEFT_OFFSET 63

#define _EXTRA_LIFE_OFFSET 60
#define _GHOST_OFFSET 59
#define _DEAD_GHOST_OFFSET 59
#define _SKULL_OFFSET 30 

#define _BOMB_OFFSET 29
#define _POWERUP_OFFSET 28
#define _FREEZE_OFFSET 28
#define _SUPER_OFFSET 28
#define _CONFUSE_OFFSET 30
#define _GUN_OFFSET 11
#define _BULLET_OFFSET 10 
#define _INVINCIBILITY_OFFSET 12 
#define _LEFT_HORIZONTAL_MISSILE_OFFSET 9
#define _RIGHT_HORIZONTAL_MISSILE_OFFSET 8	
#define _ROCKET_OFFSET 7

#define _PLAYER_DOWN (60 + _ATARI_MODE1_BLUE)
#define _PLAYER_UP (61 + _ATARI_MODE1_BLUE)
#define _PLAYER_RIGHT (62 + _ATARI_MODE1_BLUE)
#define _PLAYER_LEFT (63 + _ATARI_MODE1_BLUE)


#define _EXTRA_LIFE (60 + _ATARI_MODE1_YELLOW)
#define _GHOST (59 + _ATARI_MODE1_WHITE)
#define _DEAD_GHOST (59 + _ATARI_MODE1_RED)
#define _SKULL (30 + _ATARI_MODE1_YELLOW)
#define _BOMB (29+ _ATARI_MODE1_RED)
#define _POWERUP (28 + _ATARI_MODE1_YELLOW)
#define _FREEZE (28 + _ATARI_MODE1_BLUE)
#define _SUPER (28 + _ATARI_MODE1_RED)
#define _CONFUSE (30 + _ATARI_MODE1_RED)
#define _GUN (11 + _ATARI_MODE1_YELLOW)
#define _BULLET (10 + _ATARI_MODE1_RED)
#define _INVINCIBILITY (12 + _ATARI_MODE1_YELLOW)

#define _LEFT_HORIZONTAL_MISSILE (9 + _ATARI_MODE1_WHITE)
#define _RIGHT_HORIZONTAL_MISSILE (8 + _ATARI_MODE1_WHITE)	
#define _ROCKET (7 + _ATARI_MODE1_WHITE)

#define _EXTRA_POINTS (4 + _ATARI_MODE1_YELLOW)
#define _VERTICAL_BRICK (5 + _ATARI_MODE1_YELLOW)
#define _HORIZONTAL_BRICK (6 + _ATARI_MODE1_YELLOW)

#define _SPACE 0
#define _BROKEN_WALL _BOMB

#endif // _ATARI_MODE1_REDEFINED_CHARS_SETTINGS

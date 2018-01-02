/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "settings.h"
#include "character.h"
#include "strategy.h"
#include "text.h"
#include "level.h"
#include "enemy.h"

#if !defined(TINY_GAME)
	#include "missile.h"
	#include "invincible_enemy.h"
#endif
	
// Input input/output driver headers
#include "display_macros.h"
#include "input_macros.h"
#include "sleep_macros.h"

#include "sound_macros.h"

unsigned short ghostSlowDown;

unsigned short ghostLevel;
unsigned int points;
unsigned int highScore;
unsigned char lives;
unsigned char XSize;
unsigned char YSize;
unsigned short loop;
unsigned char level;

#if !defined(TINY_GAME)
	unsigned short invincibleSlowDown;
	unsigned char invincibleXCountDown;
	unsigned char invincibleYCountDown;
	// unsigned short powerUpCoolDown;	
	// unsigned short gunCoolDown;
	unsigned short ghostLevelDecrease;
	unsigned char missileDirection;
#endif

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. invincibleXCountDown (time needed to activate the invincible ghost)
// 4. invincibleYCountDown
// 5. invincibleSlowDown (how much the invincible ghost is slowed-down)
// 6. invincibleLoopTrigger (how long before the invincible ghost appears)

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	unsigned short invincibleLoopTrigger;
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;	
	extern Image DEAD_GHOST_IMAGE;
#endif

#if defined(FULL_GAME)
	extern unsigned char playerBlink;	
#endif

#if defined(DEBUG) && defined(FULL_GAME)
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image BUBBLE_IMAGE;
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;	
#endif

Character player; 

#if !defined(TINY_GAME)
	Character invincibleGhost;
	Item powerUp;
	Item gun;
	Item extraPoints;	
	Character missile;
#endif

Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)
	unsigned short invincibleGhostCountTrigger = INVINCIBLE_GHOST_TRIGGER;

	unsigned char innerVerticalWallY; 
	unsigned char innerVerticalWallX; 
	unsigned char innerVerticalWallLength;

	Item extraLife;
	Item invincibility;
	
	unsigned short playerInvincibilityCoolDown;	

	Character leftEnemyMissile;
	Character rightEnemyMissile;

	Character bubbles[BUBBLES_NUMBER];

	unsigned char bubbles_x[BUBBLES_NUMBER];
	
	unsigned char player_invincibility = 0;

	unsigned char dead_bubbles = 0;
	
	unsigned char arrowRange;
	
#endif

#if !defined(TINY_GAME)
	unsigned char strategyArray[GHOSTS_NUMBER];

	unsigned char playerDirection = 0; // 0: right, 1: down, 2: left, 3: up
#endif

unsigned int extraLifeThroughPointsCounter = 1;

unsigned char ghostCount = GHOSTS_NUMBER;

#if !defined(TINY_GAME)
	unsigned char playerFire = 0;
	unsigned char guns = GUNS_NUMBER;

	unsigned char invincibleGhostHits = 0;
	unsigned char invincibleGhostAlive = 1;
#endif

#if !defined(TINY_GAME)
void handle_missile()
{
	// Check if player has fired the gun
	if(playerFire && missile._status==0 && guns>0)
	{
		SHOOT_SOUND();
		--guns;
		printGunsStats();
		missileDirection = playerDirection;
		missile._status = setMissileInitialPosition(&missile, &player, missileDirection);
		playerFire = 0;
		DRAW_MISSILE(missile._x,missile._y,missile._imagePtr);					
		checkMissileVsGhosts(&missile);	
	}
	
	// Move missile if fired
	if(missile._status==1)
	{
		moveMissile(&missile, missileDirection);
		// TODO: Inefficient
		checkMissileVsGhosts(&missile);
		
		if(areCharctersAtSamePosition(&missile, &invincibleGhost))
		{
			PING_SOUND();
			die(&missile);
			DELETE_MISSILE(missile._x,missile._y,missile._imagePtr);
			missile._x = 0; missile._y = 0;
			++invincibleGhostHits;
			decreaseGhostLevel();
			// #if defined(FULL_GAME)
			reducePowerUpsCoolDowns();
			// #endif
			
			// TODO: to TEST
			if(invincibleGhostHits>=MIN_INVINCIBLE_GHOST_HITS)
			{
				invincibleGhost._status = 0;
				DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y, invincibleGhost._imagePtr);
				invincibleGhost._x=XSize-2; invincibleGhost._y=YSize-2;
				invincibleGhostAlive = 0;
				EXPLOSION_SOUND();
				points+=INVINCIBLE_GHOST_POINTS;
				displayStats();
			}
			else
			{
				DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
			}
		}
	}
}


void powerUpReached(Character * powerUpPtr)
{
	ZAP_SOUND();
	DELETE_GUN(powerUpPtr->_x,powerUpPtr->_y,powerUpPtr->_imagePtr);
	DRAW_PLAYER(player._x, player._y, player._imagePtr);
	powerUpPtr->_status = 0;
	displayStats();
}

void relocatePowerUp(Character * powerUpPtr)
{
		powerUpPtr->_status = 1;
		
		#if defined(FULL_GAME)
		do
		{
			relocateCharacter(powerUpPtr, bombs,4);
		} while(nearInnerWall(powerUpPtr));		
		#else
			relocateCharacter(powerUpPtr, bombs,4);
		#endif	
}


void powerUpEffect()
{
	points+=POWER_UP_BONUS;
	decreaseGhostLevel(); 
	powerUp._coolDown = POWER_UP_INITIAL_COOLDOWN;	
}


void gunEffect()
{
	guns = GUNS_NUMBER;
	printGunsStats();		
	points+=GUN_BONUS;			
	gun._coolDown = GUN_INITIAL_COOLDOWN;	
}

void extraPointsEffect()
{
	points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
	extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN*2; // second time is harder		
}

void handle_item(Item *itemPtr) //, unsigned short *coolDownPtr, unsigned char *blinkCounter)
{
	// Manage item
	if(itemPtr->_character._status == 1)
	{	
		if(areCharctersAtSamePosition(&player, &(itemPtr->_character)))
		{
			itemPtr->_effect();
			powerUpReached(&(itemPtr->_character));			
		}
		else
		{
			_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
		}		
	}
	else if (itemPtr->_coolDown == 0)
	{
		relocatePowerUp((Character *) itemPtr);

		_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
	}
	else
	{
		--itemPtr->_coolDown;
	}
}

#define handle_gun_item() handle_item(&gun);
#define handle_powerup_item() handle_item(&powerUp);
#define handle_extraPoints_item() handle_item(&extraPoints);
	
#if defined(FULL_GAME)
	
	void extraLifeEffect()
	{
		++lives;
		extraLife._coolDown = EXTRA_LIFE_COOL_DOWN*2; // second time is harder
		printLivesStats();		
	}

	void invincibilityEffect()
	{
		player_invincibility = 1;
		invincibility._coolDown = INVINCIBILITY_COOL_DOWN;
		playerInvincibilityCoolDown = PLAYER_INVINCIBILITY_COOL_DOWN;		
	}

	#define handle_extraLife_item() handle_item(&extraLife);
	#define handle_invincibility_item() handle_item(&invincibility);	
#endif

	// TODO: This has to be moved into level.c
	void initItems()
	{
		powerUp._blink = 0;
		powerUp._effect = powerUpEffect;
		gun._blink = 0;
		gun._effect = gunEffect;
		extraPoints._blink = 0;	
		extraPoints._effect = extraPointsEffect;
		#if defined(FULL_GAME)
			extraLife._blink = 0;
			invincibility._blink = 0;
			extraLife._effect = extraLifeEffect;
			invincibility._effect = invincibilityEffect;
		#endif	
	}
#endif

#if defined(FULL_GAME)
	unsigned char computeArrowRange(void)
	{
			return level/10;
	}
#endif


#if !defined(TINY_GAME)
void handle_invincible_ghost(void)
{
	if(!invincibleGhost._status)
	{
		// Manage invincible ghost
		
		#if defined(FULL_GAME)
		if((!bossLevel() && invincibleGhostAlive &&
							((invincibleXCountDown==0)|| (invincibleYCountDown==0) || (loop>=invincibleLoopTrigger) || (ghostCount<=invincibleGhostCountTrigger))) || 
		   (bossLevel() && loop>=invincibleLoopTrigger))
		#elif defined(TINY_GAME)
		if(loop>=invincibleLoopTrigger) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER))
		#else
		if(invincibleGhostAlive &&
							((invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
							 (loop>=invincibleLoopTrigger) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER)))
		#endif
		{
			invincibleGhost._status = 1;
			DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		}
		else
		{
			--invincibleXCountDown;
			--invincibleYCountDown;
		}
	}
	else
	{ 	
		invincibleSlowDown = computeInvincibleSlowDown();

		if(rand()>invincibleSlowDown)
		{
			TOCK_SOUND();
			DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y,invincibleGhost.imagePtr);
			moveTowardCharacter(&invincibleGhost, 4);
		}
		DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		#if defined(FULL_GAME)
		if(!player_invincibility && areCharctersAtSamePosition(&invincibleGhost, &player))
		#else
		if(areCharctersAtSamePosition(&invincibleGhost, &player))
		#endif
		{
			playerDies();
		}
	}
}
#endif

#if !defined(NO_INITIAL_SCREEN)			
void initialScreen(void)
{	
	CLEAR_SCREEN();					
	printStartMessage();
	
	#if defined(FULL_GAME)
		WAIT_PRESS();
		CLEAR_SCREEN();
		printHints();	
	#endif
}
#endif

#if defined(DEBUG)
void DEBUG_PRINT()
{
	{	
		unsigned char i;
		
		CLEAR_SCREEN();			
		
		for(i=32;i<255;++i)
		{
			printf(" %u: %c", i, i);
		}
	}
	WAIT_PRESS();
}
#endif

int main(void)
{		
	INIT_INPUT();

	INIT_GRAPHICS();

	// Ask for the screen size 
	GET_SCREEN_SIZE(&XSize, &YSize);
	
	highScore = 0;
	
	while(1)
	{

		INIT_IMAGES();
		
		#if defined(DEBUG)
			DEBUG_PRINT();
		#endif
		
		#if !defined(NO_SET_SCREEN_COLORS)
			// Set Screen Colors
			setScreenColors();				
		#endif
		
		#if !defined(NO_INITIAL_SCREEN)
			initialScreen();
			
			#if !defined(TINY_GAME)
				WAIT_PRESS();
				CLEAR_SCREEN();	
			#endif
		#else
			CLEAR_SCREEN();	
			
			printPressKeyToStart();				
		#endif

		#if defined(TINY_GAME) || defined(FULL_GAME)
			highScoreScreen();	
			WAIT_PRESS();	
		#endif
		#if !defined(TINY_GAME)

			CLEAR_SCREEN();
		#endif
		
		extraLifeThroughPointsCounter = 1;
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		do // Level (Re-)Start
		{ 

			
			#if defined(FULL_GAME)
				dead_bubbles = 0;
				extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
				invincibility._coolDown = INVINCIBILITY_COOL_DOWN;
				player_invincibility = 0;
			#endif			
						
			#if !defined(TINY_GAME)
				extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;			
				computeStrategy();
			#endif
			
			loop = 0;
			ghostLevel = 0;

			#if !defined(TINY_GAME)
				ghostLevelDecrease = 140-(level*2);
				

				invincibleGhostAlive = 1;
				invincibleGhostHits = 0;
										
				guns = 0;
				gun._character._status = 0;
							
				gun._coolDown = GUN_INITIAL_COOLDOWN;
				
				computeInvincibleGhostParameters();
			#endif

			ghostSlowDown = computeGhostSlowDown();
			
			CLEAR_SCREEN();
			#if !defined(NO_TEXT)
				// Clear the screen, put cursor in upper left corner

				printLevel();
				sleep(1);
				CLEAR_SCREEN();
			#endif
			
			#if defined(FULL_GAME)
						
				arrowRange = computeArrowRange();
			
				if(bossLevel())
				{
					printKillTheSkull();
					sleep(1);
				}
				CLEAR_SCREEN();
				
				updateInnerWallVerticalData();	
			#endif
			

			printPressKeyToStart();
			WAIT_PRESS();
			CLEAR_SCREEN();
			
			
			#if !defined(TINY_GAME)
				DRAW_BORDERS();
			#endif
			
			fillLevelWithCharacters(ghostCount);	
			
			#if !defined(TINY_GAME)
				initItems();			
				displayStatsTitles();
			#endif
			displayStats();			
			printLevelStats();
			printLivesStats();				
			
			//
			#if !defined(TINY_GAME)
				printGunsStats();
				printGhostCountStats();
			#endif

			#if defined(FULL_GAME)
			while(player._status && ((ghostCount>0 && !bossLevel()) || (invincibleGhostAlive && bossLevel()))) // while alive && there are still ghosts
			#else
			while(player._status && (ghostCount>0) )
			#endif
			{
				#if defined(SLOW_DOWN)
				{
					unsigned short i;
					for(i=0;i<GAME_SLOW_DOWN;++i)
					{
					}
				}
				#endif
									
				handle_extraPoints_item();
					
				#if defined(FULL_GAME)
					if(player_invincibility && playerInvincibilityCoolDown<=0)
					{
						player_invincibility = 0;
					}
					else
					{
						--playerInvincibilityCoolDown;
					}
					
					if((ghostCount<=MAX_GHOST_COUNT_FOR_BUBBLES && rocketLevel()) || bossLevel())
					{ 
						unsigned char i;

						for(i=0;i<BUBBLES_NUMBER;++i)
						{
							if(bubbles[i]._status)
							{
								if(!player_invincibility && areCharctersAtSamePosition(&player,&bubbles[i]))
								{
									playerDies();
								}
								
								if(rand()%2)
								{
									DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);					
									--(bubbles[i]._y);
								}

								DRAW_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);			
								if(bubbles[i]._y<=1)
								{	
									DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);
									//
									bubbles[i]._x = (i+1)*(XSize/(BUBBLES_NUMBER+1));
									bubbles[i]._y = YSize-2;							
								}
							}
						}
					}

					
					if(missileLevel() || bossLevel() || oneMissileLevel())
					{
						if(!oneMissileLevel() && leftEnemyMissile._status)
						{
							DELETE_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);
							if(leftEnemyMissile._x==XSize-2)
							{
								leftEnemyMissile._x=0;
								leftEnemyMissile._y = YSize-1-ENEMY_MISSILE_OFFSET;
							}
							else
							{
								ADVANCED_LEFT_MISSILE();
								if(loop%2 && player._y>=YSize-1-ENEMY_MISSILE_OFFSET-arrowRange && player._x>=leftEnemyMissile._x)
								{
									if(player._y>leftEnemyMissile._y)
									{
										++leftEnemyMissile._y;
									}
									else if(player._y<leftEnemyMissile._y)
									{
										--leftEnemyMissile._y;
									}
								}
							}
							DRAW_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);
							if(!player_invincibility && areCharctersAtSamePosition(&leftEnemyMissile,&player))
							{
								playerDies();
							}
						}
						
						if(rightEnemyMissile._status)
						{
							DELETE_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);
							if(rightEnemyMissile._x==1)
							{
								rightEnemyMissile._x= XSize-1;
								if(oneMissileLevel())
								{
									rightEnemyMissile._y = YSize/2;					
								}
								else
								{
									rightEnemyMissile._y = ENEMY_MISSILE_OFFSET;
								}
							}
							else
							{
								ADVANCED_RIGHT_MISSILE();
								if((!oneMissileLevel() && loop%2 && player._y<=ENEMY_MISSILE_OFFSET+arrowRange && player._x<= rightEnemyMissile._x) ||
								    (oneMissileLevel() && loop%2 && player._y<=YSize/2+arrowRange && player._y>=YSize/2-arrowRange && player._x<= rightEnemyMissile._x))
								{
									if(player._y>rightEnemyMissile._y)
									{
										++rightEnemyMissile._y;
									}
									else if(player._y<rightEnemyMissile._y)
									{
										--rightEnemyMissile._y;
									}
								}
							}
							DRAW_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);				
							if(!player_invincibility && areCharctersAtSamePosition(&rightEnemyMissile,&player))
							{
								playerDies();
							}
						}
					}	

					if (level>=EXTRA_LIFE_FIRST_LEVEL && rocketLevel())
					{
						handle_invincibility_item();
						handle_extraLife_item();
					}
					else if(level>=INVINCIBILITY_FIRST_LEVEL)
					{
						handle_invincibility_item();
					}		
				#endif
						
				++loop;
				
				if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
					printLivesStats();
				}
				
				ghostSlowDown = computeGhostSlowDown();

				MOVE_PLAYER();
				
				_DRAW_PLAYER	
	
				#if !defined(TINY_GAME)
					handle_missile();
				#endif
				
				chasePlayer(ghostSlowDown);
				
				#if !defined(TINY_GAME)
					// This detects collisions of ghosts that have just moved
					if(missile._status)
					{
						checkMissileVsGhosts(&missile);
					}
				#endif
				
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				#if !defined(TINY_GAME)
					handle_gun_item();
				
					handle_powerup_item();
				#endif

				#if defined(FULL_GAME)
				if(wallReached(&player) || 
				   (!player_invincibility && (playerReached(&player) || playerReachedBombs(&player) || innerWallReached(&player)))
				  )
				#else
				if(wallReached(&player) || playerReached(&player) || playerReachedBombs(&player))
				#endif
				{
					playerDies();
				}
			
				#if defined(FULL_GAME)
					SKIP_MORE_DRAW
					{
						SET_TEXT_COLOR(WALL_COLOR);
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
						DRAW_BOMBS();	
					}
				#else
					#if !defined(TINY_GAME)
					SKIP_MORE_DRAW
						DRAW_BOMBS();
					#endif
				#endif
				
				// Display ghosts
				SKIP_DRAW
					displayGhosts();

				#if !defined(TINY_GAME)
					handle_invincible_ghost();
				#endif
				
				++ghostLevel;
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				#if !defined(TINY_GAME)
					#if !defined(NO_TEXT)
						sleep(1);
						printVictoryMessage();
						sleep(2);

						CLEAR_SCREEN();
					#endif

					if(level<=10)
					{
						points+= LEVEL_BONUS*level;
						printLevelBonus(LEVEL_BONUS*level);
					}
					else
					{				
						points+= LEVEL_BONUS*10;
						printLevelBonus(LEVEL_BONUS*10);
					}
					sleep(1);
					CLEAR_SCREEN();						
				#else
					points+= LEVEL_BONUS*5;
				#endif			

				ghostCount = GHOSTS_NUMBER;
				
				#if defined(FULL_GAME)
					if(bossLevel())
					{	
						CLEAR_SCREEN();
						sleep(1);
						PING_SOUND();
						#if !defined(NO_TEXT)
							printExtraLife();
						#endif
						sleep(2);
						++lives;
					}
				#endif
				++level;

			}
			else // if dead
			{
				#if !defined(TINY_GAME)
					CLEAR_SCREEN();
				#endif
				--lives;
				if(lives>0)
				{
					player._status = 1;
				}
			}
		} while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
			
	if(level==FINAL_LEVEL+1) // if completed game
	{
		gameCompleted();
		#if !defined(NO_SLEEP)
			sleep(2);
		#else
			WAIT_PRESS();
		#endif
	}

	// GAME OVER	
	printGameOver();

	sleep(2);
	
	#if !defined(TINY_GAME)
		CLEAR_SCREEN();
	
		finalScore();
	#endif
	
	#if defined(NO_SLEEP)
		WAIT_PRESS();
	#endif
	
	sleep(2);
	if(points>highScore)
	{
		highScore = points;
	}
	CLEAR_SCREEN();
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}

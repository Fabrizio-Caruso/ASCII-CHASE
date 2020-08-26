/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subxect to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif


#include "cross_lib.h"

#include "init_images.h"

extern Image WALL_1_IMAGE;
extern Image WALL_2_IMAGE;

extern Image TWO_WINDOW_WALL_1_IMAGE;
extern Image TWO_WINDOW_WALL_2_IMAGE;

extern Image THREE_WINDOW_WALL_1_IMAGE;
extern Image THREE_WINDOW_WALL_2_IMAGE;

extern Image SMALL_TWO_WINDOW_WALL_1_IMAGE;
extern Image SMALL_TWO_WINDOW_WALL_2_IMAGE;

extern Image PLANE_BACK_IMAGE;
extern Image PLANE_FRONT_IMAGE;

extern Image BOMB_IMAGE;

#define BUILDINGS_NUMBER (XSize-9)
#define FIRST_BULDING_X_POS 5

uint16_t building_height[XSize];

Image *image[] = {&WALL_1_IMAGE, &WALL_2_IMAGE, &TWO_WINDOW_WALL_1_IMAGE, &TWO_WINDOW_WALL_2_IMAGE, &THREE_WINDOW_WALL_1_IMAGE, &THREE_WINDOW_WALL_2_IMAGE, &SMALL_TWO_WINDOW_WALL_1_IMAGE, &SMALL_TWO_WINDOW_WALL_2_IMAGE};


uint8_t x;
uint8_t y;

uint8_t bombActive;
uint8_t bomb_x;
uint8_t bomb_y;

uint8_t level;

uint16_t score;


void deletePlane(void)
{
    _XLIB_DELETE(x,y);
    _XLIB_DELETE(x+1,y);
}


void drawPlane(void)
{
    _XLIB_DRAW(x,y,&PLANE_BACK_IMAGE);
    _XLIB_DRAW(x+1,y,&PLANE_FRONT_IMAGE);
}


int main(void)
{        
    
    Image *buildingTypePtr;

    INIT_GRAPHICS();

    INIT_INPUT();    
    score = 0;
    level = 0;
        
    while(1)
    {
        bombActive = 0;
        bomb_x = 0;
        bomb_y = 0;
        

        
        INIT_IMAGES();
        
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(COLOR_WHITE);
        PRINT(4,0, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        WAIT_PRESS();
        CLEAR_SCREEN();
        
        for(x=0;x<XSize-2;++x)
        {
            building_height[x] = 0;
        }
        for(x=FIRST_BULDING_X_POS;x<FIRST_BULDING_X_POS+BUILDINGS_NUMBER;++x)
        {
            building_height[x] = (uint8_t) 3+(RAND()&7);
            buildingTypePtr=image[RAND()&7];
            
            for(y=0;y<building_height[x];++y)
            {
                _XLIB_DRAW(x,YSize-1-y,buildingTypePtr);
                TOCK_SOUND();
            }
            PING_SOUND();
        }
        SLEEP(1);
        y = 1+level/2;
        x = 0;
        
        SET_TEXT_COLOR(COLOR_WHITE);
        PRINTD(0,0,2,score);
        while((y<YSize-building_height[x+1]) && y<YSize-1)
        {
            drawPlane();
            
            DO_SLOW_DOWN(SLOW_DOWN-level*64);
            
            deletePlane();
            if(x<FIRST_BULDING_X_POS+BUILDINGS_NUMBER+2)
            {
                ++x;
            }
            else
            {
                x=0;
                ++y;
            }
            
            if(!bombActive && KEY_PRESSED())
            {   
                SHOOT_SOUND();
                ++bombActive;
                bomb_x = x;
                bomb_y = y;
                if(building_height[x]>0)
                {
                    building_height[x] = 0;
                    ++score;
                    SET_TEXT_COLOR(COLOR_WHITE);
                    PRINTD(0,0,2,score);
                }
            }
            
            if(bombActive)
            {
                _XLIB_DELETE(bomb_x,bomb_y);
                ++bomb_y;
                _XLIB_DRAW(bomb_x,bomb_y,&BOMB_IMAGE);
            
                if(bomb_y>YSize-2)
                {
                    bombActive = 0;
                    EXPLOSION_SOUND();
                    _XLIB_DELETE(bomb_x,bomb_y);
                }
            }
            
        }
        drawPlane();
        SLEEP(1);
        if(y==YSize-1)
        {
            CLEAR_SCREEN();
            PRINT(4,2,_XL_N _XL_E _XL_X _XL_T _XL_SPACE _XL_L _XL_E _XL_V _XL_E _XL_L);
            SLEEP(2);
            ++level;
        }
        else
        {
            level = 0;
            score = 0;
        }
        WAIT_PRESS();
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}


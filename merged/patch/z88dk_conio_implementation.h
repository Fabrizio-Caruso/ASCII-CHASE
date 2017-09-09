/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef _Z88DK_CONIO_IMPLEMENTATION
#define _Z88DK_CONIO_IMPLEMENTATION
	
	#if defined(__SPECTRUM__)
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)
			#define gotoxy(x,y) printf("\x16%c%c",y+32,x+32);
		#else
			#define gotoxy(x,y) printf("\x16%c%c",x+1,y+1); 
		#endif
	#else	
		#define gotoxy(x,y) zx_setcursorpos(y,x)
	#endif
	
	#define cputc(c) printf("%c",c);
		
	#if defined(__SPECTRUM__)
		#define cgetc() in_Inkey();
	#else
		#define cgetc() getk();
	#endif
		
	#define textcolor(c)

	#define COLOR_BLACK 0	
	#define COLOR_BLUE 1

	#define COLOR_RED 2
	#define COLOR_MAGENTA 3
	
	#define COLOR_GREEN 4
	#define COLOR_CYAN 5
	
	#define COLOR_YELLOW 6
	#define COLOR_WHITE 7
#endif // _Z88DK_CONIO_IMPLEMENTATION
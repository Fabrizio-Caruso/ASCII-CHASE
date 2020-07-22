; ******************************************************
; **       Librer�a de rutinas para Amstrad CPC       **
; **	   Ra�l Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_Chars8
GLOBAL first_char8

.first_char8 defb 32	;first defined char number (ASCII)

.cpc_Chars8   ;each bit of each byte is a pixel, same way as SYMBOL function of Locomotive BASIC.

;; KEY SET BY ANJUEL & NA_TH_AN FROM NANAKO CPC GAME.
   DEFB     0,  0,  0,  0,  0,  0,  0,  0 ; (32)
   DEFB    24, 36, 24,102,153, 24, 36,102
   DEFB    24, 60, 24,102,153, 24, 36,102
   DEFB    24, 52, 25,118,152, 24, 20, 20
   DEFB    24, 44,152,110, 25, 24, 40, 40
   DEFB   189,66,165,129,129,153,129,126
   DEFB     0,  0, 15,252,252, 15,  0,  0
   DEFB     0,  0,240, 63, 63,240,  0,  0
   DEFB    60, 66,165,129, 90, 36, 36, 60
   DEFB     0,128,126,200,248,192,128,  0
   DEFB     0, 60, 102,223,231,122, 36, 24
   DEFB     0,  0,  8, 56, 28, 16,  0,  0
   DEFB    60, 66,165,153,153,165, 66, 60
   DEFB    24, 60, 60, 60,126, 90, 66, 66
   DEFB    24, 36, 24,  0,153,  0, 36,102
   DEFB    24, 24, 24, 48, 24, 12, 24, 24 ; TILE 12 (47)
   DEFB     0,  0,  0,255,  0,  0,  0,  0 ; TILE 13 (48)
   DEFB    16, 62, 32, 60,  4,124,  8,  0 ; (49)
   DEFB   0,60,66,207,195,102,36,20 ; TILE 17 (50)
   DEFB   0,60,90,219,195,90,60,24 ; TILE 18 (51)
   DEFB    32, 32, 36, 62,  4,  4, 14,  0
   DEFB    62, 32, 32, 62,  2, 34, 62,  0
   DEFB    62, 32, 32, 62, 34, 34, 62,  0
   DEFB    62, 36,  4,  4,  4,  4, 14,  0
   DEFB    62, 34, 34, 62, 34, 34, 62,  0
   DEFB    62, 34, 34, 62,  2, 34, 62,  0
   DEFB     0, 24, 24,  0,  0, 24, 24,  0
   DEFB     0, 24, 24,  0,  0, 24, 24, 32
   DEFB     4,  8, 16, 32, 16,  8,  4,  0
   DEFB     0,  0,126,  0,  0,126,  0,  0
   DEFB    32, 16,  8,  4,  8, 16, 32,  0
   DEFB    64,124, 68,  4, 28, 16,  0, 16
   DEFB     0, 56, 84, 92, 64, 60,  0,  0
   DEFB   126, 36, 36, 36, 60, 36,102,  0
   DEFB   124, 36, 36, 62, 34, 34,126,  0
   DEFB     2,126, 66, 64, 66,126,  2,  0
   DEFB   126, 34, 34, 34, 34, 34,126,  0
   DEFB     2,126, 66,120, 66,126,  2,  0
   DEFB     2,126, 34, 48, 32, 32,112,  0
   DEFB     2,126, 34, 32, 46, 36,124,  0
   DEFB   102, 36, 36, 60, 36, 36,102,  0
   DEFB    56, 16, 16, 16, 16, 16, 56,  0
   DEFB    28,  8,  8,  8,  8, 40, 56,  0
   DEFB   108, 40, 40,124, 36, 36,102,  0
   DEFB   112, 32, 32, 32, 34,126,  2,  0
   DEFB   127, 42, 42, 42, 42,107,  8,  0
   DEFB   126, 36, 36, 36, 36, 36,102,  0
   DEFB   126, 66, 66, 66, 66, 66,126,  0
   DEFB   126, 34, 34,126, 32, 32,112,  0
   DEFB   126, 66, 66, 74,126,  8, 28,  0
   DEFB   126, 34, 34,126, 36, 36,114,  0
   DEFB   126, 66, 64,126,  2, 66,126,  0
   DEFB    34, 62, 42,  8,  8,  8, 28,  0
   DEFB   102, 36, 36, 36, 36, 36,126,  0
   DEFB   102, 36, 36, 36, 36, 24,  0,  0
   DEFB   107, 42, 42, 42, 42, 42, 62,  0
   DEFB   102, 36, 36, 24, 36, 36,102,  0
   DEFB   102, 36, 36, 60,  8,  8, 28,  0
   DEFB   126, 66,  4,  8, 16, 34,126,  0
   DEFB     4, 60, 36, 32, 36, 60,  4,  0
   DEFB     0, 64, 32, 16,  8,  4,  0,  0
   DEFB    32, 60, 36,  4, 36, 60, 32,  0
   DEFB     0, 16, 40, 68,  0,  0,  0,  0
   DEFB     0,  0,  0,  0,  0,  0,  0,  0
   DEFB     0,100,104, 16, 44, 76,  0,  0
   DEFB   126, 36, 36, 36, 60, 36,102,  0
   DEFB   124, 36, 36, 62, 34, 34,126,  0
   DEFB     2,126, 66, 64, 66,126,  2,  0
   DEFB   126, 34, 34, 34, 34, 34,126,  0
   DEFB     2,126, 66,120, 66,126,  2,  0
   DEFB     2,126, 34, 48, 32, 32,112,  0
   DEFB     2,126, 34, 32, 46, 36,124,  0
   DEFB   102, 36, 36, 60, 36, 36,102,  0
   DEFB    56, 16, 16, 16, 16, 16, 56,  0
   DEFB    28,  8,  8,  8,  8, 40, 56,  0
   DEFB   108, 40, 40,124, 36, 36,102,  0
   DEFB   112, 32, 32, 32, 34,126,  2,  0
   DEFB   127, 42, 42, 42, 42,107,  8,  0
   DEFB   126, 36, 36, 36, 36, 36,102,  0
   DEFB   126, 66, 66, 66, 66, 66,126,  0
   DEFB   126, 34, 34,126, 32, 32,112,  0
   DEFB   126, 66, 66, 74,126,  8, 28,  0
   DEFB   126, 34, 34,126, 36, 36,114,  0
   DEFB   126, 66, 64,126,  2, 66,126,  0
   DEFB    34, 62, 42,  8,  8,  8, 28,  0
   DEFB   102, 36, 36, 36, 36, 36,126,  0
   DEFB   102, 36, 36, 36, 36, 24,  0,  0
   DEFB   107, 42, 42, 42, 42, 42, 62,  0
   DEFB   102, 36, 36, 24, 36, 36,102,  0
   DEFB   102, 36, 36, 60,  8,  8, 28,  0
   DEFB   126, 66,  4,  8, 16, 34,126,  0
   DEFB     4, 60, 36, 96, 96, 36, 60,  4
   DEFB     0, 16, 16, 16, 16, 16, 16,  0
   DEFB    32, 60, 36,  6,  6, 36, 60, 32
   DEFB     0,  0, 16, 40, 68,  0,  0,  0
   DEFB   126, 66, 90, 82, 90, 66,126,  0
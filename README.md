# CROSS CHASE
Cross Lib and Cross Chase

by Fabrizio Caruso (fabrizio_caruso@hotmail.com)

## THE GOAL

This is a personal project whose goal is the creation of:
1. the universal 8-bit abstraction layer "Cross Lib" for coding universal 8-bit games
2. the "universal" 8-bit game "Cross Chase" that has to be a fun and that should run
on (nearly) ALL 8 bit computers, consoles, handhelds and scientific calculators. 
The game will be the proof of concept of how flexible the library is.

-------------------------------------------

## THE GAME 
The game's goal is to lure the "ghosts" into the mines and possibly kill the "skull" by repeatedly using the gun.
You have to avoid all enemies, the mines, all walls, rockets and chasing missiles.
All flashing items have positive effects and can grant you some special powers. Discover the 10 items.
If you use the gun against the skull and/or the missiles and rockets, you can unlock some special secret items.

You can play the game on-line for different targets:

1. MSX version: https://homebrew.file-hunter.com/index.php?id=crosschase
2. Commodore 264 series (C16/C116/C+4): http://plus4world.powweb.com/play/cross_chase_32k_v0_9
3. Sinclair ZX Spectrum 48K: https://spectrumcomputing.co.uk/playonline.php?eml=1&downid=112056
4. Atari LYNX: https://atarigamer.com/lynx/play/CrossChase/675300242

-------------------------------------------

## CURRENTLY SUPPORTED ARCHITECTURES
The main goal is to get the library and game to work on most 8-bit architectures but other systems are also considered.

### 8-BIT ARCHITECTURES
- COSMAC RCA 1802
- Intel 8080 and its derivatives
- MOS 6502 and its derivatives
- Motorola 6809 and its derivatives
- Zilog 80 and its derivatives


### 16-BIT ARCHITECTURES
- Motorola 68000/68008
- Zilog Z8000
- Intel 8088/8086 
- PDP11
- MOS 65816

### 32-BIT ARCHITECTURES
- PowerPC
- Intel 386
- Motorola 68020
- MIPS32r2 (little-endian)

### 32/64-BIT ARCHITECTURE
We also support any current architecture for which GCC can produce a working binary.

-------------------------------------------

## THE TOOL-CHAIN

How is it this done?
This is achieved  by having "Cross Lib" provide APIs for the game code.
The game and library code are  separated: 
- the game code only depends on the APIs interface and 
- the APIs do not depend on the game code. Therefore it must be possible to re-use the same library for other games/programs.

The tool-chain currently supports: CC65, Z88DK (SCCZ80 and ZSDCC), CMOC, LCC1802, ACK, XTC68, GCC, GCC-Z8K, GCC-ATARI-ST, GCC-AMIGA, GCC-TI.

For more details look at: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/COMPILERS.md

-------------------------------------------

## HOW TO COMPILE THE GAME
In order to compile the game you need to be in a POSIX environment such as Windows+Cygwin, Linux, FreeBSD, MacOS or any Unix-like environment that supports the "make" command.

For more details we refer to: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/PREREQUISITES.md


### Build all targets for a given system

You can build all targets for a given system with

`make [system_name]_targets`

For the list of systems either run `make list` or look at https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md

### Build all targets that are built by a given compiler or devkit:

`make [compiler_name]_targets`

You can find the list of compilers and dev-kit if you either run `make help` or look at https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/COMPILER_NAMES.md


### Detailed build instructions 

For more details on how to build either run `make help` or look at https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/BUILD.md

-------------------------------------------

## THE GAME CODE

The game should be as portable as possible.
Therefore the following coding choices and design rules are applied:
1. ANSI C (for the game logic);
2. strictly separated input/output and hardware-dependent code from the game logic;
3. input for keyboard/joystick and output for sound and display are provided by CrossLib

Some target(s) may get specific graphic code with re-defined characters, software/hardware sprites and music/sound effects.


-------------------------------------------

## SUPPORTED TARGETS 

In principle you can compile the game in turn-based mode with minimal input and output (stdlib) for any architecture for which there is an ANSI C capable compiler.
You can also compile the game in action mode with minimal input and output for any architecture for which there an ANCI C compiler with ncurses or conio libraries.
For most vintage targets, you can build a version of the game with some graphics, sounds and real time input.

For a complete list look at:
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md


-------------------------------------------
STATUS - ISSUES

Z80-based targets:
https://docs.google.com/spreadsheets/d/1qo2skBUtCUCAac3knEE2x-bUBdvxEA_6qvElacJ3xNY/edit?usp=sharing

-------------------------------------------


## THE FUTURE

It should be possible to write more universal games using "Cross Lib" such as: https://github.com/Fabrizio-Caruso/CROSS-SHOOT

The main future goals are
- better separation of CrossLib and the game code
- adding more systems and more compilers/dev-kits to support more architectures.

-------------------------------------------

## ADAPTIVE GRAPHICS

The tool-chain and CrossLib will produce a game with simple black and white ASCII graphics and no sound if none of these is available. 
If colors, graphics and sounds are available the tool-chain and CrossLib will produce a game with some simple sound effects and with some possibly colored graphics.

![Vic 20 title](snapshots/vic20_title.jpg)
![ZX81](snapshots/zx81_1.jpg)
![C16 title](snapshots/c264_title.jpg)
![C16 first level](snapshots/c264_level1.jpg)

For more snapshots we refer to: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SNAPSHOTS.md

-------------------------------------------

## CROSSLIB DETAILS
CrossLib provides an abstraction layer for sound, input, display, etc.

### Sound
Sound abstraction is achieved by providing common APIs for the (few) sounds that CrossLib provides.

### Input
Input abstraction is also achieved by common APIs that support either keyboard and/or joystick input for each possible target.

### Display
Display abstaction is provided by (at least) two levels of abstraction:
- Common APIs that, for each target, implement one of several graphics modes;
- Generic graphics modes that may be shared among a few targets.

For more details on CrossLib we refer to: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/CROSSLIB.md


-------------------------------------------

## LICENCE

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for non-commercial applications, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in
a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not
be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.



/*---------------------------------------------------------------------------

		Flappy Bird DOS clone
		Version: 0.02
		Engine: WIP-2DFX
		Code: Roy Massaad
		Date: Feb 14 2021

		Requirements:
		-86Box (real perfomance), DosBox, Real Pentium 1 machine
		-486DX2 66mhz+, Emulated or Real Machine + VGA card 320x200 256 colors

		Tools and Libraries:
		WGT 4.5 Wordup Graphics ToolKit GPL, Borland C++ 3.1
        http://web.archive.org/web/20001027023929/http://www.egerter.com/opensource.htm
        http://web.archive.org/web/20000606010204/http://www.egerter.com/index3.htm

		License: GPL 3

		TODO:
		-Soundbaster Audio SDK, FLIC movie & MIDI support (currently experimenting)
		-EMS expanded memory use (can't use DOS extenders with BC31)
		-Port to Watcom c++ environment (better compiler/protected mode, but
		 worse vi editor..?)
		-Use inline assembly for better collison code?
		-Check for memory leaks
		-Add/upgrade to more 'modern' engine features (scene graph with flag
		 properties (_visible, draw() and update() on objects centralized loading
		 of images with no memory duplicates)
		-Mouse support
		-Tile Support
		-Custom fonts
		-Write custom vga drawing routines in assembly ? (WGT seems fast enough)
		-Turn the core code into a general purpose 2d game engine
		-Separate the project into more files

		NOTES:
		-Space bar to jump, x to reset, q to quit (lowercase)
		-Smooth scrolling under a 486 dx..
		-We might get a cool DOS engine at some point out of all this eventually
		-BC31 is very old and lacks alot of IDE & c++ features
		-You need to setup BC3 + download and link WGT45 libs/includes/help
		-Edit the project files settings to match your dev enviroment
		-This was developed inside 86Box, files copied to windows98 vm over msnet client 3
		-Tried to use libs+OOP when possible, back in the day c++ for gaming was a
		bad idea for perfomance reasons, x86 assembly was used to speed things up
		-Why i did this? Nostalgia of course! plus flappy bird is easy to code
		-PCX is the image format supported, WGT uses it with Palettes
		-The hitbox of the bird is slighly smaller than the image on purpose
		-If you manage to run it on real 486 hardware let me know!
        -Why not Watcom/Djgpp with VI/RHIDE first? I have memories of Borland C++ IDE as a kid
		-Feel free to help pimp it out

	---------------------------------------------------------------------------
*/

// most includes are disabled and just here for future reference
// set up the proper include folders path and link correct .lib first
#include <wgt45.h>

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

// game classes, moved there to reduce clutter here. no need to use .h header
#include "extra.cpp"

// 4 pipes are enough, will be recycle used from a pool
#define PIPENB 4

// Function prototypes
void drawScene();
void initPipes();
void initBirdy();
void resetGame();
int collisionTest();
void initVGA();
void deinitVGA();
void setupScene();
void far timerRoutine (void);

// Graphic global variables
block birdPic, birdPicO, birdPicU; // O-Original pic, U-Inclined upwards
short oldMode;
block backgroundPic;
block pipePic; 						 	// downward pipe
block pipePic2;              // upward pipe
color pal[256];           // vga pallete for WGT
block virtualscreen;      // for flipping screen double buffers


// Gameplay variables
// dont them change here
int backgroundPos;
int backgroundSpeed;
int birdPosY;
int birdPosX;
int birdWidth;
int birdHeight;
Birdy* birdPlayer = new Birdy();
int fallRate;
int jumpRate;
int jumpBuffer;
int jumpHeight;
char key;
int gameScore;
int gameOver;
int pipeWidth;
int pipeHeight;
int pipeSpeed;
int pipeGap;
int pipeTopMargin;
int pipeBottomMargin;
Pipe* pipes = new Pipe[PIPENB];


void main (int argc, char *argv[])
{

	srand (time(NULL)); // Setup Random number generation

	// Startup Check
	if (!vgadetected ())
		{
		 printf ("Error - VGA card required for any WGT program.\n");
		 exit (0);
		}

	// Setup Graphics and Gameplay
	initVGA();

	setupScene();

	resetGame();


	// Game Loop
	do {

		if (gameOver == 0) timerRoutine(); //Stop game logic if player lost
		//delay(10);    							// if you want to slow the game down
		if (kbhit()) key = getch();   // capture key
		if (key == 'x') resetGame();  // Reset

	} while (key != 'q'); // Quit

	deinitVGA();

}



// change all game variables here
void resetGame(){

	// background sprite
	backgroundPos = 0;
	backgroundSpeed = 1;

	//bird variables
	birdPosY = 41;
	birdPosX = 100;
	birdWidth = 29;
	birdHeight = 23;

	// jump variables
	fallRate = 2;
	jumpRate = 7;
	jumpBuffer = 0;
	jumpHeight = 60;

	// game variables
	key = '.';
	gameScore = 0;
	gameOver = 0;

	// global variables, can be overriden per pipe if need be
	pipeWidth = 50;
	pipeHeight = 165;
	pipeSpeed = 2;
	pipeGap = 150;
	pipeTopMargin = 170;
	pipeBottomMargin = 80;

	// init interactive game objects
	initPipes();
	initBirdy();

}

void initBirdy() {

	birdPlayer->x = birdPosX;
	birdPlayer->y = birdPosY;
	birdPlayer->width = birdWidth;
	birdPlayer->height = birdHeight;

}


void initPipes() {

	int pipeStartPos = 350;

	for (int i = 0; i < PIPENB ; i++) {

		pipes[i].height = pipeHeight;
		pipes[i].width = pipeWidth;
		pipes[i].x = pipeStartPos;
		pipes[i].offset = rand() % 100;
		pipes[i].touched = 0;
		pipes[i].marginTop = pipeTopMargin;
		pipes[i].marginBottom = pipeBottomMargin;

		pipeStartPos += pipeGap;
	}

}


// this is effectivly the core of the game loop
void far timerRoutine (void)
{

	backgroundPos -= backgroundSpeed;

	// this is responsible for moving the pipes and recycling them
	for (int i=0; i < PIPENB ; i++) {

		pipes[i].x -= pipeSpeed;

		if (pipes[i].x < -pipeGap) {

			pipes[i].x = 320 + pipeGap;
			pipes[i].randomize();
			pipes[i].touched = 0;
		}
	}

	// big science physics
	birdPlayer->y += fallRate;

	// reset bird and pipes when out of screen
	if (birdPlayer->y > 300) birdPlayer->y = -50;
	if (backgroundPos < -320) backgroundPos = 0;

	// jump when spacebar is pressed
	if (key == ' ' ) {
		jumpBuffer = jumpHeight;
		 key = '.'; // change it so it wont auto be used again
	};

	// gradually consume the jump height buffer
	if (jumpBuffer > 0) {
		birdPlayer->y -= jumpRate;
		jumpBuffer -= jumpRate;
		birdPic = birdPicU;
	}
	else birdPic = birdPicO;

	collisionTest();

	drawScene();

}



void drawScene() {

		// vsync with double buffers
		wsetscreen(virtualscreen);

		// the background is 2 clones of the same pic horizontally looping
		// ps: stop the scroll if u want to gain some performance..
		wcopyscreen(abs(backgroundPos),0,319,199, backgroundPic, 0,0, virtualscreen);
		wcopyscreen(0,0,abs(backgroundPos),199, backgroundPic, 319-abs(backgroundPos),0, virtualscreen);

		//draw the pipes
		for (int i=0; i < PIPENB; i++) {

			wputblock (pipes[i].x, pipes[i].offset + pipeBottomMargin, pipePic, NORMAL);
			wputblock (pipes[i].x, pipes[i].offset - pipeTopMargin, pipePic2, NORMAL);

		}

		// draw bird with transparency
		wputblock (birdPlayer->x, birdPlayer->y, birdPic, XRAY);

		// Print Score and conditional game over screen
		wgtprintf (30, 20, NULL, "SCORE : %i", gameScore);

		if (gameOver > 0) wgtprintf (124, 50, NULL, "GAME OVER !");

		// vsync with double buffer
		wretrace();
		wcopyscreen(0,0,319,199,virtualscreen,0,0, NULL);

}

// code can be optimized for speed
int collisionTest() {

	//return 0; // skip detection

	for (int i = 0; i < PIPENB; i++) {

		// check if bird is within a pipe range
		if ( (birdPlayer->x + birdPlayer->width > pipes[i].x) && (birdPlayer->x < pipes[i].x + pipes[i].width) ) {

				// and if it hit one of the up/down subpipes pipes
				if (birdPlayer->y < (pipes[i].offset - pipes[i].marginTop + pipes[i].height) || (birdPlayer->y + birdPlayer->height > pipes[i].offset + pipes[i].marginBottom)) {

					gameOver += 1;
					pipes[i].touched = 1;
					return 1; // leave function with a value of 1 to signal pipe crash
				}
		}

		// give the player a point if the pipe is behind them without a crash
		if ( pipes[i].x + pipes[i].width < birdPlayer->x && pipes[i].touched == 0 ) {

			gameScore++;
			pipes[i].touched = 1; //invalid pipe so its not counted again

		}
	}

}

void initVGA() {

	// vga mode and save old dos mode for exit
	oldMode = wgetmode ();
	vga256 ();

	// this is the gobal pallete file used
	wloadpalette ("./pics/palette.pal", pal);
	wsetpalette (0, 255, pal);

}

void deinitVGA(){

	//free memory and reset to dos mode
	wfreeblock (backgroundPic);
	wfreeblock (pipePic);
	wfreeblock (pipePic2);
	wfreeblock (birdPic);
	wfreeblock (birdPicO);
	wfreeblock (birdPicU);

	wsetmode (oldMode);

}

void setupScene() {

	// background sprite pic
	backgroundPic = wloadpcx ("./pics/back.pcx", pal);

	// 2 sprites for the pipe in upward and downward position
	pipePic = wloadpcx ("./pics/pipe.pcx", pal);
	pipePic2 = wloadpcx ("./pics/pipe.pcx", pal);

	// different bird sprites, transparency is a special pink color
	birdPic = wloadpcx ("./pics/bird.pcx", pal);
	birdPicO = wloadpcx ("./pics/bird.pcx", pal);
	birdPicU = wloadpcx ("./pics/bird_u.pcx", pal);

	// flip the upward pipe sprite in code on init
	wflipblock(pipePic2, 0);

	virtualscreen = wnewblock(0,0,319,199);


}


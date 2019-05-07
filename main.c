#include "myLib.h"
#include "text.h"
#include "game.h"
#include <stdio.h>
//Also include any images here
#include "BGTile.h"

// Prototypes
void begin();
void goToStart();
void start();
void goToInstructions();
void instructions();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();

// State Machine variables
enum {START, INSTRUCT, GAME, PAUSE, WIN};
int state;

// Button variables
unsigned short buttons;
unsigned short oldButtons;

// Buffer for score text
char screenText[41];

int main() {
    
    begin();
    
    while(1) {
        oldButtons = buttons;
        buttons = BUTTONS;
         
        // State machine        
        switch(state) {
            case START:
                start();
                break;
            case INSTRUCT:
                instructions();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
        }          
    }
}

//Set the control register and intitialize
void begin() {
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    initialize();
	goToStart();
}

// Prepare for start state
void goToStart() {
	//Load in any background images here
	DMANow(3, BGTilePal, PALETTE, 256);
	drawFullscreenImage(BGTileBitmap);
	
    //Write title to screen
    char start[] = "Bath Time!";
    drawString(80, 90, start, WHITEID);
    
    waitForVBlank();
    flipPage();
	state = START;
}

// Run start state
void start() {
	waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
		goToInstructions();
        initialize();
    }
}

// Prepare for instructions state
void goToInstructions() {
	fillScreen(BLUEID);
    
    // Draw instructions to screen
    char instruct[] = "Catch all of the water drops";
    drawString(40, 40, instruct, WHITEID);
    char instruct2[] = "in your bathtub!";
    drawString(50, 40, instruct2, WHITEID);
    char instruct3[] = "Use the arrow keys to move";
    drawString(90, 40, instruct3, WHITEID);
    char instruct4[] = "left and right.";
    drawString(100, 40, instruct4, WHITEID);
    
    waitForVBlank();
    flipPage();
	state = INSTRUCT;
}

// Run instructions state
void instructions() {
	waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
		goToGame();
    }
}

// Prepare game state
void goToGame() {
    state = GAME;
}

// Run game state
void game() {
    update();
    draw();

    // Update the score and draw to screen
    sprintf(screenText, "Drops Until Full: %d", dropsTilFull);
    drawString(145, 5, screenText, WHITEID);

    waitForVBlank();
    flipPage();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    } else if (dropsTilFull == 0) {
        goToWin();
    }
}

// Prepare pause state
void goToPause() {
    fillScreen(BLUEID);
    drawString(77, 90, "Pause Game", WHITEID);
	
	waitForVBlank();
    flipPage();
    state = PAUSE;
}

// Run pause state
void pause() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
    } else if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToStart();
    }
}

// Prepare win state
void goToWin() {
    //Load in any background images here
    DMANow(3, BGTilePal, PALETTE, 256);
    drawFullscreenImage(BGTileBitmap);
    
    //Draw text to the screen
    drawString(77, 90, "You Win!", BLACKID);

    waitForVBlank();
    flipPage();
    state = WIN;
}

// Run win state
void win() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}
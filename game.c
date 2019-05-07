#include "game.h"
#include "myLib.h"
#include <stdlib.h>
//Include any images here too
#include "raindrop.h"
#include "bathtub.h"

// Variables
PLAYER player;
RAINDROP raindrops[DROPCOUNT];
int dropsTilFull;

void initialize() {
	initializePlayer();
    initializeDrops();

    // Initializes the score
	dropsTilFull = DROPCOUNT;

    // Initializes some basic colors
    unsigned short colors[COLORS] = {BLACK, BLUE, GREEN, RED, WHITE, GRAY};

    //Loads the raindrop palette 
    DMANow(3, raindropPal, PALETTE, 256);

    // Load the basic game colors to the end of the palette
    for (int i = 0; i < COLORS; i++) {
    	PALETTE[256-COLORS+i] = colors[i];
    }
}

// Updates the game each frame
void update() {
	updatePlayer();

	for (int i = 0; i < DROPCOUNT; i++) {
		updateDrop(&raindrops[i]);
	}
}

// Draws the game
void draw() {
    fillScreen(BLACKID);
    drawPlayer();

	for (int i = 0; i < DROPCOUNT; i++) {
		drawDrop(&raindrops[i]);
	}
}

// Initializes the player
void initializePlayer() {
	player.row = 130;
	player.col = 118;
	player.cdel = 2;
	player.height = 10;
	player.width = 20;
}

// Updates the player each frame
void updatePlayer() {
	// Move the bathtub
	if (BUTTON_HELD(BUTTON_LEFT) && player.col >= player.cdel) {
		player.col -= player.cdel;
	} else if (BUTTON_HELD(BUTTON_RIGHT) && player.col + player.width - 1 < SCREENWIDTH - player.cdel) {
		player.col += player.cdel;
	}
}

// Draws the player
void drawPlayer() {
	drawImage(player.row, player.col, player.height, player.width, bathtubBitmap);
}

// Initializes all the raindrops
void initializeDrops() {
	for (int i = 0; i < DROPCOUNT; i++) {
		raindrops[i].height = 10;
		raindrops[i].width = 10;
		raindrops[i].row = rand() % 80;
		raindrops[i].col = rand() % 120 + 10;
		raindrops[i].rdel = 2;
		raindrops[i].cdel = 2;
		raindrops[i].active = 1;
	}
}

// Updates the raindrop each frame
void updateDrop(RAINDROP* r) {
	if (r->active) {
		// Loop the drops vertically
		if (r->row <= 1 || r->row + r->height-1 >= 160) {
			r->row = 0;
			r->col = rand() % 240  + 10;
		}

		//Make the raindrops fall
		r->row += r->rdel;
		
		// Handle collisions between drop and bathtub
		if (collision(r->row, r->col, r->height, r->width, player.row, player.col, player.height, player.width)) {
				r->active = 0;
				dropsTilFull--;
		}
	}
}

// Draw a raindrop
void drawDrop(RAINDROP* r) {
	if(r->active) {
		drawImage(r->row, r->col, r->height, r->width, raindropBitmap);
	}
}
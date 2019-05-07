#include "myLib.h"

// Important addresses
unsigned short *videoBuffer = (unsigned short *)0x6000000;
DMA *dma = (DMA *)0x40000B0;

//Sets a pixel in mode 4
void setPixel(int row, int col, unsigned char color) {
	int offset = (col + row*240) >> 1;
	u16 original = videoBuffer[offset];

	if (col & 1) {
		videoBuffer[offset] = color << 8 | (original & 0x00FF);
	} else {
		videoBuffer[offset] = color | (original & 0xFF00);
	}
}

// Fills the screen in mode 4
void fillScreen(volatile unsigned char color) {
    volatile unsigned int newColor = color << 24 | color << 16 | color << 8 | color;
    DMANow(3, &newColor, videoBuffer, DMA_SOURCE_FIXED | DMA_32 | (SCREENWIDTH * SCREENHEIGHT) / 4);
}

//Draws a rectangle in mode 4
void drawRect(int row, int col, int height, int width, volatile unsigned char color) {
	int oddW = width & 1;
	int oddCol = col & 1;

	volatile u16 newColor = color | (color << 8);
	if (!oddCol && !oddW) {
		for (int i = 0; i < height; i++){
			DMANow(3, &newColor, videoBuffer + OFFSET(row + i, col, 240)/2, width/2 | DMA_SOURCE_FIXED);
		}
	} else if (!oddCol && oddW) {
		int newW = width + 1;
		for (int i = 0; i < height; i++){
			DMANow(3, &newColor, videoBuffer + OFFSET(row + i, col, 240)/2, newW/2 | DMA_SOURCE_FIXED);
		}
	}else if (oddW && oddCol) { 
		int newW = width + 1; 
		int newCol = col + 1;
		for (int i = 0; i < height; i++){
			DMANow(3, &newColor, videoBuffer + OFFSET(row + i, newCol, 240)/2, newW/2 | DMA_SOURCE_FIXED);
		}
	} else {
		int newCol = col + 1;
		for (int i = 0; i < height; i++){
			DMANow(3, &newColor, videoBuffer + OFFSET(row + i, newCol, 240)/2, width/2 | DMA_SOURCE_FIXED);
		}
	}	
}

//Draws an image in mode 4
void drawImage(int row, int col, int height, int width, const unsigned short *image) {
	for (int i = 0; i < height; i++) {
		DMANow(3, ((char *)image) + (width*i), ((char *)videoBuffer) + col + 240*(row + i), width / 2);
	}
}

//Draws a full screen image in mode 4
void drawFullscreenImage(const unsigned short *image) {
    for (int i = 0; i < 160; i++){
    	DMANow(3, ((char *)image) + (240*i), ((char *)videoBuffer) + 240*i, 240/2);
    }
}

//Waits for the Vertical Blank
void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

//Flips the page
void flipPage() {
	if (REG_DISPCTL & DISP_BACKBUFFER) {
		videoBuffer = BACKBUFFER;
	} else {
		videoBuffer = FRONTBUFFER;
	}
	REG_DISPCTL ^= DISP_BACKBUFFER;
}

//Begins DMA transfer
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt) {
	dma[channel].cnt = 0;
	dma[channel].src = src;
    dma[channel].dst = dst;
	dma[channel].cnt = cnt | DMA_ON;
}

//Checks for a collision
int collision(int rowA, int colA, int heightA, int widthA, int rowB, int colB, int heightB, int widthB) {
    return rowA < rowB + heightB - 1 && rowA + heightA - 1 > rowB
        && colA < colB + widthB - 1 && colA + widthA - 1 > colB;
}
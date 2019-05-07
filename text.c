#include "myLib.h"
#include "text.h"
#include "font.h"

void drawChar(int row, int col, char ch, unsigned char color) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			if (fontdata_6x8[48*ch + OFFSET(i, j, 6)]) {
				setPixel(row + i, col + j, color);
			}
		}
	}
}

void drawString(int row, int col, char *string, unsigned char color) {
	while (*string != '\0') {
		drawChar(row, col, *string, color);
		col += 6;
		string++;
	}
}
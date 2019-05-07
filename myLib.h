#ifndef MY_LIB_H
#define MY_LIB_H

typedef unsigned short u16;

#define OFFSET(r,c,rowlen) ((r)*(rowlen)+(c))

// DISPLAY
#define REG_DISPCTL (*(unsigned short *)0x4000000)
#define MODE4 4
#define DISP_BACKBUFFER (1<<4)
#define BG2_ENABLE (1<<10)
#define SCANLINECOUNTER (*(volatile unsigned short *)0x4000006)
#define SCREENHEIGHT 160
#define SCREENWIDTH 240

extern unsigned short *videoBuffer;
#define FRONTBUFFER ((unsigned short *)0x6000000)
#define BACKBUFFER ((unsigned short *)0x600A000)

#define PALETTE ((unsigned short *)0x5000000)

#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define BLACK 0
#define WHITE COLOR(31,31,31)
#define GRAY COLOR(15,15,15)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define YELLOW COLOR(31,31,0)

void setPixel(int row, int col, unsigned char color);
void drawRect(int row, int col, int height, int width, volatile unsigned char color);
void fillScreen(volatile unsigned char color);
void drawImage(int row, int col, int height, int width, const unsigned short *image);
void drawFullscreenImage(const unsigned short *image);
void waitForVBlank();
void flipPage();

// INPUT
#define BUTTONS (*(volatile unsigned short *)0x04000130)

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

extern unsigned short oldButtons;
extern unsigned short buttons;

#define BUTTON_HELD(key)  (~(BUTTONS) & (key))
#define BUTTON_PRESSED(key) (!(~(oldButtons)&(key)) && (~buttons & (key)))

// DMA
typedef volatile struct {
    volatile const void *src;
    volatile void *dst;
    unsigned int cnt;
} DMA;

extern DMA *dma;

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)

#define DMA_ON (1 << 31)

void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt);

// MISC

int collision(int rowA, int colA, int heightA, int widthA, int rowB, int colB, int heightB, int widthB);

#endif









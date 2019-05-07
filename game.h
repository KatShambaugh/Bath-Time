typedef struct {
	int row;
	int col;
	int cdel;
	int height;
	int width;
} PLAYER;

typedef struct {
	int row;
	int col;
	int rdel;
	int cdel;
	int height;
	int width;
	int active;
} RAINDROP;

// Constants
#define DROPCOUNT 15
#define COLORS 6

enum {BLACKID=(256-COLORS), BLUEID, GREENID, REDID, WHITEID, GRAYID};
extern unsigned short colors[COLORS];

// Variables
extern PLAYER player;
extern RAINDROP raindrops[DROPCOUNT];
extern int dropsTilFull;

// Prototypes
void initialize();
void update();
void draw();
void initializePlayer();
void updatePlayer();
void drawPlayer();
void initializeDrops();
void updateDrop(RAINDROP *);
void drawDrop(RAINDROP *);
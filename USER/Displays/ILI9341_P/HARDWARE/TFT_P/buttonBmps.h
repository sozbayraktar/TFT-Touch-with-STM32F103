
#include "inttypes.h"

// ============= BUTON ETIKET TANIMLARI (BMPFILE içindek, indeksler) ==================

#define ZERO  0
#define ONE 16
#define TWO 32
#define THREE 48
#define FOUR 64
#define FIVE 80
#define SIX 96
#define SEVEN 112
#define EIGHT 128
#define NINE 144
#define UP 160
#define DOWN 176
#define LEFT 192
#define RIGHT 208
#define Z_UP 224
#define Z_DOWN 240
#define CHECK 256
#define CENTER 272
#define EXIT 288
#define XYZ 304
#define ENTER 320
#define _X 336
#define _Y 352
#define _Z 368
#define _CLEAR_ 384
#define DASH 400
#define ASTERIX 416
#define _D 432
#define FILE 448
#define JOG 464
#define OPTS 480
#define STEPUP 496
#define STEPDOWN 512
#define ZOOMIN 528
#define ZOOMOUT 544
#define MIRROR 560
#define CUT 576
#define DRAW 592
#define CUTANDRAW 608
#define UNMIRROR 624

// ============= BUTON ETIKET TANIMLARI SONU ==================

struct mtrx{
short matrixNo;	
short numberOfColumns;
short numberOfRows;
int leftUpCornerX;
int	leftUpCornerY;
int xSize;
int ySize;
short gap;
short labelOffset;
short errMargin;
};

struct mtrx TFT_keypad;
struct mtrx* pTFT_keypad=&TFT_keypad;

const uint16_t labelList[] = 
{
ASTERIX,_D,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,ZERO,    // 0..11 liste satirlari	

FILE,JOG,XYZ,OPTS,  //12..19 Main Menu
DASH,DASH,_CLEAR_,EXIT,
	
_X,CHECK,                  //20..21 EVET/HAYIR  	
	
DASH,DASH,               //22..35
DASH,DASH,DASH,DASH,
DASH,DASH,DASH,DASH,
DASH,DASH,DASH,DASH,

Z_UP,UP,STEPUP,DASH,  // 36..51 JOYSTICK KEYPAD (jogPAD)
LEFT,CENTER,RIGHT,DASH,
Z_DOWN,DOWN,STEPDOWN,DASH,
EXIT,DASH,DASH,DASH,	

ONE,TWO,THREE,_X,  // 52..67 KOORDINAT GIRISI
FOUR,FIVE,SIX,_Y,
SEVEN,EIGHT,NINE,_Z,
EXIT,ZERO,CHECK,ENTER,
	
CUTANDRAW,DASH,ZOOMIN,MIRROR,  //68..83 OPTIONS
CUT,DASH,DASH,UNMIRROR,
DRAW,DASH,ZOOMOUT,DASH,
EXIT,DASH,DASH,ENTER}
;

const uint16_t* pLabelList=&labelList[0];


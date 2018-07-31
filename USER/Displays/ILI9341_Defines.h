#include "stdint.h"
typedef enum {
	TM_ILI9341_Landscape,
	TM_ILI9341_Portrait
} TM_ILI9341_Orientation;

typedef enum {
	TM_ILI9341_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	TM_ILI9341_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	TM_ILI9341_Orientation_Landscape_1, /*!< Landscape orientation mode 1 (0,0) sag altta*/
	TM_ILI9341_Orientation_Landscape_2,  /*!< Landscape orientation mode 2 (0,0) sol üstte*/
	TM_ILI9341_Orientation_Landscape_3  /*!< Landscape orientation mode 3 (0,0) sol altta */
} TM_ILI9341_Orientation_t;

typedef struct {
	uint16_t width;
	uint16_t height;
	TM_ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
} TM_ILI9341_Options_t;

#define ILI9341_CS_PORT      GPIOA
#define ILI9341_CS_PIN       GPIO_Pin_11
#define ILI9341_CS           GPIOA,GPIO_Pin_11
#define ILI9341_DC_PORT      GPIOA
#define ILI9341_DC_PIN       GPIO_Pin_8
#define ILI9341_DC           GPIOA,GPIO_Pin_8
#define ILI9341_RST_PORT     GPIOB
#define ILI9341_RST_PIN      GPIO_Pin_0
#define ILI9341_RST          GPIOB,GPIO_Pin_0


/* Pin definitions */
#define ILI9341_RST_SET				GPIO_SetBits(ILI9341_RST)
#define ILI9341_RST_RESET			GPIO_ResetBits(ILI9341_RST)
#define ILI9341_CS_SET				GPIO_SetBits(ILI9341_CS)
#define ILI9341_CS_RESET			GPIO_ResetBits(ILI9341_CS)
#define ILI9341_DC_SET				GPIO_SetBits(ILI9341_DC)
#define ILI9341_DC_RESET			GPIO_ResetBits(ILI9341_DC)


#define ILI9341_TFTWIDTH  480
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04


/*
#define ILI9341_PWCTR6  0xFC

*/

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

/* LCD settings */
#define ILI9341_WIDTH        480
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        154240

#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))  // __AVR__  #include <avr/pgmspace.h>


	

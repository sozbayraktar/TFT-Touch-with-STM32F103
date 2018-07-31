#ifndef __TEST_H__
#define __TEST_H__
#include "inttypes.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//////////////////////////////////////////////////////////////////////////////////

void DrawTestPage(uint8_t *str);
void Test_Color(void);
void Test_FillRec(void);
void Test_Circle(void);
void English_Font_test(void);
void main_test(void);
extern void Delay_1ms(uint16_t time);
extern void TFT_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode);

#endif

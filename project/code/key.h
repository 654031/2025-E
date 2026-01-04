#ifndef __KEY_H
#define __KEY_H

#include "zf_common_headfile.h"

// 没有按键按下，level全为1，temp全为0
// 逐飞主板的四个按键
#define Key_Temp_zf ((Key_A30|(Key_A31<<1)|(Key_B0 <<2)|(Key_B1<<3))^0x0F)
#define Key_A30 gpio_get_level(A30)
#define Key_A31 gpio_get_level(A31)
#define Key_B0 gpio_get_level(B0)
#define Key_B1 gpio_get_level(B1)

// 无名主板的八个按键
#define Key_Temp_wm ((Key_K1|(Key_K2<<1)|(Key_K3 <<2)|(Key_UP<<3)|(Key_DN<<4)|(Key_LT <<5)|(Key_RT<<6)|(Key_ME<<7))^0xFF)
#define Key_K1 gpio_get_level(B21)
#define Key_K2 gpio_get_level(B23)
#define Key_K3 gpio_get_level(B24)
#define Key_UP gpio_get_level(B12) // 五向按键 上
#define Key_DN gpio_get_level(B8)	 // 五向按键 下
#define Key_LT gpio_get_level(B9)  // 五向按键 左
#define Key_RT gpio_get_level(B10) // 五向按键 右
#define Key_ME gpio_get_level(B11) // 五向按键 中

extern uint8 Key_Flag;
extern uint32 KeyTick;

void Key_Init(uint8 whichBoard);	// 放init()里面
void Key_Scan(uint8 whichBoard);
void Key_Judge(uint8 whichBoard);	// 放pit里面
void key_pro(void);								// 放duty_10ms吧

#endif

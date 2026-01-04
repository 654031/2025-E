#ifndef __DISPLAY_H
#define __DISPLAY_H

/**********************************
	本头文件下的函数仅使用于IPS200的屏幕
	屏幕短边最大值为240
	屏幕长边最大值为300
***********************************/

#include "zf_common_headfile.h"

#define IPS200_TYPE     (IPS200_TYPE_SPI)    

extern char LineString[50];

void display_pro(void);				// 放在void Duty_10ms()

//使用这两个函数必须包含zf_device_ips200.h 因为有些引脚定义在里面
//屏幕默认竖着使用 黑底白字 
//想修改使用去LCD_Init函数中修改




//			LCD_DisplayStringLine:
//			参数1 x 行数         参数2 y 列数     参数3为字符串数组
//  			0<=x<=30						0<=y<=20
//
//  		使用方法与蓝桥杯相同
//
//			sprintf((char*)box,"%d",c);
//			LCD_DisplayStringLine(0,0,(char*)box);
void LCD_DisplayStringLine(uint16 x ,uint16 y,const char dat[]);
void LCD_Init(void);
#endif

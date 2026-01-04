#ifndef _PID_H_
#define _PID_H_
#include "zf_common_headfile.h"

float Position_PID(float reality,float target);				// 位置式角度			更快

float Position_PID_2(float reality,float target);			// 位置式角速度
float Incremental_PID_2(float reality,float target);	// 增量式角速度

int Position_PID_3(float reality,float target);				// 位置式速度		
int Incremental_PID_1(int reality,int target);				// 增量式速度			更稳

float Position_PID_4(float reality,float target);				// 位置式灰度环		替换角度环		

int PwmXianfu(float dat,int min,int max);
//int abs(int num);

// 增量式速度环
extern float Incremental_KP_1;
extern float Incremental_KI_1;
extern float Incremental_KD_1;

// 位置式角度环
extern float Position_KP;
extern float Position_KI;
extern float Position_KD; 

// 增量式角速度环
extern float Incremental_KP_2;
extern float Incremental_KI_2;
extern float Incremental_KD_2;  

// 位置式角速度环
extern float Position_KP_2;
extern float Position_KI_2;
extern float Position_KD_2;

// 位置式速度环
extern float Position_KP_3;
extern float Position_KI_3;
extern float Position_KD_3; 

// 位置灰度环
extern float Position_KP_4;
extern float Position_KI_4;
extern float Position_KD_4; 

#endif

#ifndef  __ENCOUNDER_H
#define  __ENCOUNDER_H

#include "zf_common_headfile.h"

// 左电机
#define E1A                      (B17)						// 编码器1的A相
#define E1B                      (B18)						// 编码器1的B相
// 右电机
#define E2A                      (B26)   
#define E2B                      (B27)


extern float position_lt;				// 左电机编码器里程
extern float position_rt;				// 右电机编码器里程


extern int speed_lt;						// 测速用 不是给PWM的
extern int speed_rt;						// 测速用 不是给PWM的

extern int drive_left;					// 速度环输出给PWM
extern int drive_right;					// 速度环输出给PWM

extern int turn_left;
extern int turn_right;

extern float last_position;
extern float position;					// 左右里程的平均数
extern int Reality_Velocity;		// 左右速度里程的平均数

void encoder_init(void);	// 放init()里面
// 获取速度,积分得到里程
void encoder_read(void);	// 放Duty_20ms()

void enconder_v(void);

#endif

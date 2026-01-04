#ifndef _motor_h_
#define _motor_h_

#include "zf_common_headfile.h"

// motorA接左电机，motorB接右电机
// 电机旋转速度（给电机的PWM）	A0和G7时钟源选择都是80MHz
#define MOTOR_LT_PWM               (PWM_TIM_A0_CH0_B8)
#define MOTOR_RT_PWM               (PWM_TIM_G7_CH0_A26)

// 电机旋转方向
#define MOTOR_AIN1                (B13)						// AIN1	置1为正转
#define MOTOR_AIN2                (B9)						// AIN2	置1为反转
#define MOTOR_BIN1                (B11)
#define MOTOR_BIN2                (A27)

void motor_init(void);	// 放init()里面
void set_motor_pwm(int left_pwm, int right_pwm);


#endif

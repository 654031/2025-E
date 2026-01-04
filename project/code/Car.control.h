#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "zf_common_headfile.h" 

#define IMU963RA	(0)

#define dead_zone	(1200)	// 电机死区，需要测试，deadZone_test(void);
//A车死区1000
#define xianfu (3000)
extern int imu_flag;
extern float error_pwm_1;
extern float last_angle;
extern float now_angle;
extern float target_angle;
extern float v_angle;

extern int new_drive_left;
extern int new_drive_right;

extern int error_pwm;

extern uint8 zhuanjiao_cnt;
extern uint8 last_zhuanjiao_cnt;

extern uint8 zhuanwan_flag;
extern uint8 stop_flag ;			// 到达信号
extern uint8 car_start_flag;	// 发车信号


extern float road1;
extern float road3;
extern int basic_speed;				// 慢慢的速度  作为基础速度
extern int high_speed;				// 慢慢的速度  作为基础速度
extern int Target_Velocity;
	
extern uint8 quanshu;
extern uint8 target_quanshu;	
	
void deadZone_test(void);			// 死区测试时，需要把imu_control注释掉
void control_test_1(void);

void control1(void);
void control_2(void);
void control_3(void);
void control_4(void);

void imu_read(void);
void imu_flag_(void);

void IMU_select(uint8 whichIMU);

void dot_init(void);
void imu_target(void);
//****最终输出PWM的位置****//
void imu_control(void);//*//
//****最终输出PWM的位置****//

#endif

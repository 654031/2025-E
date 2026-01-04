#ifndef __IMUNEW_H_
#define __IMUNEW_H_

#define times               (500)		// 初始化采样次数

/*
	适用于IMU963RA
*/

void imu_read(void);		// 放void pit_handler (uint32 state, void *ptr);	5ms执行一次
void imu_flag_(void);		// 放init();

extern int imu_flag;

extern float yaw_imu;		// 7.28修改，和维特陀螺仪区分
extern float gxx,gyy,gzz;

#endif
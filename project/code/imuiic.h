#ifndef _imuiic_h_
#define _imuiic_h_

#include "zf_common_typedef.h"




//================================================声明 IMU963RA 基础函数================================================
void    imu963ra_get_acc_iic            (void);                                     // 获取 IMU963RA 加速度计数据
void    imu963ra_get_gyro_iic           (void);                                     // 获取 IMU963RA 陀螺仪数据
void    imu963ra_get_mag_iic            (void);                                     // 获取 IMU963RA 磁力计数据
uint8   imu963ra_init_iic               (void);                                     // 初始化 IMU963RA
//================================================声明 IMU963RA 基础函数================================================



#endif

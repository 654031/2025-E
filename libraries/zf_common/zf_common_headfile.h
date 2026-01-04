/*********************************************************************************************************************
* MSPM0G3507 Opensource Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          zf_common_headfile
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2025-06-1        SeekFree            first version
********************************************************************************************************************/

#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_

#include "stdio.h"
#include "stdint.h"
#include "string.h"

//===================================================芯片 SDK 底层===================================================
#include "ti_msp_dl_config.h"
//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_timer.h"
#include "zf_driver_uart.h"
//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_oled.h"
#include "zf_device_tft180.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_ips200pro.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu963ra.h"
#include "zf_device_imu660rb.h"
#include "zf_device_type.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wifi_spi.h"
#include "zf_device_tsl1401.h"
#include "zf_device_dl1b.h"
#include "zf_device_dl1a.h"
#include "zf_device_wireless_uart.h"
#include "CoreDrive.h"
//===================================================外接设备驱动层===================================================

//===================================================应用组件层===================================================
#include "seekfree_assistant.h"
#include "seekfree_assistant_interface.h"
//===================================================应用组件层===================================================

/***************
视觉	 	-> uart0
debug  	-> uart1
wit	    -> uart2
串口屏	-> uart3
****************/

//=======================================================204=====================================================

/*********任务调度（核心）*****/
#include "init.h"
#include "pit.h"
#include "scheduler.h"
#include "Car.control.h"
#include "Servocontrol.h"
#include "DriveControl.h"
#include "renwuManage.h"
/*********任务调度（核心）*****/

/*********无线调试***********/
#include "vofadebug.h"
#include "ANO_Debug.h"
/*********无线调试***********/

/*********硬件驱动***********/
#include "key.h"
#include "init.h"
#include "display.h"
#include "encoder.h"
#include "motor.h"
#include "imunew.h"
#include "flash.h"
#include "servo.h"
#include "track_black.h"
#include "imuiic.h"
#include "mpu.h"
/*********硬件驱动***********/

/*********算法层************/
#include "pid.h"
/*********算法层************/

//========================================================204======================================================
extern float gyrox_wit, gyroy_wit, gyroz_wit;
extern float pitch_wit, roll_wit, yaw_wit;

extern uint32_t ANO_1;
extern uint32_t ANO_2;
extern uint32_t ANO_3;
extern uint32_t ANO_4;
extern uint32_t ANO_5;
extern uint32_t ANO_6;
extern uint32_t ANO_7;
extern uint32_t ANO_8;
extern uint32_t ANO_9;
extern uint32_t ANO_10;
extern uint8_t ANO_Flag;

extern uint32_t ANO_tx_1;
extern uint32_t ANO_tx_2;
extern uint32_t ANO_tx_3;

extern uint16_t Angle2;//偏航角，全量输出
extern uint16_t Angle1;//俯仰角

extern float errerr;//误差差值
extern float position;
extern uint8_t Dir1;//俯仰角，转动方向，顺时针
extern uint8_t Dir2;//偏航角，转动方向，顺时针
extern uint8_t Drive_Flag1;
extern uint8_t Drive_Flag2;
extern uint16 Time; 
extern int16_t RollX_step;//X轴步长
extern float Angle2_temp; //
extern _PID Roll_Inc_Pid; //横滚角
extern _PID Pitch_Inc_Pid;//俯仰角
extern uint8_t Flag;
extern uint16_t Task3Off;
extern uint8_t Task3_Flag;
extern uint16_t Task4_Flag;
extern uint8 whichIMU;
extern uint8_t Task3_Dir;

extern uint16_t Task4_Flag_Old;
extern uint16_t Task4_Flag_New;

extern uint16_t Quan1;
extern uint16_t Quan2;
extern uint16_t Quan3;
extern uint16_t Quan4;

extern int16 imu963ra_gyro_x_iic , imu963ra_gyro_y_iic , imu963ra_gyro_z_iic ;
extern int16 imu963ra_acc_x_iic ,  imu963ra_acc_y_iic ,  imu963ra_acc_z_iic;

//陀螺仪零漂值
extern gyroOffset_info_struct gyroOffset;
extern accOffset_info_struct accOffset;
//用于姿态解算的低通滤波后的数据
extern imu_info_struct imu;
//四元数姿态解算欧拉角用于串级PID角度环
extern float roll,pitch,yaw;

extern float p_gx,p_gy,p_gz;
extern float Yaw_Angle_Temp;
extern PidTypeDef Yaw_Angle_Pid;//Yaw轴角度环PID，用于云台控制
extern uint8_t task456_start_flag;//456用于隔离云台和车的控制
extern uint16_t Angle_out;//Yaw轴角度环输出，用于直接控制云台
extern uint16_t Angle_Speed;//角度环速度值
extern uint16_t Task4_Flag;
extern uint8 zhuanwan_flag;
extern uint8_t Task4_Turn_flag;//转角
extern uint8_t Task4_Run_flag;//直线

extern int Reality_Velocity;//车车速度
extern float xishu1;
extern float xishu2;
extern float Speed_Kp1;//速度前馈补偿
extern float Speed_Kp2;//速度前馈补偿
extern float FB_Velo;//前馈速度
extern float gzz1;
extern float start_position,ture_position;//用于编码器前馈
extern uint8_t ANO4_Err;
#endif

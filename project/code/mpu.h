#ifndef __MPU_H
#define __MPU_H
#include "zf_common_headfile.h"               //头文件




//===================================================宏定义===================================================

#define PI                  ( 3.1415926535898 )
#define GYR_SAMPLE           (0x00)                                      // 陀螺仪量程
#define delta_T  0.005f
#define IMU_TYPE 0
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // 弧度转换为角度
#if IMU_TYPE
    #define GYRO_SPL 14.3
    #define ACC_SPL  4098.0
#else
    #define GYRO_SPL 0.03051756f*2
// 设置为:0x52 陀螺仪量程为:±125dps  获取到的陀螺仪数据除以228.6，   可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x50 陀螺仪量程为:±250dps  获取到的陀螺仪数据除以114.3，   可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x54 陀螺仪量程为:±500dps  获取到的陀螺仪数据除以57.1，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x58 陀螺仪量程为:±1000dps 获取到的陀螺仪数据除以28.6，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x5C 陀螺仪量程为:±2000dps 获取到的陀螺仪数据除以14.3，    可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x51 陀螺仪量程为:±4000dps 获取到的陀螺仪数据除以7.1，     可以转化为带物理单位的数据，单位为：°/s

    #define ACC_SPL  4098.0

// 设置为:0x30 加速度量程为:±2G      获取到的加速度计数据 除以16393，可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x38 加速度量程为:±4G      获取到的加速度计数据 除以8197， 可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x3C 加速度量程为:±8G      获取到的加速度计数据 除以4098， 可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x34 加速度量程为:±16G     获取到的加速度计数据 除以2049， 可以转化为带物理单位的数据，单位：g(m/s^2)
#endif

typedef struct
{
        short x;
        short y;
        short z;
}SI_F_XYZ;

typedef struct
{
  SI_F_XYZ deg_s;      //度每秒
  SI_F_XYZ acc_g;      //加速度

  float att_acc_factor;
  float att_gyro_factor;
  float roll;
  float pitch;
  float yaw;
}imu_data;


typedef struct
{
  SI_F_XYZ gyro;      //角速度
  signed char  offset_flag;
}imu_offset;

//欧拉角结构体
typedef struct
{
    double pitch;
    double roll;
    double yaw;
}eulerAngle_info_struct;

//四元数结构体
typedef struct {
    double q0;
    double q1;
    double q2;
    double q3;
}quater_info_struct;

//角速度漂移量
typedef struct
{
    double Xdata;
    double Ydata;
    double Zdata;
} gyroOffset_info_struct;

//加速度计速度漂移量
typedef struct
{
    double Xdata;
    double Ydata;
    double Zdata;
} accOffset_info_struct;

//三轴
typedef enum
{
    X,
    Y,
    Z,
    NUM_XYZ
}imu_info_enum;

//陀螺仪
typedef struct
{

    _Bool zero_calc_flag;//零漂计算标志

   double angle[NUM_XYZ];//简单积分得到的角度

   double last_angle[NUM_XYZ];//上一次角度

   double zero_angle[NUM_XYZ];//总漂移量

   double gyro[NUM_XYZ];//角速度

   int count;

}gyroscope_info_struct;

//加速度计
typedef struct
{
    double angle[NUM_XYZ];//加速度计得到的角度
    double acc[NUM_XYZ];//角加速度
}acc_info_struct;

//加速度计
typedef struct
{
    double mag[NUM_XYZ];//角加速度
}mag_info_struct;

//imu
typedef struct
{
    gyroscope_info_struct gyro; //陀螺仪
    acc_info_struct acc; //加速度计
    mag_info_struct mag;//磁力计
}imu_info_struct;

//最终角度
typedef struct
{
   double yaw;
   double pitch;
   double roll;
}complementation_info_struct;

//===================================================变量声明===================================================
extern eulerAngle_info_struct eulerAngle;
extern gyroOffset_info_struct gyroOffset;
extern accOffset_info_struct accOffset;
extern double imu_kp;                                             //加速度计的收敛速率比例增益
extern double imu_ki;                                             //陀螺仪收敛速率的积分增益
extern double I_ex, I_ey, I_ez;                                  // 误差积分
extern double ex, ey, ez;

extern short aacx, aacy, aacz;    // 加速度传感器原始数据
extern short gyrox, gyroy, gyroz; // 陀螺仪原始数据

extern imu_offset set;
extern imu_data imu1;    //陀螺仪数据存储
//===================================================变量声明===================================================


//===================================================函数声明===================================================
void ICM_Get_Raw_data(void);
void imu_task(void);
void Update_Angle(void);
void imu_data_deal(void);
void gyroOffsetInit(void);
void accOffsetInit(void);

float iir_lpf(float in,float out,float lpf_factor);
void get_iir_factor(float *out_factor,float Time, float Cut_Off);
float gyro_transition (signed short int  gyro_value);
void IIR_imu(void);
void IMU(void);
//===================================================函数声明===================================================




#endif

#include "zf_common_headfile.h"


/******************************变量定义************************************/
double q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // 初始位置姿态角为：0、0、0，对应四元数为：1、0、0、0
eulerAngle_info_struct eulerAngle;                            //欧拉角
gyroOffset_info_struct gyroOffset;
accOffset_info_struct accOffset;

double accoffsetx=120,accoffsety=-170,accoffsetz=-18;
double I_ex, I_ey, I_ez;                                  // 误差积分

//1ms
double imu_kp= 30;   //0.17   1.5                                            //加速度计的收敛速率比例增益
double imu_ki= 0.0008;        //0.004       0.0005                            //陀螺仪收敛速率的积分增益
float alpha = 0.35;  //0.35 低通滤波系数
float roll,pitch,yaw;
imu_info_struct imu;
float gzz1=0.0;

short aacx, aacy, aacz;    // 加速度传感器原始数据
short gyrox, gyroy, gyroz; // 陀螺仪原始数据
short zqx;
imu_data imu1 ;

imu_offset set ;

float gyro_x,gyro_y,gyro_z;//陀螺仪实际物理数据

/******************************变量定义************************************/




/**
  * @brief   IIR低通滤波器
  * @param   *acc_in 输入三轴数据指针变量
  * @param   *acc_out 输出三轴数据指针变量
  * @param   lpf_factor 滤波因数
  * @retval  x
  */
float iir_lpf(float in,float out,float lpf_factor)
{
    out = out + lpf_factor * (in - out);
  return out;
}

/*
 * 函数名：ICM_Get_Raw_data
 * 描述  ：获取陀螺仪六轴数据
 * 输入  ：角速度与加速度的原始数据
 * 返回  ：
 */
void ICM_Get_Raw_data(void){
//			imu963ra_get_acc_iic();                                                     // 获取 imu963ra 的加速度测量数值
      imu963ra_get_gyro_iic();                                                    // 获取 imu963ra 的角速度测量数值
}



/*
 * 函数名：get_iir_factor
 * 描述  ：求取IIR滤波器的滤波因子
 * 输入  ：out_factor滤波因子首地址，Time任务执行周期，Cut_Off滤波截止频率
 * 返回  ：
 */
void get_iir_factor(float *out_factor,float Time, float Cut_Off)
{
    *out_factor = Time /( Time + 1/(2.0f * PI * Cut_Off) );
}




//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 IMU660RA 陀螺仪数据转换为实际物理数据
// 参数说明     gyro_value              // 任意轴的陀螺仪数据
// 返回参数     void
// 使用示例     float data = imu660ra_gyro_transition(imu660ra_gyro_x);  // 单位为°/s
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float gyro_transition (signed short int  gyro_value)
{
    float gyro_data = 0;
    switch(GYR_SAMPLE)
    {
        case 0x00: gyro_data = (float)gyro_value / 16.4f;  break;  //  0x00 陀螺仪量程为:±2000dps      获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
        case 0x01: gyro_data = (float)gyro_value / 32.8f;  break;  //  0x01 陀螺仪量程为:±1000dps      获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
        case 0x02: gyro_data = (float)gyro_value / 65.6f;  break;  //  0x02 陀螺仪量程为:±500 dps      获取到的陀螺仪数据除以65.6          可以转化为带物理单位的数据，单位为：°/s
        case 0x03: gyro_data = (float)gyro_value / 131.2f; break;  //  0x03 陀螺仪量程为:±250 dps      获取到的陀螺仪数据除以131.2         可以转化为带物理单位的数据，单位为：°/s
        case 0x04: gyro_data = (float)gyro_value / 262.4f; break;  //  0x04 陀螺仪量程为:±125 dps      获取到的陀螺仪数据除以262.4         可以转化为带物理单位的数据，单位为：°/s
        default: break;
    }
    return gyro_data;
}

void IIR_imu(void)
{


//	for(signed char  i=0;i<=100;i++)//进行平均滤波，方便后续陀螺仪进行零漂处理
//	{
//			ICM_Get_Raw_data(); // 得到陀螺仪传感器数据

//		 set.gyro.x+=imu963ra_gyro_x_iic;
//		 set.gyro.y+=imu963ra_gyro_y_iic;
//		 set.gyro.z+=imu963ra_gyro_z_iic;
//	}
//	set.gyro.x/= 100;//陀螺仪去零漂，思想就是后续读取的值减去之前读取多少组数据的平均值
//	set.gyro.y/= 100;
//	set.gyro.z/= 100;
    set.offset_flag = 1;//读取数据完毕，标志位置为1
//    get_iir_factor(&imu1.att_acc_factor,0.005f,15);//求iir滤波器加速度因子，在IMU后续使用，很方便
    get_iir_factor(&imu1.att_gyro_factor,0.005f,10);//求iir滤波器角速度因子，在IMU后续使用。



}


void IMU(void)
{
    /*获取X、Y的角速度和加速度*/
    ICM_Get_Raw_data(); // 得到陀螺仪传感器数据

    /*滤波算法*/

    imu963ra_gyro_z_iic-=gyroOffset.Zdata;
	  
	  gzz1=imu963ra_gyro_z_iic;
 
    if(( imu963ra_gyro_z_iic > 10.0f) || ( imu963ra_gyro_z_iic < -10.0f)) //数据太小可以认为是干扰，不是偏航动作
    {
       yaw  -=  imu963ra_gyro_z_iic /14.3*0.005;//角速度积分成偏航角
    }
		else
		{
		imu963ra_gyro_z_iic=0;
		}
}



//陀螺仪解算姿态任务函数
void imu_task(void)
{
//    acc_get_angle();
//    gyro_get_angle();

    //数据处理
    imu_data_deal();

    //解算角度
    Update_Angle();
}

/*
 * @brief 计算陀螺仪零漂
 * 通过采集一定数据求均值计算陀螺仪零点偏移值。
 * 后可以用 陀螺仪读取的数据 - 零飘值，即可去除零点偏移量。
 */

void gyroOffsetInit(void)
    {
        gyroOffset.Xdata = 0;
        gyroOffset.Ydata = 0;
        gyroOffset.Zdata = 0;

        for (uint16_t i = 0; i < 500; ++i)
        {
            ICM_Get_Raw_data(); // 得到加速度传感器数据
            gyroOffset.Xdata += imu963ra_gyro_x_iic;
            gyroOffset.Ydata += imu963ra_gyro_y_iic;
            gyroOffset.Zdata += imu963ra_gyro_z_iic;
            system_delay_ms(5);

        }

        gyroOffset.Xdata /= 500;
        gyroOffset.Ydata /= 500;
        gyroOffset.Zdata /= 500;
    }
		
		
		
		
void accOffsetInit(void)
    {
      accOffset.Xdata = 0;
      accOffset.Ydata = 0;
      accOffset.Zdata = 0;

      for (uint16_t i = 0; i < 200; ++i)
      {
          ICM_Get_Raw_data(); // 得到加速度传感器数据
          accOffset.Xdata += imu963ra_acc_x_iic;
          accOffset.Ydata += imu963ra_acc_y_iic;
          accOffset.Zdata += imu963ra_acc_z_iic;
          system_delay_ms(5);
      }

      accOffset.Xdata /= 200;
      accOffset.Ydata /= 200;
      accOffset.Zdata /= 200;
   }


//imu数据处理
void imu_data_deal(void)
{
//    float alpha = 0.2;  //0.35


       //一阶低通滤波，单位g
      imu.acc.acc[X] =  (imu963ra_acc_x_iic - accOffset.Xdata) / ACC_SPL * alpha  + imu.acc.acc[X] * (1 - alpha);
      imu.acc.acc[Y] =  (imu963ra_acc_y_iic - accOffset.Ydata) / ACC_SPL * alpha  + imu.acc.acc[Y] * (1 - alpha);
      imu.acc.acc[Z] =  (imu963ra_acc_z_iic - accOffset.Zdata) / ACC_SPL * alpha  + imu.acc.acc[Z] * (1 - alpha);

      imu.gyro.gyro[X] = ANGLE_TO_RAD((imu963ra_gyro_x_iic - gyroOffset.Xdata) * GYRO_SPL);
      imu.gyro.gyro[Y] = ANGLE_TO_RAD((imu963ra_gyro_y_iic - gyroOffset.Ydata) * GYRO_SPL);
      imu.gyro.gyro[Z] = ANGLE_TO_RAD((imu963ra_gyro_z_iic - gyroOffset.Zdata) * GYRO_SPL);

}

/**
 * @brief 用互补滤波算法解算陀螺仪姿态(即利用加速度计修正陀螺仪的积分误差)
 * 加速度计对振动之类的噪声比较敏感，长期数据计算出的姿态可信；陀螺仪对振动噪声不敏感，短期数据可信，
 * 但长期使用积分误差严重(内部积分算法放大静态误差)。
 * 因此使用姿态互补滤波，短期相信陀螺仪，长期相信加速度计。
 */
double ex, ey, ez;     // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差
void Update_Angle(void)
{
    double halfT = 0.5 * delta_T;    // 采样周期一半

    double vx, vy, vz;     // 当前姿态计算得来的重力在三轴上的分量

    double q0q0 = q0 * q0;//先相乘，方便后续计算
    double q0q1 = q0 * q1;
    double q0q2 = q0 * q2;
    //double q0q3 = q0 * q3;//未使用
    double q1q1 = q1 * q1;
    //double q1q2 = q1 * q2;//未使用
    double q1q3 = q1 * q3;
    double q2q2 = q2 * q2;
    double q2q3 = q2 * q3;
    double q3q3 = q3 * q3;

    // 正常静止状态为-g 反作用力。
    if(imu.acc.acc[X] * imu.acc.acc[Y] * imu.acc.acc[Z] == 0) // 加速度计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
        return;

    // 对加速度数据进行归一化得到单位加速度
    //加速度计<测量>的重力加速度向量(机体坐标系)
    double norm = 1/sqrt(imu.acc.acc[X] *imu.acc.acc[X] + imu.acc.acc[Y]*imu.acc.acc[Y] + imu.acc.acc[Z]* imu.acc.acc[Z]);
    imu.acc.acc[X] = imu.acc.acc[X] * norm;
    imu.acc.acc[Y] = imu.acc.acc[Y] * norm;
    imu.acc.acc[Z] = imu.acc.acc[Z] * norm;

    //陀螺仪积分<估计>重力向量(机体坐标系)
    // 机体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    //将加速度计获得的重力向量归一化后的值与提取的姿态矩阵的重力向量叉乘获取姿态误差
    ex = imu.acc.acc[Y] * vz - imu.acc.acc[Z]* vy;
    ey = imu.acc.acc[Z] * vx - imu.acc.acc[X]* vz;
    ez = imu.acc.acc[X] * vy - imu.acc.acc[Y]* vx;

    //对误差进行积分
    I_ex += ex;
    I_ey += ey;
    I_ez += ez;
//    I_ex += halfT * ex;
//    I_ey += halfT * ey;
//    I_ez += halfT * ez;

    //将误差输入PID控制器后与陀螺仪测得的角速度相加，修正角速度值
    imu.gyro.gyro[X] = imu.gyro.gyro[X] + imu_kp* ex + imu_ki* I_ex;
    imu.gyro.gyro[Y] = imu.gyro.gyro[Y] + imu_kp* ey + imu_ki* I_ey;
    imu.gyro.gyro[Z] = imu.gyro.gyro[Z] + imu_kp* ez + imu_ki* I_ez;

    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2
    q0 = q0 + (-q1 * imu.gyro.gyro[X] - q2 * imu.gyro.gyro[Y] - q3 * imu.gyro.gyro[Z]) * halfT;
    q1 = q1 + ( q0 * imu.gyro.gyro[X] + q2 * imu.gyro.gyro[Z] - q3 * imu.gyro.gyro[Y]) * halfT;
    q2 = q2 + ( q0 * imu.gyro.gyro[Y] - q1 * imu.gyro.gyro[Z] + q3 * imu.gyro.gyro[X]) * halfT;
    q3 = q3 + ( q0 * imu.gyro.gyro[Z] + q1 * imu.gyro.gyro[Y] - q2 * imu.gyro.gyro[X]) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = 1/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;



    pitch  = RAD_TO_ANGLE(atan2(2 * q2 * q3 + 2* q0 * q1, q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3));//-180~180
    roll   = RAD_TO_ANGLE(asin(2 * q0 * q2 - 2 * q1 * q3));//-90~90
		
 yaw    = RAD_TO_ANGLE(atan2(2 * q1 * q2 + 2 * q0 * q3,  q0 * q0 +q1 * q1 - q2 * q2 - q3 * q3));//0~360//0~360 静止还在漂，用不了
		
//float yaw_G = imu1.deg_s.z;
//    if((yaw_G > 10.0f) || (yaw_G < -10.0f)) //数据太小可以认为是干扰，不是偏航动作
//    {
//       yaw  += yaw_G * 0.005f;//角速度积分成偏航角
//    }
}


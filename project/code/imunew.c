#include "zf_common_headfile.h"  

//*******************************************************//
float gps_ratio = 0.6;      // 加速度计比例
float gyro_ratio = 0.38;    // 陀螺仪比例
float dt = 0.005;           // 采样周期

float gx,gy,gz;
int8 start_flag=1;
int imu_flag=1;
float b_gx=0.0,b_gy=0.0,b_gz=0.0;
float p_gx=0.0,p_gy=0.0,p_gz=0.0;
int i=0;
float gxx,gyy,gzz;
float yaw_imu=0;

//*******************************************************//
//GPS采的点  放在这是因为计算角度的时候方便
//科目一目前就打算用两个点

/*不需要区分-180和+180，一直积分就行*/
void imu_read(void)
{
	imu963ra_get_gyro();
	gx=imu963ra_gyro_x;
	gy=imu963ra_gyro_y;
	gz=imu963ra_gyro_z;
	gxx=gx-p_gx;
	gyy=gy-p_gy;
	gzz=gz-p_gz;
	if(gzz>=10||gzz<=-10)
	{
		yaw_imu -= gzz/14.3*0.005;
	}
	else
		gzz = 0;			// 低通滤波
//	if(now_angle>360)
//	{
//		now_angle -=360;
//	}
//	if(now_angle<-360)
//	{
//		now_angle +=360;
//	}
//	quaternion_update();
//	EulerAngles angles = get_euler_angles();
//	now_angle= angles.yaw;
	
}
//陀螺仪校准函数
void imu_flag_(void)
{
	if(start_flag==1){i++;b_gx+=gx;b_gy+=gy;b_gz+=gz;}//start_flag默认置1  但是好像在主函数那面加了一个flag  这个貌似没什么用了
	if(i>=times&&imu_flag)
	{
		//计算平均值
		b_gx/=i;
		b_gy/=i;
		b_gz/=i;
		//转存平均值
		p_gx=b_gx;
		p_gy=b_gy;
		p_gz=b_gz;
		
		imu_flag=0;//标志位置零表示校准结束
		start_flag=0;//使在主函数那边不再进入这个函数  减少时间的浪费
	}
}
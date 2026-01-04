#include "zf_common_headfile.h"

#include <math.h>

uint8 drive_start_flag = 0;	// 电机启动信号
uint8_t Task3_Flag=0;
uint8_t Task3_Dir=1;
float Yaw_Angle_Temp=0.0;
uint8_t task456_start_flag=0;

_PID Roll_Inc_Pid;//横滚角
_PID Pitch_Inc_Pid;//俯仰角
PidTypeDef Yaw_Angle_Pid;//Yaw轴角度环PID，用于云台控制
uint16_t Angle_out=1;//Yaw轴角度环输出，用于直接控制云台
uint16_t Angle_Speed=1500;//角度环速度值

float Speed_Kp1=0.075;//小车行进速度前馈补偿
float Speed_Kp2=0;//0.00000700;//小车转弯速度前馈补偿
//    Speed_Kp2=0.000005;	

uint8_t Task4_Turn_flag=0;//转角
uint8_t Task4_Run_flag=0;//直线

int16_t RollX_step=100;
float errerr=0.0;//误差差值，KD项输入

float start_position,ture_position;//用于编码器前馈
//0--0.1--0.2--0.3--0.4--0.5--0.6--0.7--0.8--0.9
//直线1
static const float Run1[] =  {0.0,  0.2, 0.6, 0.9};// 80, 90, 100,  110,  120,  130,  140,  160};
static const float FB_table1   [] = {-0.058,-0.045, -0.020,  -0.010};//,  -0.028,  -0.015,  -0.013,  -0.010, -0.007,  -0.005};//  8,  6,   6,   1,     6,    0,    0,    0};
float map_speed1(float position)
{
    for (uint8_t i = 0; i < 10; i++)
        if (position <= Run1[i]) return FB_table1[i];
    return FB_table1[3];
}
//直线2
static const float Run2[] =  {0.0,  0.2, 0.6, 0.9};// 80, 90, 100,  110,  120,  130,  140,  160};
static const float FB_table2   [] = {-0.040,-0.040, -0.040,  -0.040};//,  -0.028,  -0.015,  -0.013,  -0.010, -0.007,  -0.005};//  8,  6,   6,   1,     6,    0,    0,    0};
float map_speed2(float position)
{
    for (uint8_t i = 0; i < 10; i++)
        if (position <= Run2[i]) return FB_table2[i];
    return FB_table2[3];
}
//直线3
static const float Run3[] =  {0.0,  0.2, 0.6, 0.9};// 80, 90, 100,  110,  120,  130,  140,  160};
static const float FB_table3   [] = {-0.015,-0.015, -0.025,  -0.04};//,  -0.028,  -0.015,  -0.013,  -0.010, -0.007,  -0.005};//  8,  6,   6,   1,     6,    0,    0,    0};
float map_speed3(float position)
{
    for (uint8_t i = 0; i < 10; i++)
        if (position <= Run3[i]) return FB_table3[i];
    return FB_table3[3];
}


float retarder_RollX(float input, float unit)
{
    static float output = 0;//相反则改变步长
//    if (input * unit < 0)
//        unit = unit * 0.3;
    if (unit == 0)
    {
        output = input;
        return input;
    }
    if (output > input) // 如果输入减小，则减少output
    {
        output -= unit; // 减少一个单位
        if (output < input)
            output = input; // 如果超调，则限制output
    }
    if (output < input) // 如果输入增加，则增加output
    {
        output += unit; // 增加一个单位
        if (output > input)
            output = input; // 如果超调，则限制output
    }
    return output;
}

//void Drive_PID_ControlX(void)
//{

//if(ANO_1==1)	//在屏幕左侧
//{
//Roll_Inc_Pid.err=(-(int32_t)(ANO_2));//减脉冲

//}

//else if(ANO_1==2)	//在屏幕右侧
//{
//Roll_Inc_Pid.err=ANO_2;//加脉冲

//}

//pid_incremental(&Roll_Inc_Pid);//增量式计算

//if(Roll_Inc_Pid.pwm_out>0)//输出是正
//{
//	Dir2=1;//偏航角，正向转
//	Angle2=Roll_Inc_Pid.pwm_out + Task3Off;
//}
//else if(Roll_Inc_Pid.pwm_out<=0)//输出是负
//{
//	Dir2=2;//反向转
//	Angle2=fabs(-(Roll_Inc_Pid.pwm_out - Task3Off));
//}
//}

//void Drive_PID_ControlY(void)
//{

//if(ANO_3==1)	//在屏幕上侧
//{
//Pitch_Inc_Pid.err=(-(int32_t)(ANO_4));//網上脉冲

//}

//else if(ANO_3==2)	//在屏幕下侧
//{
//Pitch_Inc_Pid.err=ANO_4;//加脉冲，往下

//}

//pid_incremental(&Pitch_Inc_Pid);//增量式计算

//if(Pitch_Inc_Pid.pwm_out>0)//输出是正
//{
//	Dir2=1;//偏航角，正向转
//	Angle1=Pitch_Inc_Pid.pwm_out ;
//}
//else if(Pitch_Inc_Pid.pwm_out<=0)//输出是负
//{
//	Dir2=2;//反向转
//	Angle1=fabs(-(Pitch_Inc_Pid.pwm_out ));
//}
//}

//void Drive_PID_Init(void)
//{
//	pid_Para_Init_Inc(&Roll_Inc_Pid,   -0.75,       0,    0,    2000,    65535);
//	pid_Para_Init_Inc(&Pitch_Inc_Pid,   0.55,       0,    0,    2000,    65535);
//}

void Drive_PID_ControlY(void)
{

if(ANO_3==1)	//在屏幕上侧
{
Pitch_Inc_Pid.err=(-(int32_t)(ANO_4-ANO4_Err));//網上脉冲

}

else if(ANO_3==2)	//在屏幕下侧
{
	
Pitch_Inc_Pid.err=(ANO_4-ANO4_Err);//加脉冲，往下

}

pid_incremental(&Pitch_Inc_Pid);//增量式计算

if(Pitch_Inc_Pid.pwm_out>0)//输出是正
{
	Dir1=1;//偏航角，正向转
	Angle1=Pitch_Inc_Pid.pwm_out ;
}
else if(Pitch_Inc_Pid.pwm_out<=0)//输出是负
{
	Dir1=2;//反向转
	Angle1=fabs(-(Pitch_Inc_Pid.pwm_out ));
}
}

//void Drive_PID_Init(void)
//{
//	pid_Para_Init_Inc(&Roll_Inc_Pid,   -0.75,       0,    0,    2000,    65535);
//	pid_Para_Init_Inc(&Pitch_Inc_Pid,   0.55,       0,    0,    2000,    65535);
//}

void Drive_PID_ControlX(void)
{

if(ANO_1==1)	//在屏幕左侧
{
	
Roll_Inc_Pid.err=ANO_2;//往左移,加上Err Kp

}

else if(ANO_1==2)	//在屏幕右侧 往右移动 减去Err Kp
{
	
Roll_Inc_Pid.err=(-(int32_t)(ANO_2));//往右移动

}

pid_incremental(&Roll_Inc_Pid);//位置式计算

if(Roll_Inc_Pid.pwm_out>0)//输出是正
{
	
	Dir2=1;//偏航角，逆时针转
	Angle2=Roll_Inc_Pid.pwm_out + Task3Off;
	
}
else if(Roll_Inc_Pid.pwm_out<=0)//输出是负
{
	Dir2=2;//正向转
	//Angle2=fabs(-(Roll_Inc_Pid.pwm_out - Task3Off));//适用于第三问
	Angle2=-Roll_Inc_Pid.pwm_out;
}
}

void Drive_PID_Init(void)
{
	pid_Para_Init_Inc(&Roll_Inc_Pid,   3,       0,    0,    2000,    65535);
	pid_Para_Init_Inc(&Pitch_Inc_Pid,   0.55,       0,    0,    2000,    65535);
	
	//pid_Para_Init(&Yaw_Angle_Pid,   2.5,  0,  0.00000100,  0,  100);//输出限幅，每次最多转6°---  10ms控制一次
  pid_Para_Init(&Yaw_Angle_Pid,   2.5,  0,  0,  0,  100);
}
//增量式
void pid_Para_Init_Inc(_PID* pid, float kp, float ki, float kd, float intemax, float max){

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral_max=intemax;
    pid->pwm_out_max=max;

}


//Yaw_Angle_Pid
/************角度环串级内环****************/
//位置式
void pid_Para_Init(PidTypeDef* pid, float kp, float ki, float kd, float intemax, float max){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->inteMax = intemax;
    pid->pwm_out_max = max;
    pid->inte = 0;
}

//uint8_t Task4_Turn_flag=0;//转角
//uint8_t Task4_Run_flag=0;//直线
void Task4_Change_PID(void)
{

if((Task4_Flag==1 || Task4_Flag==5)&&zhuanwan_flag==1&&(Task4_Turn_flag==0||Task4_Turn_flag==4))//---第一个转角
{
xishu1=0.056;
xishu2=0.056;
Roll_Inc_Pid.kp=3;	
Speed_Kp1=0;
//Speed_Kp2=0.0001;0.00000700	
Speed_Kp2=-0.0000130;
Yaw_Angle_Pid.kp=3;		
	
Task4_Turn_flag++;	
}

if((Task4_Flag==1 || Task4_Flag==5)&&zhuanwan_flag==0&& (Task4_Run_flag==0||Task4_Run_flag==4))//---直角边1 
{
xishu1=0.056;
xishu2=0.056;
Roll_Inc_Pid.kp=3;	
Speed_Kp1=FB_table1[0];
start_position=position;//基准位置
	
Speed_Kp2=0;
	
//Speed_Kp2=0.000005;	
Yaw_Angle_Pid.kp=2.5;	
	
Task4_Run_flag++;	
}

if((Task4_Flag==2 || Task4_Flag==6)&&zhuanwan_flag==1&&(Task4_Turn_flag==1||Task4_Turn_flag==5))//---第二个转角
{
Roll_Inc_Pid.kp=3;	
xishu1=0.056;
xishu2=0.056;
	
Speed_Kp1=0;
Speed_Kp2=0.000010;	
	
Yaw_Angle_Pid.kp=2.5;		
Task4_Turn_flag++;	
}

if((Task4_Flag==2 || Task4_Flag==6)&&zhuanwan_flag==0&& (Task4_Run_flag==1||Task4_Run_flag==5))//---直角边2 
{
Roll_Inc_Pid.kp=3;		
xishu1=0.056;
xishu2=0.056;
start_position=position;//基准位置	
Speed_Kp1=FB_table2[0];
Speed_Kp2=0;	
	
Yaw_Angle_Pid.kp=2.5;	
Task4_Run_flag++;	
}

if((Task4_Flag==3 || Task4_Flag==7)&&zhuanwan_flag==1&&(Task4_Turn_flag==2||Task4_Turn_flag==6))//---第三个转角
{
xishu1=0.056;
xishu2=0.056;
	
Speed_Kp1=0;
Speed_Kp2=0.000040;		
	
Yaw_Angle_Pid.kp=2.5;		
Task4_Turn_flag++;	
}

if((Task4_Flag==3 || Task4_Flag==7)&&zhuanwan_flag==0&& (Task4_Run_flag==2||Task4_Run_flag==6))//---直角边3
{
xishu1=0.056;
xishu2=0.056;
start_position=position;//基准位置	
Speed_Kp1=FB_table3[0];
Speed_Kp2=0;	
	
Yaw_Angle_Pid.kp=2.5;		
Task4_Run_flag++;	
}

if((Task4_Flag==4 || Task4_Flag==8)&&zhuanwan_flag==1&&(Task4_Turn_flag==3||Task4_Turn_flag==7))//---第四个转角
{
xishu1=0.056;
xishu2=0.056;
	
Speed_Kp1=0;
Speed_Kp2=0.000055;		
	
Yaw_Angle_Pid.kp=2.5;		
Task4_Turn_flag++;	
}

if((Task4_Flag==4 || Task4_Flag==8)&&zhuanwan_flag==0&& (Task4_Run_flag==3||Task4_Run_flag==7))//---直角边4 
{
xishu1=0.056;
xishu2=0.056;
start_position=position;//基准位置		
Speed_Kp1=0.1;
Speed_Kp2=0;	
	
Yaw_Angle_Pid.kp=2.5;	
	
Task4_Run_flag++;	
}

if(zhuanwan_flag==0 && (Task4_Run_flag==1||Task4_Run_flag==5))//---直角边1 
{
ture_position=position-start_position;//直角边1位置
	
Speed_Kp1=map_speed1(ture_position);

}

if(zhuanwan_flag==0 && (Task4_Run_flag==2||Task4_Run_flag==6))//---直角边2
{
ture_position=position-start_position;//直角边1位置
Speed_Kp1=map_speed2(ture_position);

}

if(zhuanwan_flag==0 && (Task4_Run_flag==3||Task4_Run_flag==7))//---直角边3 
{
ture_position=position-start_position;//直角边1位置
Speed_Kp1=map_speed3(ture_position);

}

if(zhuanwan_flag==0 && (Task4_Run_flag==4||Task4_Run_flag==8))//---直角边4 
{
ture_position=position-start_position;//直角边1位置
if(ture_position>=0.6)	
Speed_Kp1=0.075;

}



}


//Yaw_Angle_Pid.target = Roll_Velo_Pid_A.out;//目标值为视觉输出
void yaw_Cascade_Pid(void){
static float last_position,now_position;
	if(renwu>=4)
	{
	now_position=position;
	FB_Velo=(now_position-last_position)*1000;//计算真实浮点速度，提高精度
	last_position=now_position;
		
	Yaw_Angle_Pid.target=Yaw_Angle_Pid.target+FB_Velo*Speed_Kp1+gzz*Speed_Kp2;//速度前馈，每次计算利用编码器位置进行前馈;角速度前馈，每次计算利用陀螺仪角速度进行前馈，需要使用小车上的陀螺仪角速度。并用云台上的陀螺仪进行角度闭环（或许不需要云台上的陀螺仪？）
  }
	else
	{
	}
	pid_Locate(&Yaw_Angle_Pid, yaw);//目标值在视觉，真实值是云台上的陀螺仪数据
	
	
//	uint16_t Angle_out=Yaw_Angle_Pid.out;
	
	if(Yaw_Angle_Pid.out>0)//输出是正
{
	
	Dir2=2;//偏航角，正转
	Angle_out=Yaw_Angle_Pid.out;
	Angle_out=Angle_out;//+Speed_Kp2*Reality_Velocity;
	
}
else if(Yaw_Angle_Pid.out<=0)//输出是负
{
	Dir2=1;//偏航角，反转转
	
	Angle_out=-Yaw_Angle_Pid.out;
	Angle_out=Angle_out;//+Speed_Kp1*FB_Velo;
}

Drive_Move_speed(1,(uint8_t )Dir2,(uint16_t)Angle_out,Angle_Speed,0,UART_2);

}






/*
 * @breif: 积分限幅
 * */
void inte_Constrain(PidTypeDef* pid){
    if(pid->inte > pid->inteMax) pid->inte = pid->inteMax;
    if(pid->inte < -pid->inteMax) pid->inte = -pid->inteMax;

}


/*
 * @breif: 位置式pid
 * @para : real_value  传感器反馈值?
 * */
void pid_Locate(PidTypeDef * pid, float real_Value){
//误差
    pid->error = pid->target - real_Value;
	
//		if(pid->error<0)
//		pid->error=pid->error-Speed_Kp1*FB_Velo;
//		
//		if(pid->error>0)
//		pid->error=pid->error+Speed_Kp2*FB_Velo;
//		
	
//		if(pid->error>0)
//		pid->error=pid->error-Speed_Kp1*FB_Velo;
//    if(pid == &Pitch_Acc_Pid){          //行进轮，pid->error小于5滤去，避免微小误差累积造成积分饱和
//        if(pid->error > 3 || pid->error < -3){
//            pid->inte += pid->error;
//            //积分限幅
//            inte_Constrain(pid);
//        }
//    }
    //其他轮?，直接进行累加
//    else{
        pid->inte += pid->error;
//        积分限幅
            inte_Constrain(pid);
//    }
//初始化输出
    pid->out = 0;
//比例环节
    pid->out += pid->kp * pid->error;
//积分环节
    pid->out += pid->ki * pid->inte;
//微分环节--行进轮角度环，使用陀螺仪输出作为微分项
//    if(pid == &Pitch_Angle_Pid){
//        pid->out += pid->kd * icm_data_lpf.gyroX;
//    }
//微分环节--其他轮，使用两次误差之差  角速度作为微分项
    
    pid->out += pid->kd * gzz1;//(pid->error - pid->last_error);
    
//输出限幅
    if (pid->out > pid->pwm_out_max) {pid->out = pid->pwm_out_max;}
    else if (pid->out < -pid->pwm_out_max) {pid->out = -pid->pwm_out_max;}

//更新误差
    pid->last_error = pid->error;

}

/***********************/


void pid_incremental(_PID *pid_GO)
{
	
    // 计算当前误差
//    pid_GO->err = pid_GO->expect - pid_GO->feedback;
//pid_GO->err=0;
	errerr=pid_GO->kd * (pid_GO->err - pid_GO->err_last);
    // 计算控制量的增量
    float delta_pwm = pid_GO->kp * pid_GO->err           	
//		                  pid_GO->ki * pid_GO->err +
                      + pid_GO->kd * (pid_GO->err - pid_GO->err_last);

    // 更新输出
    pid_GO->pwm_out = delta_pwm;

    // 输出限幅
    if (pid_GO->pwm_out > pid_GO->pwm_out_max) {pid_GO->pwm_out = pid_GO->pwm_out_max;}
    else if (pid_GO->pwm_out < -pid_GO->pwm_out_max) {pid_GO->pwm_out = -pid_GO->pwm_out_max;}

    // 更新上一次的误差
    pid_GO->err_last = pid_GO->err;
//    pid_GO->err_last_last = pid_GO->err_last;
    //积分清零
//    if(car.i_clear == 1){
//        pid_GO->err =0 ;
//        pid_GO->pwm_out=0;
//        car.i_clear=0;
//    }

}

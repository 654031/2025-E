#ifndef DRIVECONTROL_H_
#define DRIVECONTROL_H_

#include "zf_common_headfile.h"

extern uint8 drive_start_flag;	// 电机启动信号

typedef struct{
   float err;
   float err_last;
   float err_last_last;
   float expect;
   float feedback;
   float kp;
   float ki;
   float kd;
   float integral;
   float integral_max;
   float pwm_out;
   float pwm_out_max;
   float mid_value;
} _PID;

typedef struct{
        float target;       //目标值?
        float kp;
        float ki;
        float kd;

        float inte;         //积分值?
        float inteMax;      //积分限幅

        float error;        //偏差
        float last_error;   //历史偏差

        float out;         //输出

        float pwm_out_max;

}PidTypeDef;


void pid_Para_Init_Inc(_PID* pid, float kp, float ki, float kd, float intemax, float max);
void pid_Para_Init(PidTypeDef* pid, float kp, float ki, float kd, float intemax, float max);
void pid_Locate(PidTypeDef * pid, float real_Value);//位置式PID
void pid_incremental(_PID *pid_GO);
void Drive_PID_Init(void);
void Drive_PID_ControlX(void);
void Drive_PID_ControlY(void);
float retarder_RollX(float input, float unit);
void yaw_Cascade_Pid(void);
void Task4_Change_PID(void);

#endif

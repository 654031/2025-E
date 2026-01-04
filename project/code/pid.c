#include "zf_common_headfile.h"


///*  Position_KP=5.0,Position_KI=0.0,Position_KD=0.0;           

//float Position_KP=8.0,Position_KI=0.0,Position_KD=3.0;          /* 位置式PID系数 */

////float Position_KP=8.0,Position_KI=0.01,Position_KD=3.0;          /* 位置式PID系数 */
float Position_KP=10.0,Position_KI=0.0,Position_KD=3.0;  //2
///*   Incremental_KP_2=0.2,Incremental_KI_2=0.025,Incremental_KD_2=0.04;  
//		Incremental_KP_2=0.10,Incremental_KI_2=0.025,Incremental_KD_2=0.07;
//		差不多的一组  /* 增量式PID系数 */

float Position_KP_2=2.0,Position_KI_2=0.0,Position_KD_2=0.0;  //	角速度位置式（未调参）

float Position_KP_4=18,Position_KI_4=0.0,Position_KD_4=0;  //	灰度位置式（已调参）PD改小了很多<-7.31错误还要大					用

float Incremental_KP_2=0.20,Incremental_KI_2=0.0,Incremental_KD_2=0.04;  // 角速度增量式 /* 增量式PID系数 */		用


////控制速度的

///*  Incremental_KP_1=14.0,Incremental_KI_1=2.8,Incremental_KD_1=1.8;   /* 增量式PID系数 */  

///* Incremental_KP_1=10.0,Incremental_KI_1=2.0,Incremental_KD_1=2.5;   速度11的  /* 增量式PID系数 */  

///*float Incremental_KP_1=8.0,Incremental_KI_1=1.0,Incremental_KD_1=2.5;   最好的一次表起来  */  
// 增量式速度
float Incremental_KP_1=8.0,Incremental_KI_1=1.0,Incremental_KD_1=2.5;																							//用
// 位置式速度
float Position_KP_3=2.0,Position_KI_3=0.0,Position_KD_3=0.0;		// 

//位置式角度环pid
float Position_PID(float reality,float target)
{ 	
    static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
    Bias=target-reality;                            /* 计算偏差 */
    Integral_bias+=Bias;	                        /* 偏差累积 */
    
    if(Integral_bias> 5000) Integral_bias = 5000;   /* 积分限幅 */
    if(Integral_bias<-5000) Integral_bias =-5000;
    
    Pwm = (Position_KP*Bias)                        /* 比例环节 */
         +(Position_KI*Integral_bias)               /* 积分环节 */
         +(Position_KD*(Bias-Last_Bias));           /* 微分环节 */
	
    Last_Bias=Bias;                                 /* 保存上次偏差 */
    return Pwm;                                     /* 输出结果 */
}

//位置式pid	用于角速度环
float Position_PID_2(float reality,float target)
{ 	
    static float Bias,Pwm,Last_Bias,Integral_bias=0;

	    if(reality>700)
		{
			reality = 700;
		}   
    if(reality<-700)
		{
			reality = -700;
		} 
    Bias=target-reality;                            /* 计算偏差 */
    Integral_bias+=Bias;	                        /* 偏差累积 */
    
    if(Integral_bias> 5000) Integral_bias = 5000;   /* 积分限幅 */
    if(Integral_bias<-5000) Integral_bias =-5000;
    
    Pwm = (Position_KP_2*Bias)                        /* 比例环节 */
         +(Position_KI_2*Integral_bias)               /* 积分环节 */
         +(Position_KD_2*(Bias-Last_Bias));           /* 微分环节 */
	
    Last_Bias=Bias;                                 /* 保存上次偏差 */
    return Pwm*8;                                     /* 输出结果 */
}
//位置式pid	用于速度环
int Position_PID_3(float reality,float target)
{ 	
    static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
    Bias=target-reality;                            /* 计算偏差 */
    Integral_bias+=Bias;	                        /* 偏差累积 */
    
    if(Integral_bias> 5000) Integral_bias = 5000;   /* 积分限幅 */
    if(Integral_bias<-5000) Integral_bias =-5000;
    
    Pwm = (Position_KP_3*Bias)                        /* 比例环节 */
         +(Position_KI_3*Integral_bias)               /* 积分环节 */
         +(Position_KD_3*(Bias-Last_Bias));           /* 微分环节 */
	
    Last_Bias=Bias;                                 /* 保存上次偏差 */
    return Pwm;                                     /* 输出结果 */
}

//用于速度控制的PID
int Incremental_PID_1(int reality,int target) //传入参数：当前速度，目标速度
{ 	
	 static float Bias_1,Pwm_1=0,Last_bias_1=0,Prev_bias_1=0;
    
	 Bias_1=(float)target-(float)reality;                                   /* 计算偏差 */
    
	 Pwm_1 += (Incremental_KP_1*(Bias_1-Last_bias_1))               /* 比例环节 */
           +(Incremental_KI_1*Bias_1)                           /* 积分环节 */
           +(Incremental_KD_1*(Bias_1-2*Last_bias_1+Prev_bias_1));  /* 微分环节 */ 
	
     Prev_bias_1=Last_bias_1;                                   /* 保存上上次偏差 */
	 Last_bias_1=Bias_1;	                                    /* 保存上一次偏差 */
	
	
	 return Pwm_1*4;                                            /* 输出结果 */
}
//串级PID里面的增量式PID
float Incremental_PID_2(float reality,float target) 
{ 	
	 static float Bias_2,Pwm_2=0,Last_bias_2=0,Prev_bias_2=0;
	   if(reality>600)
		{
			reality = 600;
		}   
    if(reality<-600)
		{
			reality = -600;
		}     
	 Bias_2=target-reality;                                   /* 计算偏差 */

    
	 Pwm_2 += (Incremental_KP_2*(Bias_2-Last_bias_2))               /* 比例环节 */
           +(Incremental_KI_2*Bias_2)                           /* 积分环节 */
           +(Incremental_KD_2*(Bias_2-2*Last_bias_2+Prev_bias_2));  /* 微分环节 */ 
    
     Prev_bias_2=Last_bias_2;                                   /* 保存上上次偏差 */
	 Last_bias_2=Bias_2;	                                    /* 保存上一次偏差 */
	 
	
	if(Pwm_2>500)
	{
		Pwm_2=500;
	}
	else if(Pwm_2<-500)
	{
		Pwm_2=-500;
	}
    
	 return Pwm_2*6.0;                                            /* 输出结果 */		// 从8->6
}

//位置式灰度环pid
float Position_PID_4(float reality,float target)
{ 	
    static float Bias,Pwm,Last_Bias,Integral_bias=0;
    
    Bias=target-reality;                            /* 计算偏差 */
    Integral_bias+=Bias;	                        /* 偏差累积 */
    
    if(Integral_bias> 5000) Integral_bias = 5000;   /* 积分限幅 */
    if(Integral_bias<-5000) Integral_bias =-5000;
    
    Pwm = (Position_KP_4*Bias)                        /* 比例环节 */
         +(Position_KI_4*Integral_bias)               /* 积分环节 */
         +(Position_KD_4*(Bias-Last_Bias));           /* 微分环节 */
	
    Last_Bias=Bias;                                 /* 保存上次偏差 */
    return Pwm*4;                                     /* 输出结果 */				// 从 8->2
}

int PwmXianfu(float dat,int min,int max)
{	
	if(dat<min)  dat=min;
    if(dat> max) dat= max;	
    return dat;
}

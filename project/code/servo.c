#include "zf_common_headfile.h"

#define PWM_CH1                 (PWM_TIM_A0_CH2_B4)//上舵机
#define PWM_CH2                 (PWM_TIM_A1_CH1_B5)//下舵机

#ifndef Servo_Mode_1


#define Servo1_max_duty          (820)  //250->535->820 === -90->0->90
#define Servo2_max_duty          (1250)  //250->750->1250 === -90->0->90

#define Servo_min_duty           (250)
	;
void servo_init()
{
	
    pwm_init(PWM_CH1, 50, 535);                                                // 初始化 上舵机为0度 
    pwm_init(PWM_CH2, 50, 750);                                                // 初始化 下舵机为0度

}


void servo_motor_a(int angle)                   //-90<angle<90
{
   uint32 duty_a=(angle+90)*(285)/90+250;
	if(duty_a>Servo1_max_duty)
	{
	   duty_a=Servo1_max_duty;
	}
	
	if(duty_a<Servo_min_duty)
	{
	   duty_a=Servo_min_duty ;
	}
   pwm_set_duty(PWM_CH1,duty_a);
  
}

void servo_motor_b(int angle)                  //-90<angle<90
{
   uint32 duty_b=(angle+90)*(500)/90+250;
	if(duty_b>Servo2_max_duty)
	{
	   duty_b=Servo2_max_duty;
	}
	
	if(duty_b<Servo_min_duty)
	{
	   duty_b=Servo_min_duty ;
	}
   pwm_set_duty(PWM_CH2,duty_b);


}          

#else

#define PWM_DUTY_MAX           ( 10000 )
#define Servo1_max_duty          (1250)  //250->750->1250 === -90->0->90
#define Servo2_max_duty          (1250)  //250->750->1250 === -135->0->135

#define Servo_min_duty           (250)
	;
void servo_init()
{
	
    pwm_init(PWM_CH1, 50, 750);                                                // 初始化 上舵机为0度,精度在2,即2/10000，0.36度
	                                                                             // 精度在2，即每动2个占空比，舵机会动一下
	
    pwm_init(PWM_CH2, 50, 750);                                                // 初始化 下舵机为0度,精度在2，即2/10000，0.54度
	                                                                             // 即每动2个占空比，舵机会动一下
                                                
}
    

void servo_motor_a(int angle)                   //-90<angle<90
{
   uint32 duty_a=(angle+90)*(500)/90+250;
	if(duty_a>Servo1_max_duty)
	{
	   duty_a=Servo1_max_duty;
	}
	
	if(duty_a<Servo_min_duty)
	{
	   duty_a=Servo_min_duty ;
	}
   pwm_set_duty(PWM_CH1,duty_a);
  
}

void servo_motor_b(int angle)                  //-135<angle<135
{
   uint32 duty_b=(angle+135)*(500)/135+250;
	if(duty_b>Servo2_max_duty)
	{       
	   duty_b=Servo2_max_duty;
	}
	
	if(duty_b<Servo_min_duty)
	{
	   duty_b=Servo_min_duty ;
	}
   pwm_set_duty(PWM_CH2,duty_b);

}          

//void Servo_control()


#endif


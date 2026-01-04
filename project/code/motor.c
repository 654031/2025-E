#include "zf_common_headfile.h"



void motor_init()
{
		// 电机初始化
		// 电机旋转方向初始化（默认释放）	
    gpio_init(MOTOR_AIN1, GPO, GPIO_LOW, GPO_PUSH_PULL);    
		gpio_init(MOTOR_AIN2, GPO, GPIO_LOW, GPO_PUSH_PULL);	  	
    gpio_init(MOTOR_BIN1, GPO, GPIO_LOW, GPO_PUSH_PULL);   
		gpio_init(MOTOR_BIN2, GPO, GPIO_LOW, GPO_PUSH_PULL);                                
   	   	 
		pwm_init(MOTOR_LT_PWM, 20000, 0);         // 频率我不知道设多少合适	20kHz 最合适 ！！      
    pwm_init(MOTOR_RT_PWM, 20000, 0);

}



/*
	与zf_driver_pwm.h中的	#define PWM_DUTY_MAX  ( 10000 )	保持一致	，尽量不损失精度
	uint16  match_temp = (uint16)(period_temp * ((float)duty / PWM_DUTY_MAX));  // 计算占空比
	left_pwm 	范围0~10000	
	right_pwm 范围0~10000
	时钟源选择80MHz，时钟分频系数为0，预分频系数也为0（不分频）
	重装载值是	3999	（80 000 000 / 1 / 1 / 20 000 = 4 000）
*/
void set_motor_pwm(int left_pwm,int right_pwm)
{
		if(left_pwm >= 0)	// 左电机正转
	  {
			gpio_set_level(MOTOR_AIN1,  1);
			gpio_set_level(MOTOR_AIN2,  0);
			pwm_set_duty(MOTOR_LT_PWM,left_pwm);	
		}
		else							// 左电机反转
	  {
			gpio_set_level(MOTOR_AIN1,  0);
			gpio_set_level(MOTOR_AIN2,  1);
			pwm_set_duty(MOTOR_LT_PWM,-left_pwm);
		}
	  if(right_pwm >= 0)	// 右电机正转
	  {
			gpio_set_level(MOTOR_BIN1,  1);
			gpio_set_level(MOTOR_BIN2,  0);
			pwm_set_duty(MOTOR_RT_PWM,right_pwm);
		}
		else							// 右电机反转
	  {
			gpio_set_level(MOTOR_BIN1,  0);
			gpio_set_level(MOTOR_BIN2,  1);
			pwm_set_duty(MOTOR_RT_PWM,-right_pwm);
		}					
}
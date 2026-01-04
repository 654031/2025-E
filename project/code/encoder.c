#include "zf_common_headfile.h"

// 编码器AB相脉冲输入

static int encoder_lt_count = 0;				// 左电机编码器计数	省得爆栈
static int encoder_rt_count = 0;				// 右电机编码器计数	省得爆栈

float position_lt = 0;				// 左电机编码器里程
float position_rt = 0;				// 右电机编码器里程

int speed_lt;	// 得写一个定时中断测速，过一段时间清空，做速度闭环，用来纠正编码器 每圈脉冲计数存在的正负72偏差
int speed_rt;


int Reality_Velocity;
float FB_Velo;//前馈速度
int drive_left;
int drive_right;

int turn_left;
int turn_right;
// 左右电机编码器的外部中断——检测A相的上跳沿
void encoder_lt_exti_handler (uint32 state, void *ptr);
void encoder_rt_exti_handler (uint32 state, void *ptr);
void enconder_v(void);

float position = 0;
float last_position = 0;
void encoder_init()
{
	// 编码器初始化
	// B相GPIO初始化
	gpio_init(E1B , GPI, GPIO_HIGH, GPI_PULL_UP);
	gpio_init(E2B , GPI, GPIO_HIGH, GPI_PULL_UP);
	
	//	A相设置为外部中断	
	exti_init(E1A , EXTI_TRIGGER_RISING,encoder_lt_exti_handler,NULL);
	exti_init(E2A , EXTI_TRIGGER_RISING,encoder_rt_exti_handler,NULL);
 
	interrupt_set_priority(GPIOB_INT_IRQn, 0); 		// GPIOB默认优先级是1 
}

void encoder_read(void)
{
	speed_lt = encoder_lt_count;
	speed_rt = encoder_rt_count;
	encoder_lt_count = 0;
	encoder_rt_count = 0;
	/*
		位置=圈数*周长  圈数=一段时间内编码器计数/(编码器线数*减速比) 单位：m
		MG513XP28	线数：13		减速比：28		轮子直径：65mm
	*/
	
	position_lt += (float) speed_lt/(13*28)*3.14*0.065;
	position_rt += (float) speed_rt/(13*28)*3.14*0.065;
	
	position = (position_lt+position_rt)/2;
	Reality_Velocity = (speed_lt+speed_rt)/2;
//	FB_Velo= (speed_lt+speed_rt)/2;
}

// 左后轮	E1A上跳沿
void encoder_lt_exti_handler (uint32 state, void *ptr)
{
   //E1B	左电机B相高电平	反转  
	if(1 == gpio_get_level(E1B))
	{
		encoder_lt_count++;
	}	  	
	else		
	{
		encoder_lt_count--;
	}
}

// 右后轮	E2A上跳沿
void encoder_rt_exti_handler (uint32 state, void *ptr)
{  
  //E2B	右电机B相高电平	正转
	if(1 == gpio_get_level(E2B))
	{
		encoder_rt_count--;	
	}	
	else
	{
		encoder_rt_count++;	  
	}		
}

/*大改速度环限幅，不让小车加速太快----7.31*/
void enconder_v(void)
{
	drive_left=700+Incremental_PID_1(Reality_Velocity,Target_Velocity);
	drive_left=PwmXianfu(drive_left,0,1300);
	drive_right=drive_left;	
	
//	// 位置式速度环
//	drive_left = Position_PID_3(Reality_Velocity,Target_Velocity);
//	drive_left=PwmXianfu(drive_left,1000,7000);
//	drive_right=drive_left;
}

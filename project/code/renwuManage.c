#include "zf_common_headfile.h"

uint8 page;
//新定义菜单
uint8 renwu = 1;//1 2 3 4 5 6
uint8 bianliang = 0;
uint8 car_delay_2s_flag = 0;			// 车等两秒再开


// 因为电机上电要自启动，（A30按下）drive_start_flag置位必须在拨动开关后按下之后
void startManage(void)	// 控制电机启动还是小车启动
{
	if(1 == renwu&&0 == car_start_flag)
	{
		car_start_flag = 1;
		drive_start_flag = 0;	
	}	
	else if(2 == renwu&&0 == drive_start_flag)//临时修改唯一
		
	{
		car_start_flag = 0;
		drive_start_flag = 1;	
	}
	else if(3 == renwu&&0 == drive_start_flag)
	{
		car_start_flag = 0;
		drive_start_flag = 1;	
	}
	else if(4 == renwu&&0 == car_start_flag)		// 第四问		车先不开
	{
		car_start_flag = 0;
		drive_start_flag = 1;	
		target_quanshu = 1;
	}	
	else if(5 == renwu&&0 == car_start_flag)		// 第5问		车先不开
	{
		car_start_flag = 0;
		drive_start_flag = 1;	
		target_quanshu = 1;
	}	
	else if(6 == renwu&&0 == car_start_flag)		// 第6问		车先不开
	{
		car_start_flag = 0;
		drive_start_flag = 1;	
		target_quanshu = 1;
	}	
}

void controlManage_IT(void)	// 控制调用哪一个问题	放中断
{
//	if((2 == renwu||3 == renwu)&&drive_start_flag == 1)
//	{
//									// 10ms中断任务	
//	if(jishu%50==0)
//	{
//		if(Task3_Flag==0)
//		{	
//			Drive_Move_speed(1,(uint8_t )Task3_Dir,50,120,100,UART_2);
//		}	
//	}
//	}
}

void controlManage_noIT(void);	// 控制调用哪一个问题（哪个control）不在中断	炫


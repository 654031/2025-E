#include "zf_common_headfile.h"

uint8 Key_Down;
uint8 Key_Up;
uint8 Key_Old;
uint8 Key_Flag;
uint32 KeyTick = 0;

// 0为逐飞，1为无名
void Key_Init(uint8 whichBoard)
{
	if(0 == whichBoard)
	{
		gpio_init(A30, GPI, 1, GPI_PULL_UP);
		gpio_init(A31, GPI, 1, GPI_PULL_UP);
		gpio_init(B0, GPI,  1, GPI_PULL_UP);
		gpio_init(B1, GPI,  1, GPI_PULL_UP);
	}
	else if(1 == whichBoard)
	{
		gpio_init(B21, GPI, 1, GPI_PULL_UP);
		gpio_init(B23, GPI, 1, GPI_PULL_UP);
		gpio_init(B24, GPI, 1, GPI_PULL_UP);
		gpio_init(B12, GPI, 1, GPI_PULL_UP);
		gpio_init(B8, GPI,  1, GPI_PULL_UP);
		gpio_init(B9, GPI,  1, GPI_PULL_UP);
		gpio_init(B10, GPI, 1, GPI_PULL_UP);
		gpio_init(B11, GPI, 1, GPI_PULL_UP);
	}
}

void Key_Scan(uint8 whichBoard)
{
	if(whichBoard == 0)
	{	
		Key_Down = Key_Temp_zf &(Key_Old^Key_Temp_zf);	// 下跳沿
		Key_Up = ~Key_Temp_zf &(Key_Old^Key_Temp_zf);		// 上跳沿
		Key_Old = Key_Temp_zf;	
	}
	else if(whichBoard == 1)
	{
		Key_Down = Key_Temp_wm &(Key_Old^Key_Temp_wm);	// 下跳沿
		Key_Up = ~Key_Temp_wm &(Key_Old^Key_Temp_wm);		// 上跳沿
		Key_Old = Key_Temp_wm;	
	}
}

void Key_Judge(uint8 whichBoard)
{
	Key_Scan(whichBoard);
	if(0 == whichBoard)
	{
		if(Key_Down & 0x01)  Key_Flag = 0x01 ; //		A30
		if(Key_Down & 0x02)  Key_Flag = 0x02 ; //		A31
		if(Key_Down & 0x04)  Key_Flag = 0x04 ; //		B0
		if(Key_Down & 0x08)  Key_Flag = 0x08 ; //		B1
	}
	else if(1 == whichBoard)
	{
		if(Key_Down & 0x01)  Key_Flag = 0x01 ; //		B21
		if(Key_Down & 0x02)  Key_Flag = 0x02 ; //		B23
		if(Key_Down & 0x04)  Key_Flag = 0x04 ; //		B24
		if(Key_Down & 0x08)  Key_Flag = 0x08 ; // 五向按键 上
		if(Key_Down & 0x10)  Key_Flag = 0x10 ; // 五向按键 下
		if(Key_Down & 0x20)  Key_Flag = 0x20 ; // 五向按键 左
		if(Key_Down & 0x40)  Key_Flag = 0x40 ; // 五向按键 右
		if(Key_Down & 0x80)  Key_Flag = 0x80 ; // 五向按键 中
	}
	
}

/****************************
A30:跑车
A31:切换任务
B0:切换控制的变量
B1:递增             暂时只有第一问的圈数控制	当且只有在任务1时有效
*****************************/


void key_pro(void)
{
			if(Key_Flag == 0x01)
			{
	//			set_motor_pwm(3000,3000);
				startManage();
				//拓展任务按下A30，等待发车
				if(renwu==4 && drive_start_flag == 1)
				{
				task456_start_flag= 1;	
				}
				if(renwu==5 && drive_start_flag == 1)
				{
				task456_start_flag= 1;
				}
				if(renwu==6 && drive_start_flag == 1)
				{
				task456_start_flag= 1;
				}
				
				Key_Flag = 0x00;
	//			set_motor_pwm(shuaige,-shuaige);
			}				
			if(Key_Flag == 0x02)
			{
	//			set_motor_pwm(-3000,-3000); 
				renwu++;
				if(renwu==4 && drive_start_flag == 0)
				{
				drive_start_flag = 1;	
				}
				if(renwu==5 && drive_start_flag == 0)
				{
				drive_start_flag = 1;	
				}
				if(renwu==6 && drive_start_flag == 0)
				{
				drive_start_flag = 1;	
				}
				
				bianliang = 0;
				if(renwu>6)
				{
					renwu=1;
				}
				Key_Flag = 0x00;
			}	
			if(Key_Flag == 0x04)
			{
				switch(renwu)
				{
					case 1:
					{
						bianliang++;
						if(bianliang>=3)
						{
							bianliang = 0;
						}
					}break;
					case 2:
					{
						bianliang++;
						if(bianliang>=3)
						{
							bianliang = 0;
						}
					}break;
					case 3:
					{
						bianliang++;
						if(bianliang>=3)
						{
							bianliang = 0;
						}
					}break;
					case 4:
					{
						bianliang++;
						if(bianliang>=5)
						{
							bianliang = 0;
						}
					}break;
					case 5:
					{
						bianliang++;
						if(bianliang>=3)
						{
							bianliang = 0;
						}
					}break;
					case 6:
					{
						bianliang++;
						if(bianliang>=3)
						{
							bianliang = 0;
						}
					}break;					
				}
				Key_Flag = 0x00;
			}	
			if(Key_Flag == 0x08)
			{
				switch(renwu)
				{
					case 1:
					{
						if(bianliang==0)
						{
							target_quanshu++;
							if(target_quanshu==6)
							{
								target_quanshu = 1;
							}
						}
					}break;
					
					case 2:
					{
						if(bianliang==0)
						{
							 Task3_Dir++;
							if(Task3_Dir==3)
							{
								Task3_Dir = 1;
							}
						}
							if(bianliang==1)
						 {
							ANO4_Err=ANO4_Err+1;
							if(ANO4_Err>=9)
							{
							ANO4_Err=3;
							}
					  }
						if(bianliang==2)
						 {
							Roll_Inc_Pid.kp=Roll_Inc_Pid.kp+0.5;
							if(Roll_Inc_Pid.kp>=3.5)
							{
							Roll_Inc_Pid.kp=2;
							}
					  } 
						
						
					}break;
					case 3:
					{
						
					}break;
					case 4:
					{
						  if(bianliang==0)
						  {
						  Quan1=Quan1+50;
							if(Quan1>=1800)
							{
							Quan1=800;
							}
					  	}
							
							if(bianliang==1)
						  {
							Quan2=Quan2+50;
							if(Quan2>=1800)
							{
							Quan2=800;
							}
					  	}
							
							if(bianliang==2)
						  {
							Quan3=Quan3+50;
							if(Quan3>=1800)
							{
							Quan3=800;
							}
					  	}
							
							if(bianliang==3)
						  {
							Quan4=Quan4+50;
							if(Quan4>=1800)
							{
							Quan4=800;
							}
						  }
							
						}
						
					break;
					case 5:
					{
						
					}break;
					case 6:
					{
						
					}break;					
					
				}
				Key_Flag = 0x00;
			}	
}

#include "zf_common_headfile.h"
int Sec=0;

loop_t loop; 
uint32_t time[10],time_sum;
extern uint8 pit_state ; 

uint8_t Time1=0;
uint8 data_buffer[32];
uint8 data_len;
uint8_t ANO4_Err=5;
float xishu1=0.056;
float xishu2=0.056;

uint8_t Speed=0;	// 第四问速度控制

void Loop_check(void)
{
if(pit_state){
	loop.cnt_1ms++;
	loop.cnt_2ms++;
	loop.cnt_4ms++;
	loop.cnt_5ms++;
	loop.cnt_6ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_1000ms++;

	if( loop.check_flag >= 1)
	{
		loop.err_flag ++;// 2ms 
	}
	else
	{
		loop.check_flag += 1;   //该标志位在循环后面清0
	}
	pit_state = 0; 
}
}

void main_loop()
{
	#ifndef UART1_DMA
  ANO_debug();
	#else
	ANO_Getdata(); 
  #endif
	Drive_Get();//UART_2
	Vofa_debug();//UART_0

	if( loop.check_flag >= 1 )
	{
		if( loop.cnt_1ms >= 1 )
		{
			loop.cnt_1ms = 0;
			
			Duty_1ms();	 					//周期1ms的任务
		}
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			
			Duty_2ms();	 					//周期2ms的任务
		}
		if( loop.cnt_4ms >= 4 )
		{
			loop.cnt_4ms = 0;
			Duty_4ms();						//周期4ms的任务
		}
				if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务
		}
		if( loop.cnt_6ms >= 6 )
		{
			loop.cnt_6ms = 0;
			Duty_6ms();						//周期6ms的任务
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//周期10ms的任务
		} 
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//周期20ms的任务
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//周期50ms的任务
		}
		if( loop.cnt_1000ms >= 1000)
		{
			loop.cnt_1000ms = 0;
			Duty_1000ms();				//周期1s的任务
		}
		loop.check_flag = 0;		//循环运行完毕标志
	}
}

/////////////////////////////////////////////////////////
void Duty_1ms()
{
//ANO_polling();
}

/////////////////////////////////////////////////////////
void Duty_2ms()
{
	
if((2 == renwu||3 == renwu)&&drive_start_flag == 1)
{
if(Flag==1 && ANO_Flag==0)
{
// Time1=1;	
	
Flag=0;	 
if(ANO_1!=0)//如果发现方框
		{	
			Task3_Flag=1;
		}
		
		if(Task3_Flag==1)
		{
			
//		if(ANO_2<=3 && (ANO_4 -ANO4_Err) <=3)
//		{
//		gpio_set_level(B4, 1);//如果到達指定位置，開激光
//		}
//		
//		else if(ANO_2>3 || (ANO_4 - ANO4_Err)>3)
//		{
//			
//		gpio_set_level(B4, 0);
//		}
			
		if(ANO_2<=3 )
		{
		gpio_set_level(B4, 1);//如果到達指定位置，開激光
		}
		
		else if(ANO_2>3 )
		{
		gpio_set_level(B4, 0);
		}
		
//		Roll_Inc_Pid.kp=2.5;//不加角度环控制频率不够
		
//Drive_PID_ControlX();//計算	
//Drive_PID_ControlY();//計算	
		
//Drive_Move_speed(1,(uint8_t )Dir2,(uint16_t)Angle2,30,0,UART_2);
		
//Drive_Move_speed(1,(uint8_t )Dir1,(uint16_t)Angle1,10,0,UART_0);		
		Drive_PID_ControlX();//計算	
//Angle2=Angle2+Reality_Velocity;
if(Dir2==1)//往左
{
float Angle2_temp=Angle2*xishu1;	
Yaw_Angle_Temp=yaw-Angle2_temp;//角度环目标
Yaw_Angle_Pid.target=Yaw_Angle_Temp;
}

else if(Dir2==2)//往右
{
float Angle2_temp=Angle2*xishu2;	
Yaw_Angle_Temp=yaw+Angle2_temp;//角度环目标
Yaw_Angle_Pid.target=Yaw_Angle_Temp;	
}
	}	
}
}

if((5 == renwu||4 == renwu)&&drive_start_flag == 1)//视觉环
{
if(Flag==1 && ANO_Flag==0)
{
// Time1=1;	
	
Flag=0;	 
//if(ANO_1!=0)//如果发现方框
//		{	
//			Task3_Flag=1;
//		}
		
//		if(Task3_Flag==1)
//		{
//		if(ANO_2<=3 && ANO_4<=3)
//		{
//		gpio_set_level(B4, 1);//如果到達指定位置，開激光
//		}
//		
//		else if(ANO_2>3 || ANO_4>3)
//		{
//		gpio_set_level(B4, 0);
//		}
			
//Roll_Inc_Pid.kp=3;//vofa调参时注释此行
//gpio_set_level(B4, 1);//调试时加上此行

Drive_PID_ControlX();//計算	
//Angle2=Angle2+Reality_Velocity;
if(Dir2==1)//往左
{
float Angle2_temp=Angle2*xishu1;	
Yaw_Angle_Temp=yaw-Angle2_temp;//角度环目标
Yaw_Angle_Pid.target=Yaw_Angle_Temp;
}

else if(Dir2==2)//往右
{
float Angle2_temp=Angle2*xishu2;	
Yaw_Angle_Temp=yaw+Angle2_temp;//角度环目标
Yaw_Angle_Pid.target=Yaw_Angle_Temp;	
}
//Drive_PID_ControlY();//計算	
		
//Drive_Move_speed(1,(uint8_t )Dir2,(uint16_t)Angle2,30,0,UART_2);
		
//Drive_Move_speed(1,(uint8_t )Dir1,(uint16_t)Angle1,10,0,UART_0);		
//	}	
}
}

//	if(!Drive_Flag2)
//	{
//  Drive_Move(1,(uint8_t )Dir2,(uint16_t)Angle2,60,60,UART_2);
//  Drive_Flag2=1;
//	}
}

/////////////////////////////////////////////////////////
void Duty_4ms()
{
//Vofa_debug();		//上位机数据处理
//Drive_PID_ControlX();	
//ANO_polling();	


}		

/////////////////////////////////////////////////////////
void Duty_5ms()
{

}

/////////////////////////////////////////////////////////
void Duty_6ms()					// 任务选择和执行
{
//	IMU_select(IMU963RA);	
//	if((1 == car_start_flag)&&(0 == stop_flag))
//	{
////		control_test_1();
//		enconder_v();
//		imu_control();
////		control_test_1();
//		if(1 == renwu)//临时
//		{
//			control1();			// 放非中断6ms
//		}
//		else if(4 == renwu)
//		{
//			control_2();			// 放非中断6ms
//		}
//		else if(5 == renwu)
//		{
//			control_3();			// 放非中断6ms
//		}		
//		else if(6 == renwu)
//		{
//			control_4();			// 放非中断6ms
//		}		
//	}
}

/////////////////////////////////////////////////////////
void Duty_10ms()
{
	key_pro();	
}

/////////////////////////////////////////////////////////
void Duty_20ms()
{
	
/**************************ANO****************************/			
//	ANO_polling();
//	if(!Drive_Flag1)
//  {
//		
//	Drive_Move(1,(uint8_t )Dir1,(uint16_t)Angle1,60,120,UART_0);
//	Angle1=Angle1+200;
//	Angle1%=3000;	
//	Drive_Flag1=1;
//		
//	}	
//		if(!Drive_Flag2)
//  {
		
//	Angle2=Angle2+500;
//	Angle2%=3000;		
//	Drive_Flag2=1;
//		
//	}	
	display_pro();
}

/////////////////////////////////////////////////////////
void Duty_50ms()
{
//Drive_Move(1,(uint8_t )Dir2,(uint16_t)Angle2,60,0,UART_2);	

//	    wireless_uart_send_byte('\r');
//    wireless_uart_send_byte('\n');
//    wireless_uart_send_string("SEEKFREE wireless uart demo.\r\n");              // 初始化正常 输出测试信息
//	 wireless_uart_send_string("SEEKFREE wireless uart demo.\r\n");    
//		sprintf((char *)data_buffer,"%.2f,%d,%d,%d,%d\r\n", position,Reality_Velocity,new_drive_left,new_drive_right,drive_left);
//		wireless_uart_send_buffer(data_buffer, strlen((char *)data_buffer)); 
}

/////////////////////////////////////////////////////////
void Duty_1000ms()
{
	//Sec++;
//	static int i=0;
//	i++;
//	i=i%2;
//	if(i)
//Drive_Move(1,(uint8_t )1,(uint16_t)300,60,120,UART_0);
//else
//Drive_Move(1,(uint8_t )2,(uint16_t)500,60,120,UART_0);	
	last_angle = yaw_imu;	// 记录1秒前的角度				找到bug了
}
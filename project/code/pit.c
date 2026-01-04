#include "zf_common_headfile.h"

uint16_t Task3Off =0;
uint8 pit_state = 0;
uint8 jishu = 0;

uint8 jishu_2s = 0;

float last_angle_zhuan;
uint16_t Time=5;
void pit_handler (uint32 state, void *ptr);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的周期中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c1
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit_handler (uint32 state, void *ptr)
{
	pit_state = 1;                                                              // 周期中断触发 标志位置位
//	if(imu_flag==0)
//	{
	jishu++;
	controlManage_IT();	// 控制调用哪一个问题（哪个control）	针对启炫的中断开机
	if(jishu%1==0)										// 1ms中断任务
	{
		Task4_Change_PID();
		Key_Judge(0);										// 按键检测
	
		/*****************xuan*******************/
//		if(ANO_1!=0)										// 如果发现方框
//		{	
//			Task3_Flag=1;
//		}
		/*****************xuan*******************/
	}
	if(jishu%2==0)										// 2ms中断任务	
	{
		read_track_pin();								
		grayscale_get_2(track_arr);		
		/*****************car*******************/
		if(1 == car_start_flag)					// 只有车启动才能巡线转弯
		{			
			if(xun<1)													// 必须够快
			{			
				if(zhuanwan_flag==0&&(position-last_position)>0.7)		// 重要参数，当小车差不多到点 
				{
					last_position = position;				// 差不多是1m
					zhuanwan_flag = 1;							// 光是巡不到黑线说明不了车的位置
					Task4_Flag_New++;
					last_angle_zhuan = now_angle;
					
				}			
			}
			if(zhuanwan_flag==1)
			{

				if(now_angle-last_angle_zhuan<-75&&xun>=1)	// 重要参数得反复调	 
				{//&&now_angle>(-90*zhuanjiao_cnt-2 -quanshu)
					zhuanwan_flag = 0;
					Task4_Flag_Old++;
					last_zhuanjiao_cnt = zhuanjiao_cnt;
					zhuanjiao_cnt = zhuanjiao_cnt+1;

					drive_left = 700;
					drive_right = 700;					
					if((renwu>=4)&&((last_zhuanjiao_cnt)%4 ==0) && ((zhuanjiao_cnt)%4 == 1)&&(position>0.67))//扩展
					{
						quanshu++;
					}
					if((renwu<4)&&(last_zhuanjiao_cnt)%4 ==3 && (zhuanjiao_cnt)%4 == 0) //基础
					{
						quanshu++;
					}
				}
				
			}				
		}	
		/*****************car*******************/
	}	
	if(jishu%5==0)										// 5ms中断任务	
	{
    IMU();
		
		imu_read();			// 获取imu角度
		encoder_read(); // 编码器计算位置
		
	if((2 == renwu||3 == renwu)&&drive_start_flag == 1)
	{		
		if(Task3_Flag==0)
		{	
			Drive_Move_speed(1,(uint8_t )Task3_Dir,ANO4_Err,Angle_Speed,0,UART_2);
		}//Drive_Move_speed(1,(uint8_t )Dir2,(uint16_t)Angle_out,Angle_Speed,0,UART_2);
		else{
		yaw_Cascade_Pid();
		}
	}
		if(4==renwu && drive_start_flag == 1)
	{
yaw_Cascade_Pid();//角度环
//	gimbal_track_step();
	}
if(5==renwu && drive_start_flag == 1)
	{
yaw_Cascade_Pid();//角度环
//	gimbal_track_step();
	}
	}
	if(jishu%6==0)
	{
	IMU_select(IMU963RA);	
	if((1 == car_start_flag)&&(0 == stop_flag))
	{
//		control_test_1();
		enconder_v();
		imu_control();
//		control_test_1();
		if(1 == renwu)//临时
		{
			control1();			// 放非中断6ms
		}
		else if(4 == renwu)
		{
			control_2();			// 放非中断6ms
		}
		else if(5 == renwu)
		{
			control_3();			// 放非中断6ms
		}		
		else if(6 == renwu)
		{
			control_4();			// 放非中断6ms
		}		
	}
	}
	
	if(jishu%10==0)										// 10ms中断任务	
	{
		/*****************xuan*******************/
//		if(Flag==1)
//		{		
//		Flag=0;
//		Drive_PID_ControlX();
//		Drive_PID_ControlY();		// 8/1 v2
////	Angle2 = Roll_Inc_Pid.pwm_out + Single;
//		}
		
//if(4==renwu && drive_start_flag == 1)
//	{
//yaw_Cascade_Pid();//角度环
////	gimbal_track_step();
//	}
//if(5==renwu && drive_start_flag == 1)
//	{
//yaw_Cascade_Pid();//角度环
////	gimbal_track_step();
//	}
		/*****************xuan*******************/
		
	}

	if(jishu%30==0)		// 30ms中断任务	
	{
		/*****************xuan*******************/
//		if(Task3_Flag==1)
//		{
//			Drive_Move(1,(uint8_t )Dir1,(uint16_t)Angle2,120,100,UART_2);	//橫滾鎖定
//			
//			Drive_Move(1,(uint8_t )Dir2,(uint16_t)Angle1,100,100,UART_0);//俯仰鎖定		// 8/1 v2
////			Drive_Flag1=1;
//		}	
		/*****************xuan*******************/
	}
	
	if(jishu%50==0)		// 50ms中断任务	
	{
		jishu=0;
//		if(!Drive_Flag1 && Task3_Flag==0)
//		{	
//			Drive_Move(1,(uint8_t )Dir1,(uint16_t)Task3Off,60,120,UART_2);
//			Task3Off=Task3Off+200;//32
//			Task3Off=Task3Off%3000;	
//			Drive_Flag1=1;
//		}
		/*****************xuan*******************/
//		if(Task3_Flag==0)
//		{				
//			Drive_Move(1,(uint8_t )Dir1,(uint16_t)Task3Off,100,100,UART_2);//橫滾尋找
//			Task3Off=Task3Off+40;//32
//			Drive_Flag1=1;
//		}
		/*****************xuan*******************/
		
		if(renwu>=4&&drive_start_flag==1&& task456_start_flag==1 &&car_delay_2s_flag == 0)
		{
			jishu_2s++;
			if(jishu_2s>40)
			{
				car_delay_2s_flag = 1;
				car_start_flag = 1;				
			}		
		}
		if(car_delay_2s_flag == 1)
		{
			jishu_2s = 0;
		}	
	}
}
void pit_1ms_init(void){

 pit_ms_init(PIT_TIM_G12, 1 , pit_handler , NULL);                              // 初始化 PIT_CH0 为周期中断 1ms 周期
	
 interrupt_set_priority(PIT_PRIORITY, 7);                                    // 设置 PIT1 对周期中断的中断优先级为 7
}


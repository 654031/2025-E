#include "zf_common_headfile.h" 


int8 qqqqqq;

uint16_t Task4_Flag_Old;
uint16_t Task4_Flag_New;
uint16_t Task4_Flag;

uint16_t Quan1=1100;
uint16_t Quan2=1000;
uint16_t Quan3=1000;
uint16_t Quan4=1000;


float last_angle = 0.0;
float now_angle=0.0;
float target_angle=0.0;
float current_x,current_y,target_x,target_y;

int error_pwm;
int new_drive_left;
int new_drive_right;
float error_pwm_1;
uint8 stop_flag = 0;
uint8 car_start_flag = 0;
uint8 chuanji_cnt=0;


uint8 zhuanwan_flag = 0;	// 1为左转，2为右转

uint8 quanshu = 1;
uint8 target_quanshu = 0;

uint8 zhuanjiao_cnt = 0;
uint8 last_zhuanjiao_cnt = 0;
float zhuanwan_angle=0.0;
float jifen_angle=0.0;
float xun_buchang;

int basic_speed=7;//慢慢的速度  作为基础速度
int high_speed=2;//慢慢的速度  作为基础速度

float sudu_xiuzheng=1;

int Target_Velocity;
float v_angle;

uint8 neihuan_flag = 0;	// 0为增量式，1为位置式
uint8 task1_flag = 0;

/*********************************发挥部分*******************************/
//提高任务1

uint8 taskflag2_1 = 0;
uint8 taskflag2_2 = 0;
uint8 taskflag2_3 = 0;

uint8 target_quanshu_2_1 = 1;
uint8 kuozhan=0;

uint8 speed_chushi=0;
uint8 zhuanjiao_buchang=0;

uint8 zhuanwan_kaishi = 0;

//靶的位置坐标
float x0=50.0;
float y0=150.0;
float z0=40.0;

//移动位置(上电前赋值）
float x1;//zhixian
float y1;
float z1;

float zhuanwan_x;//zhuanwna
float zhuanwan_y;

float zuizhong_x;
float zuizhong_y;
//修正数值
float d=24.0; //车长   
float m=6.0; //圆台偏心  

float h1=14;//轴高
float h2=10;//臂长


float yaw_deliver=0.0;
float pitch_deliver=0.0;
float last_pitch_deliver=0.0;
/**********************************************************************/

//*******************************************************//
// 第一问
void control1()
{
	if(stop_flag==0&&car_start_flag == 1)
	{
		switch(task1_flag)
		{
			case 0:
			{
				Target_Velocity = 2;
				if(quanshu>target_quanshu)
				{
					task1_flag = 1;
				}
			}
			break;
			case 1:
			{
				stop_flag = 1;
				car_start_flag = 0;
				///bibijiao{}
			}
			break;	
		}
	}
}

/********************************发挥部分*********************************/
//static const uint8_t an2_threshold[] =  {4,  8, 16, 24, 32, 40, 48, 56,  64,  72, 80, 90, 100,  110,  120,  130,  140,  160};
//static const uint8_t speed_table   [] = {0,  0,  1,  5,  2,  8,  9,  10, 12,   6,  8,  6,   6,   1,     6,    0,    0,    0};
//uint8_t map_speed(uint8_t ano)
//{
//    for (uint8_t i = 0; i < sizeof(an2_threshold); i++)
//        if (ano <= an2_threshold[i]) return speed_table[i];
//    return speed_table[sizeof(speed_table)-1];
//}
///提高扩展需要将初始的position赋值为一个-0.85以上左右，让不然逆行没有转弯
// 扩展第一问
void control_2()
{
//					if(position>=0.15)
//					{
//					Drive_Move_speed(1,2,1000,50,0,UART_2);
//					Drive_Flag1=1;	
//					}
//					
//					if(position>=1.10)
//					{
//					
//					Drive_Move_speed(1,2,800,50,0,UART_2);
//					Drive_Flag1=1;		
//					}
//					if(position>=2.20)
//					{
//					
//					Drive_Move_speed(1,2,800,50,0,UART_2);
//					Drive_Flag1=1;		
//					}
//					if(position>=3.20)
//					{
//					Drive_Move_speed(1,2,800,50,0,UART_2);
//					Drive_Flag1=1;	
//					}
		


	if(stop_flag==0&&car_start_flag == 1)
	{
		switch(taskflag2_1)
		{
case 0:
			{
if(zhuanwan_flag==1)
{

if(zhuanjiao_cnt==0 && Task4_Flag==0)
{
	
Task4_Flag++;	
	
}

if(zhuanjiao_cnt==1 && Task4_Flag==1)
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==2 && Task4_Flag==2)
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==3 && Task4_Flag==3)
{
	
Task4_Flag++;
	
}
//Task4_Change_PID();
}
				gpio_set_level(B4, 1);
				Target_Velocity = high_speed;
				if(quanshu>=target_quanshu&&position>3.8)
				{
					taskflag2_1 = 1;
				}
				
//				/*************************xuan****************/
//				if(Flag==1 && ANO_Flag==0)//如果读取到一帧
//				{
//						Flag=0;	 
//					if(zhuanwan_flag==0 && Drive_Flag1==0)//直行并且横滚角转角到位时
//					{						
//							speed_chushi = map_speed(ANO_2);
//							Speed = speed_chushi+zhuanjiao_buchang;
//						if(ANO_1==1)	//在屏幕左侧
//						{
//							Dir2=1;	
//						}
//						else if(ANO_1==2)	//在屏幕右侧 往右移动 减去Err Kp
//						{
//							Dir2=2;
//						}
//						Drive_Move_speed(1,(uint8_t )Dir2,(uint16_t)10000,Speed,0,UART_2);
//					}
//					
//				}

		
				/*************************xuan****************/
			}
			break;
			case 1:
			{
				stop_flag = 1;
				car_start_flag = 0;
			}
			break;	
		}
	}
	
}

// 扩展第二问
void control_3()
{
	if(stop_flag==0&&car_start_flag == 1)
	{
		switch(taskflag2_2)
		{
			case 0:
			{	
				
if(zhuanwan_flag==1)
{

if(zhuanjiao_cnt==0 && Task4_Flag==0)
{
	
Task4_Flag++;	
	
}

if(zhuanjiao_cnt==1 && Task4_Flag==1)
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==2 && Task4_Flag==2)
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==3 && Task4_Flag==3)
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==4 && Task4_Flag==4)//第二圈
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==5 && Task4_Flag==5)//第二圈
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==6 && Task4_Flag==6)//第二圈
{
	
Task4_Flag++;
	
}

if(zhuanjiao_cnt==7 && Task4_Flag==7)//第二圈
{
	
Task4_Flag++;
	
}

//Task4_Change_PID();
}
gpio_set_level(B4, 1);
				Target_Velocity = high_speed;
				if(quanshu>2||position>7.6)
				{
					taskflag2_2 = 1;
				}
			}
			break;
			case 1:
			{
				stop_flag = 1;
				car_start_flag = 0;
				///bibijiao{}
			}
			break;	
		}
	}
}

// 扩展第三问
void control_4(void)
{
	if(stop_flag==0&&car_start_flag == 1)
	{
		switch(taskflag2_3)
		{
			case 0:
			{
				gpio_set_level(B4, 1);
				if(zhuanwan_flag==1)
{

if(zhuanjiao_cnt==0 && Task4_Flag==0)
{
	
Drive_Move_speed(1,2,1100,10,0,UART_2);
Task4_Flag++;	
}

if(zhuanjiao_cnt==1 && Task4_Flag==1)
{
Drive_Move_speed(1,2,1000,10,0,UART_2);
Task4_Flag++;
}

if(zhuanjiao_cnt==2 && Task4_Flag==2)
{
	
Drive_Move_speed(1,2,800,10,0,UART_2);
Task4_Flag++;
}

if(zhuanjiao_cnt==3 && Task4_Flag==3)
{
	
Drive_Move_speed(1,2,800,10,0,UART_2);
Task4_Flag++;
	
}
//if(zhuanjiao_cnt==4 && Task4_Flag==4)
//{
//	
//Drive_Move_speed(1,2,800,30,0,UART_2);
//	
//}

}
				Target_Velocity = high_speed;
				if(quanshu>1)
				{
					taskflag2_3 = 1;
				}
			}
			break;
			case 1:
			{
				stop_flag = 1;
				car_start_flag = 0;
				///bibijiao{}
			}
			break;	
		}
	}
}

//计算步进电机目标转度
void jiesuan(void)
{
	if(zhuanwan_flag==0)
	{
		if(position<1)
		{
			if(position*100>0&&position<(d/2-m))//-100是为了与之前转弯匹配
			{
				y1 = 100.0;
				x1=d/2-m-position*100;
			}
			else if(position>(d/2-m))
			{
				y1 = 100 - 100*position;
				x1 = 0.0;	
			}
		}
		else if(position<2)
		{
			x1 = (position-1)*100;
			y1 = 0;
		}
		else if(position<3)
		{
			x1 = 100;
			y1 = position*100 - 200;
		}
		else if(position<4)
		{
			x1 = 400 - position*100;
			y1 = 100;
		}
		zhuanwan_x=0;
		zhuanwan_y=0;
	}
	else
	{
		zhuanwan_x=cos(now_angle*3.1415926/180.0)*m;
		zhuanwan_y=sin(now_angle*3.1415926/180.0)*m;
	}
	zuizhong_x = -zhuanwan_x+x1;
	zuizhong_y = zhuanwan_y+y1;
	
//z1的处理
	last_pitch_deliver= yaw_deliver;
	z1 = h1 + h2*cos(last_pitch_deliver*3.1415926/180.0);
	
	yaw_deliver = atan2f(y0-y1,x0-zuizhong_x);
	pitch_deliver = atan2f(z0-z1,sqrt(pow(zuizhong_x-x0,2)+pow(y1-y0,2)));
	
	
}

/************************************************************************************************/

void IMU_select(uint8 whichIMU)
{
	if(whichIMU == IMU963RA)
	{
		v_angle = gzz;
		now_angle = yaw_imu;
	}
}


///外环角度环，内环角速度环
void imu_control(void)
{	
	//检查

	if(whichIMU == IMU963RA)
	{
		v_angle = gzz;
		now_angle = yaw_imu;
	}
	
	// 直行逻辑
	if(zhuanwan_flag==0)					
	{
//		chuanji_cnt++;			
//		if(chuanji_cnt>5)													//	内环5次，外环1次		***				
//		{
//			chuanji_cnt=0;
			xun_buchang = 0;
		
				if(xun>=4&&xun<4.5)
				{
					xun = 2;
				}
				else if(xun<4&&xun>=3.5)
				{
					xun = 0.3;
				}
				else if(xun<3.5&&xun>=2)
					xun = -2;
				else if(xun>1&&xun<2)
					xun = -5;
				else if(xun<=1)
					xun = -6;
				
				else if(xun>4.5&&xun<=5)
				{
					xun =7;
				}
				else if(xun<=5.5&&xun>5)
				{
					xun = 8.3;
				}
				else if(xun>5.5&&xun<=7)
					xun = 12;
				else if(xun>7&&xun<8)
					xun = 15;
				else if(xun>=8)
					xun = 16;		
//		

			error_pwm_1 = Position_PID_4(xun+xun_buchang,4.5); 	// 外环的输出（灰度环）作为内环的输入
			error_pwm_1  = PwmXianfu(error_pwm_1 ,-xianfu,xianfu); 
			error_pwm=Incremental_PID_2(v_angle,error_pwm_1); //角速度内环pid根据目标角度，输出目标角速度对应的调整值
	//		printf("error_pwm：%4d\r\n",error_pwm);
			//用内环输出调整当前pwm
			// 速度环输出
//		if(xun<=8&&xun>=1)   // 能够寻上黑线就直接灰度串角速度输出  
//		{ 
			error_pwm  = PwmXianfu(error_pwm ,-xianfu+1000,xianfu-1000); 
			
			drive_right =PwmXianfu(drive_right,-xianfu,xianfu);
			drive_left  =PwmXianfu(drive_left ,-xianfu,xianfu); 
			
			new_drive_left =dead_zone -error_pwm + drive_left; //25为左轮不平衡
			new_drive_right=dead_zone + error_pwm +drive_right;
			
			new_drive_left=PwmXianfu(new_drive_left,-xianfu,xianfu);
			new_drive_right=PwmXianfu(new_drive_right,-xianfu,xianfu);
			
			set_motor_pwm(new_drive_left,new_drive_right);	// 加上死区
//		}
	}
	// 转弯逻辑	(根据当前角度是否在90度的cnt倍左右，做位式控制)
	else if(zhuanwan_flag==1)
	{
//		error_pwm_1 = -Position_PID(now_angle,zhuanwan_angle);

//		error_pwm=Incremental_PID_2(v_angle,error_pwm_1);


				if(now_angle-last_angle_zhuan>=-90 )						// 重要参数得反复调
				{
//					new_drive_left =-dead_zone -error_pwm; 
					new_drive_left=-dead_zone -750;
					new_drive_right=dead_zone + 800 ;
				}
//				else if(now_angle>(-90*zhuanjiao_cnt ))						// 重要参数得反复调
//				{
////					new_drive_left =-dead_zone -error_pwm; 
//					new_drive_left=-dead_zone -200;
//					new_drive_right=dead_zone + 1000 ;
//				}
//				if(now_angle<=(-90*zhuanjiao_cnt +20))
//				{
////					new_drive_left =dead_zone -error_pwm; 
//					new_drive_left=dead_zone+ 1000 +500;
//					new_drive_right=-dead_zone -500;		
//				}

				else if(now_angle-last_angle_zhuan<-90)
				{
//					new_drive_left =dead_zone -error_pwm; 
					new_drive_left=dead_zone+ 800;
					new_drive_right=-dead_zone -750;		
				}
				//进行限幅
				new_drive_left=PwmXianfu(new_drive_left,-xianfu,xianfu);
				new_drive_right=PwmXianfu(new_drive_right,-xianfu,xianfu);
				
				set_motor_pwm(new_drive_left,new_drive_right);
				/******重点：何时才能解除转弯，在上位机重新巡到灯的时候，角度是多少*************/
			
	}
}

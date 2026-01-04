#include "zf_common_headfile.h"

char LineString[50] = {0};
//uint8 page;

////新定义菜单
//uint8 renwu = 1;//1 2 3 4 5 6
//uint8 bianliang = 1;
//按键1发车 2选择任务 3选择界面 4参数切换（某一范围内循环）

void display_pro(void)
{	  sprintf(LineString,"yaw:%.2f   ",yaw);
		LCD_DisplayStringLine(0,2,LineString);
	
	      sprintf(LineString,"testing        ");
	  LCD_DisplayStringLine(0,10,LineString);
		    sprintf(LineString,"ANO_1:%d  ANO_3:%d       ",ANO_1,ANO_3);
		LCD_DisplayStringLine(0,11,LineString);
				sprintf(LineString,"ANO_2:%d  ANO_4:%d       ",ANO_2,ANO_4);
		LCD_DisplayStringLine(0,12,LineString);
//				      sprintf(LineString,"zhuanwan_flag:%d         ",zhuanwan_flag);
//						LCD_DisplayStringLine(0,3,LineString);
						sprintf(LineString,"target_quanshu:%d         ",target_quanshu);
						LCD_DisplayStringLine(0,4,LineString);				
						sprintf(LineString,"XUN:%.2f          ",xun);
						LCD_DisplayStringLine(0,5,LineString);
						sprintf(LineString,"position = %.5f",position);
						LCD_DisplayStringLine(0,6,LineString);
						sprintf(LineString,"gray = %d%d%d%d %d%d%d%d",track_arr[7],track_arr[6],track_arr[5],track_arr[4],track_arr[3],track_arr[2],track_arr[1],track_arr[0]);
						LCD_DisplayStringLine(0,7,LineString);	
						
						sprintf(LineString,"gzz:%.2f          ",gzz);
						LCD_DisplayStringLine(0,8,LineString);
						sprintf(LineString,"now_angle:%.2f          ",now_angle);
						LCD_DisplayStringLine(0,9,LineString);
//				sprintf(LineString,"ANO_3:%d         ",ANO_3);
//		LCD_DisplayStringLine(0,13,LineString);
//				sprintf(LineString,"ANO_4:%d         ",ANO_4);
//		LCD_DisplayStringLine(0,14,LineString);
	switch(renwu)
	{
		case 4:
		{
						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"bianliang:%d         ",bianliang);
						LCD_DisplayStringLine(0,1,LineString);			
		       
						sprintf(LineString,"ture_position:%.5f",ture_position);
						LCD_DisplayStringLine(0,3,LineString);
			
//			      sprintf(LineString,"zhuanwan_flag:%d         ",zhuanwan_flag);
//						LCD_DisplayStringLine(0,3,LineString);
						sprintf(LineString,"target_quanshu:%d         ",target_quanshu);
						LCD_DisplayStringLine(0,4,LineString);				
						sprintf(LineString,"XUN:%.2f          ",xun);
						LCD_DisplayStringLine(0,5,LineString);
						sprintf(LineString,"position = %.5f",position);
						LCD_DisplayStringLine(0,6,LineString);
						sprintf(LineString,"gray = %d%d%d%d %d%d%d%d",track_arr[7],track_arr[6],track_arr[5],track_arr[4],track_arr[3],track_arr[2],track_arr[1],track_arr[0]);
						LCD_DisplayStringLine(0,7,LineString);	
						
			sprintf(LineString,"gzz:%.0f  ",gzz);
						LCD_DisplayStringLine(0,8,LineString);
						sprintf(LineString,"now_angle:%.2f          ",now_angle);
						LCD_DisplayStringLine(0,9,LineString);
		
						sprintf(LineString,"xishu1:%.3f 2:%.3f         ",xishu1,xishu2);
        		LCD_DisplayStringLine(0,13,LineString);
				  	sprintf(LineString,"Speed_kp1:%.5f  ",Speed_Kp1);
        		LCD_DisplayStringLine(0,14,LineString);
			
			
			sprintf(LineString,"Speed_kp2:%.8f",Speed_Kp2);
						LCD_DisplayStringLine(0,15,LineString);			
			sprintf(LineString,"Angle_Kp:%.2f Roll:%.2f",Yaw_Angle_Pid.kp,Roll_Inc_Pid.kp);
						LCD_DisplayStringLine(0,16,LineString);	
						sprintf(LineString,"Kd:%.2f Turn:%d cnt:%d     ",Yaw_Angle_Pid.kd,Task4_Turn_flag,zhuanjiao_cnt	);
						LCD_DisplayStringLine(0,17,LineString);
			sprintf(LineString,"Ki:%.2f  Run:%d      ",Yaw_Angle_Pid.ki,Task4_Run_flag	);
						LCD_DisplayStringLine(0,18,LineString);		
						

		}
		break;
		case 2:
		{
				    sprintf(LineString,"Speed:%d ",ANO4_Err);
						LCD_DisplayStringLine(0,3,LineString);

						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"Dir 1-left 2-right:%d         ",Task3_Dir);
						LCD_DisplayStringLine(0,1,LineString);
			
						sprintf(LineString,"     Roll:%.2f      ",Roll_Inc_Pid.kp);
						LCD_DisplayStringLine(0,16,LineString);	

		}
		break;		
		case 3:
		{
	
						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"bianliang:%d         ",bianliang);
						LCD_DisplayStringLine(0,1,LineString);	

			

		}
		break;
		case 1:
		{

						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"LiCheng:%.2f         ",position);
						LCD_DisplayStringLine(0,1,LineString);	

						sprintf(LineString,"xishu1:%.3f 2:%.3f         ",xishu1,xishu2);
        		LCD_DisplayStringLine(0,13,LineString);
				  	sprintf(LineString,"Spppd_kp1:%.5f  ",Speed_Kp1);
        		LCD_DisplayStringLine(0,14,LineString);
			
			
			sprintf(LineString,"Speed_kp2:%.8f",Speed_Kp2);
						LCD_DisplayStringLine(0,15,LineString);		
			sprintf(LineString,"Angle_Kp:%.2f Roll:%.2f",Yaw_Angle_Pid.kp,Roll_Inc_Pid.kp);
						LCD_DisplayStringLine(0,16,LineString);	
			sprintf(LineString,"Kd:%.2f  Turn:%d      ",Yaw_Angle_Pid.kd,Task4_Turn_flag	);
						LCD_DisplayStringLine(0,17,LineString);
			sprintf(LineString,"Ki:%.2f  Run:%d      ",Yaw_Angle_Pid.ki,Task4_Run_flag	);
						LCD_DisplayStringLine(0,18,LineString);		
			
//Task4_Flag			
//zhuanjiao_cnt
			
	

		}
		break;
		case 5:
		{
						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"bianliang:%d         ",bianliang);
						LCD_DisplayStringLine(0,1,LineString);		

						sprintf(LineString,"ture_position:%.5f",ture_position);
						LCD_DisplayStringLine(0,3,LineString);
//			      sprintf(LineString,"zhuanwan_flag:%d         ",zhuanwan_flag);
//						LCD_DisplayStringLine(0,3,LineString);
						sprintf(LineString,"target_quanshu:%d         ",target_quanshu);
						LCD_DisplayStringLine(0,4,LineString);				
						sprintf(LineString,"XUN:%.2f          ",xun);
						LCD_DisplayStringLine(0,5,LineString);
						sprintf(LineString,"position = %.5f",position);
						LCD_DisplayStringLine(0,6,LineString);
						sprintf(LineString,"gray = %d%d%d%d %d%d%d%d",track_arr[7],track_arr[6],track_arr[5],track_arr[4],track_arr[3],track_arr[2],track_arr[1],track_arr[0]);
						LCD_DisplayStringLine(0,7,LineString);	
						
			sprintf(LineString,"gzz:%.1f ",gzz);
						LCD_DisplayStringLine(0,8,LineString);
						sprintf(LineString,"now_angle:%.2f          ",now_angle);
						LCD_DisplayStringLine(0,9,LineString);
		
						sprintf(LineString,"xishu1:%.3f 2:%.3f         ",xishu1,xishu2);
        		LCD_DisplayStringLine(0,13,LineString);
				  	sprintf(LineString,"Speed_kp1:%.5f  ",Speed_Kp1);
        		LCD_DisplayStringLine(0,14,LineString);
			
			
			sprintf(LineString,"Speed_kp2:%.8f",Speed_Kp2);
						LCD_DisplayStringLine(0,15,LineString);				
			sprintf(LineString,"Angle_Kp:%.2f Roll:%.2f",Yaw_Angle_Pid.kp,Roll_Inc_Pid.kp);
						LCD_DisplayStringLine(0,16,LineString);	
						sprintf(LineString,"Kd:%.2f Turn:%d cnt:%d     ",Yaw_Angle_Pid.kd,Task4_Turn_flag,zhuanjiao_cnt	);
						LCD_DisplayStringLine(0,17,LineString);
			sprintf(LineString,"Ki:%.2f  Run:%d      ",Yaw_Angle_Pid.ki,Task4_Run_flag	);
						LCD_DisplayStringLine(0,18,LineString);					

		}
		break;
		case 6:
		{

						sprintf(LineString,"renwu:%d         ",renwu);
						LCD_DisplayStringLine(0,0,LineString);
						sprintf(LineString,"bianliang:%d         ",bianliang);
						LCD_DisplayStringLine(0,1,LineString);						
		}
		break;		
	}
}
//void display(void)
//{
//	
////      sprintf(LineString,"ANO_rx_1:%d         ",ANO_1);
////			LCD_DisplayStringLine(0,0,LineString);
////		  sprintf(LineString,"ANO_rx_2:%d         ",ANO_2);
////			LCD_DisplayStringLine(0,1,LineString);
////		  sprintf(LineString,"ANO_rx_3:%d         ",ANO_3);
////			LCD_DisplayStringLine(0,2,LineString);
//		  sprintf(LineString,"ANO_rx_4:%.5f         ",yaw_imu);
//			LCD_DisplayStringLine(0,3,LineString);
////	
//////	    sprintf(LineString,"imu_kp%.4f  ",(float)imu_kp);
//////			LCD_DisplayStringLine(label_id[4],LineString, RGB565_GREEN);
//////	    sprintf(LineString,"imu_ki%.4f           ",(float)imu_ki);
//////			LCD_DisplayStringLine(label_id[5],LineString, RGB565_GREEN);
////	
//////	    sprintf(LineString,"Sec %d           ",Sec);
//////			LCD_DisplayStringLine(label_id[6],LineString, RGB565_GREEN);
////			
////			sprintf(LineString,"ANO_tx_1:%d            ",ANO_tx_1);
////			LCD_DisplayStringLine(0,3,LineString);
////		  sprintf(LineString,"ANO_tx_2:%d            ",ANO_tx_2);
////			LCD_DisplayStringLine(0,4,LineString);
////		  sprintf(LineString,"ANO_tx_3:%d            ",ANO_tx_3);
////			LCD_DisplayStringLine(0,5,LineString);
//			
//			sprintf(LineString,"Angle1:%d            ",Angle1);
//			LCD_DisplayStringLine(0,6,LineString);
//		  sprintf(LineString,"Angle2:%d            ",Angle2);
//			LCD_DisplayStringLine(0,7,LineString);
//		  sprintf(LineString,"Dir1:%d              ",Dir1);
//			LCD_DisplayStringLine(0,8,LineString);
//			sprintf(LineString,"Dir2:%d              ",Dir2);
//			LCD_DisplayStringLine(0,9,LineString);
//			if(Roll_Inc_Pid.err>1000){
//			sprintf(LineString,"err:err              ");
//			}else{
//			sprintf(LineString,"err:%.5f             ",Roll_Inc_Pid.err);
//			}
//			LCD_DisplayStringLine(0,10,LineString);
//			sprintf(LineString,"Roll.Kp:%.5f          ",Roll_Inc_Pid.kp);
//			LCD_DisplayStringLine(0,11,LineString);
//			sprintf(LineString,"OUT:%.5f              " ,Roll_Inc_Pid.pwm_out);
//			LCD_DisplayStringLine(0,12,LineString);
//			sprintf(LineString,"Angle2_temp:%.5f      ",Angle2_temp);
//			LCD_DisplayStringLine(0,13,LineString);
//			sprintf(LineString,"Step:%d              ",RollX_step);
//			LCD_DisplayStringLine(0,14,LineString);
//			sprintf(LineString,"time:%d              ",Time);
//			LCD_DisplayStringLine(0,15,LineString);
//			//Angle2_temp
//			
//			
//			//Roll_Inc_Pid.pwm_out
//			
////			sprintf(LineString,"roll%.3f            ",(float)roll_wit);
////			LCD_DisplayStringLine(label_id[7],LineString, RGB565_GREEN);
////		  sprintf(LineString,"pitch%.3f            ",(float)pitch_wit);
////			LCD_DisplayStringLine(label_id[8],LineString, RGB565_GREEN);
////		  sprintf(LineString,"yaw%.3f              ",(float)yaw_wit);
////			LCD_DisplayStringLine(label_id[9],LineString, RGB565_GREEN);
//}



void LCD_DisplayStringLine(uint16 x ,uint16 y,const char dat[])
{
	ips200_show_string( x , 16*y, dat);
}	

void LCD_Init(void)
{
	  ips200_set_dir(IPS200_PORTAIT_180);						 //在这里改是横着用还是竖着用
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);//在这里修改颜色 1是字体 2是背景
    ips200_init(IPS200_TYPE);
	
//下面是方便修改的定义//
//typedef enum
//{
//    RGB565_WHITE    = (0xFFFF),           // 白色           					typedef enum
//    RGB565_BLACK    = (0x0000),           // 黑色                      { 																									
//    RGB565_BLUE     = (0x001F),           // 蓝色                          IPS200_PORTAIT                      = 0,          // 竖屏模式  
//    RGB565_PURPLE   = (0xF81F),           // 紫色                          IPS200_PORTAIT_180                  = 1,          // 竖屏模式  旋转180 
//    RGB565_PINK     = (0xFE19),           // 粉色                          IPS200_CROSSWISE                    = 2,          // 横屏模式 
//    RGB565_RED      = (0xF800),           // 红色                          IPS200_CROSSWISE_180                = 3,          // 横屏模式  旋转180 
//    RGB565_MAGENTA  = (0xF81F),           // 品红                      }ips200_dir_enum; 
//    RGB565_GREEN    = (0x07E0),           // 绿色
//    RGB565_CYAN     = (0x07FF),           // 青色
//    RGB565_YELLOW   = (0xFFE0),           // 黄色
//    RGB565_BROWN    = (0xBC40),           // 棕色
//    RGB565_GRAY     = (0x8430),           // 灰色

//    RGB565_39C5BB   = (0x3616),
//    RGB565_66CCFF   = (0x665F),
//}rgb565_color_enum;
}
#include "zf_common_headfile.h"
#include "track_black.h"

unsigned char lukou_num = 0; //全局变量定义检测到路口的次数
float xun;									// 与4.5作比较
float last_xun;
uint8 track_arr[8] = {0};

void track_init(void)
{
	gpio_init(D1, GPI, 1, GPI_PULL_UP);
	gpio_init(D2, GPI, 1, GPI_PULL_UP);
	gpio_init(D3, GPI, 1, GPI_PULL_UP);
	gpio_init(D4, GPI, 1, GPI_PULL_UP);
	gpio_init(D5, GPI, 1, GPI_PULL_UP);
	gpio_init(D6, GPI, 1, GPI_PULL_UP);
	gpio_init(D7, GPI, 1, GPI_PULL_UP);
	gpio_init(D8, GPI, 1, GPI_PULL_UP);
}

// 黑线-高电平-灯灭
void read_track_pin(void)
{
    if(gpio_get_level(D1) == 1) track_arr[0] = 1;
    else track_arr[0] = 0;  

    if(gpio_get_level(D2) == 1) track_arr[1] = 1;
    else track_arr[1] = 0;  

    if(gpio_get_level(D3) == 1) track_arr[2] = 1;
    else track_arr[2] = 0;  

    if(gpio_get_level(D4) == 1) track_arr[3] = 1;
    else track_arr[3] = 0;  

    if(gpio_get_level(D5) == 1) track_arr[4] = 1;
    else track_arr[4] = 0;  

    if(gpio_get_level(D6) == 1) track_arr[5] = 1;
    else track_arr[5] = 0;  

    if(gpio_get_level(D7) == 1) track_arr[6] = 1;
    else track_arr[6] = 0;  

    if(gpio_get_level(D8) == 1) track_arr[7] = 1;
    else track_arr[7] = 0;  
}

void grayscale_get_2(uint8_t *gray_sensor)			// 获取黑线位置			黑的是0
{
	uint8 black_cnt = 0;												// 检测到黑线的数量
	uint8 black_sum = 0;												// 统计黑色的位置	
	
	for(uint8 i=0;i<8;i++)
	{
		if(gray_sensor[i] == 1)										// 检测是黑灯
		{
			black_cnt++;
			black_sum+= (8-i);				// 正着装
//			black_sum+= (i+1);			// 反着装
		}	
	}
	if(black_sum == 0)													// 全是白的
	{
		black_cnt = 1;
	}	
		xun = 1.0*black_sum/black_cnt;
	if(xun != 0)																// 防跑飞
	{
		last_xun = xun;
	}	
//	else if(xun == 0)
//	{
//		xun = last_xun;														// 记录上一次黑线位置
//	}	
}


/*************************************
*函数名称：track_zhixian1()
*函数功能：直线循迹
*参数：
*说明：数字口获取数字量
*			白底黑线，线宽2厘米，其他线宽根据实际情况改写，提供的程序只供参考。
**************************************/
//void track_zhixian()
//{    
//	unsigned char num = 0,i;  //num个灯压线认为是到达路口
//	
//	for(i=0;i<2;i++) //循环检测路口2次
//	{
//		if(track_arr[0] == 1)  num++;	if(track_arr[1] == 1)  num++;	if(track_arr[2] == 1)  num++;	if(track_arr[3] == 1)  num++;
//		if(track_arr[4] == 1)  num++;	if(track_arr[5] == 1)  num++;	if(track_arr[6] == 1)  num++;	if(track_arr[7] == 1)  num++;
//		
//		if(num >= (ADC_N-4)) //大于等于ADC_N-4个灯压线认为是到达路口
//		{ 
//			lukou_num++; 
//			if(lukou_num == 1)	 
//            system_delay_ms(30); //第一次检测到延时10ms，消抖操作
//		}  
//		num = 0;
//	}
//    //选择性加这个，如果转向需要停车加这个
//	//if(lukou_num >= 2) { lukou_num = 0; motor(0,0); Delay_ms(1000);Delay_ms(1000);}  //检测2次都是路口后才认为是真正到达路口，防止误判

//    //这里的逻辑是1路在小车的左边，ADC_N路在小车的右边
//    //8765 4321（路）																																																																																																																																															  //8765 4321（路）																											
//    if((track_arr[0] == BLACK_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))        motor(0,50);      //0000 0001
//    else if((track_arr[0] == BLACK_EXIST)&&(track_arr[1] == BLACK_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))   motor(10,50);     		//0000 0011
//    else if((track_arr[0] == BLACK_EXIST)&&(track_arr[1] == BLACK_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))   motor(15,40);     		//0000 0010
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_EXIST)&&(track_arr[2] == BLACK_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))   motor(20,40);     		//0000 0110
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))   motor(25,40);     //0000 0100
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == BLACK_NOT_EXIST)&&(track_arr[6] == BLACK_NOT_EXIST)&&(track_arr[7] == BLACK_NOT_EXIST))   motor(35,40);     //0000 1100
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == 1)&&(track_arr[4] == 0)&&(track_arr[5] == 0)&&(track_arr[6] == 0)&&(track_arr[7] == 0))   motor(35,40);     //0000 1000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_EXIST)&&(track_arr[4] == BLACK_EXIST)&&(track_arr[5] == 0)&&(track_arr[6] == 0)&&(track_arr[7] == 0))   motor(40,40);     //0001 1000 //正中间位置
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_EXIST)&&(track_arr[5] == 0)&&(track_arr[6] == 0)&&(track_arr[7] == 0))   motor(40,35);     //0001 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_EXIST)&&(track_arr[5] == 1)&&(track_arr[6] == 0)&&(track_arr[7] == 0))   motor(40,35);     //0011 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == 1)&&(track_arr[6] == 0)&&(track_arr[7] == 0))   motor(40,25);     //0010 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == 1)&&(track_arr[6] == 1)&&(track_arr[7] == 0))   motor(40,20);     //0110 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == 0)&&(track_arr[6] == 1)&&(track_arr[7] == 0))   motor(40,15);     //0100 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == 0)&&(track_arr[6] == 1)&&(track_arr[7] == 1))   motor(50,10);     //1100 0000
//    else if((track_arr[0] == BLACK_NOT_EXIST)&&(track_arr[1] == BLACK_NOT_EXIST)&&(track_arr[2] == BLACK_NOT_EXIST)&&(track_arr[3] == BLACK_NOT_EXIST)&&(track_arr[4] == BLACK_NOT_EXIST)&&(track_arr[5] == 0)&&(track_arr[6] == 0)&&(track_arr[7] == 1))   motor(50,0);      //1000 0000
//    else   motor(20,20);
//}
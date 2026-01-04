#include "zf_common_headfile.h"

uint16_t Angle2=0;//偏航角
uint16_t Angle1=0;//俯仰角

float Angle2_temp=0;//偏航角
uint16_t Angle1_temp=0;//俯仰角


uint8_t Dir1=1;//俯仰角，转动方向，顺时针

uint8_t Dir2=1;//偏航角，转动方向，顺时针

uint8_t Drive_Flag1=0;//俯仰角转动标志位
uint8_t Drive_Flag2=0;//偏航角转动标志位
uint8_t Serial_RxPacket[4];

extern uint8 get_ano ;// 接收数据变量
extern fifo_struct ANO_data_fifo;

/**
  * @brief  电机驱动

  * @param  Address 电机地址位 1俯仰角 2偏航                      范围 1-2
  * @param  Clockwise 电机转动方向 1顺时针 2逆时针                范围 1-2
  * @param  Angle 电机转动角度 6400脉冲对应360°                  范围 0-65535
  * @param  Speed 电机转动最大速度 单位：每分钟几圈（RPM）        范围 0-65535
  * @param  Acceleration 电机加速度档位 默认给60                  范围 0-255
  * @param  uart_index 串口号 硬件层  默认UART_2                  宏定义

  * @retval None
  */

void Drive_Move(uint8_t Address,uint8_t Clockwise,uint16_t Angle,uint16_t Speed,uint8_t Acceleration,uart_index_enum uart_index)
{
	//初始定义
	uint8_t CommandStep1[]={0x01,0xFD,0x01,0x00,0x00,0x60,0x00,0x00,0x0C,0x80,0x01,0x00,0x6B};
		
	//电机地址位 1俯仰角 2偏航角
	switch(Address)
	{
		case 1:CommandStep1[0]=0x01;break;
		case 2:CommandStep1[0]=0x02;break;
	}
	
	//电机转动方向 1顺时针 2逆时针
	switch(Clockwise)
	{
		case 1:CommandStep1[2]=0x00;break;
		case 2:CommandStep1[2]=0x01;break;
	}
	
	//电机转动角度 6400脉冲对应360°
	CommandStep1[8]=0x00;
	CommandStep1[9]=0x00;
	uint8_t Angle_HighByte = (Angle >> 8) & 0xFF;                  //高字节
	uint8_t Angle_LowByte  = Angle & 0xFF;                         //低字节
	CommandStep1[8]=Angle_HighByte;
	CommandStep1[9]=Angle_LowByte;
	
	//电机转动最大速度 单位：每分钟几圈（RPM）
	CommandStep1[3]=0x00;
	CommandStep1[4]=0x00;
	uint8_t Speed_HighByte = (Speed >> 8) & 0xFF;                  //高字节
	uint8_t Speed_LowByte  = Speed & 0xFF;                         //低字节
	CommandStep1[3]=Speed_HighByte;
	CommandStep1[4]=Speed_LowByte;
	
	//电机加速度档位 默认给60
	CommandStep1[5]=0x00;
	uint8_t Acceleration_LowByte  = Acceleration & 0xFF;           //低字节
	CommandStep1[5]=Acceleration_LowByte;
	
	//串口数据发送
	uart_write_buffer(uart_index,CommandStep1,13);
	system_delay_ms (50);
}

void Drive_Move_speed(uint8_t Address,uint8_t Clockwise,uint16_t Angle,uint16_t Speed,uint8_t Acceleration,uart_index_enum uart_index)
{
	//初始定义
	uint8_t CommandStep1[]={0x01,0xFD,0x01,0x00,0x00,0x60,0x00,0x00,0x0C,0x80,0x00,0x00,0x6B};
		
	//电机地址位 1俯仰角 2偏航角
	switch(Address)
	{
		case 1:CommandStep1[0]=0x01;break;
		case 2:CommandStep1[0]=0x02;break;
	}
	
	//电机转动方向 1顺时针 2逆时针
	switch(Clockwise)
	{
		case 1:CommandStep1[2]=0x00;break;
		case 2:CommandStep1[2]=0x01;break;
	}
	
	//电机转动角度 6400脉冲对应360°
	CommandStep1[8]=0x00;
	CommandStep1[9]=0x00;
	uint8_t Angle_HighByte = (Angle >> 8) & 0xFF;                  //高字节
	uint8_t Angle_LowByte  = Angle & 0xFF;                         //低字节
	CommandStep1[8]=Angle_HighByte;
	CommandStep1[9]=Angle_LowByte;
	
	//电机转动最大速度 单位：每分钟几圈（RPM）
	CommandStep1[3]=0x00;
	CommandStep1[4]=0x00;
	uint8_t Speed_HighByte = (Speed >> 8) & 0xFF;                  //高字节
	uint8_t Speed_LowByte  = Speed & 0xFF;                         //低字节
	CommandStep1[3]=Speed_HighByte;
	CommandStep1[4]=Speed_LowByte;
	
	//电机加速度档位 默认给60
	CommandStep1[5]=0x00;
	uint8_t Acceleration_LowByte  = Acceleration & 0xFF;           //低字节
	CommandStep1[5]=Acceleration_LowByte;
	
	//串口数据发送
	uart_write_buffer(uart_index,CommandStep1,13);
}


void Dirve_IRQ(void){
//	 static uint8_t state = 0;           // 状态机状态
//   static uint8_t data_index = 0;      // 当前数据存储位置
   //uint8_t get_ano = USART_ReceiveData(USART3);
	uart_query_byte(UART_2, &get_ano);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
	fifo_write_buffer(&ANO_data_fifo, &get_ano, 1);                           // 将数据写入 fifo 中
//	uart_write_buffer(UART_0,&get_ano,1);
	
//	switch(state) 
//        {
//            // 状态0：等待起始字节
//            case 0:
//                if (get_ano >= 0x01 && get_ano <= 0x07) 
//                {
//                    Serial_RxPacket[0] = get_ano; // 存储起始字节
//                    data_index = 1;      // 下一个数据存储位置
//                    state = 1;
//                }
//                break;

//            // 状态1：接收数据部分
//            case 1:
//                if (data_index < sizeof(Serial_RxPacket)) 
//                {
//                    Serial_RxPacket[data_index++] = get_ano;
//                    
//                    // 接收满3个数据字节后进入状态2
//                    if (data_index >= 4)  // 起始字节+3数据字节
//                    {
//                        state = 2;
//                    }
//                } 
//                else 
//                {
//                    // 缓冲区溢出，强制重置
//                    state = 0;
//                    data_index = 0;
//                }
//                break;

//            // 状态2：验证结束符
//            case 2:
////                Serial_RxFlag = (get_ano == 0x6B); // 设置接收完成标志
//						    uart_write_buffer(UART_0,(uint8_t *)Serial_RxPacket,4);
//                state = 0;      // 无论是否成功都重置状态机
//                data_index = 0;
//                break;
//        }

	
//	fifo_write_buffer(&ANO_data_fifo, &get_ano, 1);                           // 将数据写入 fifo 中
  
//	uart_write_buffer(UART_0,&get_ano,1);
}



#include "zf_common_headfile.h"

extern uint8_t Time1;
uint8_t Flag=0;

void Get_ANO(ano_frameStruct rec_frame_struct){
//if(!Time1)
//{
ano_scanf("iiii",
	&ANO_1,
	&ANO_2,
	&ANO_3,
	&ANO_4
	);
	
	Flag=1;//置标志位，表示接收到一帧数据
//	gpio_set_level(A14, 1);//置高电平
//}
}

uint8 get_data = 0;
uint8 get_ano = 0;// 接收数据变量

fifo_struct uart_data_fifo;
fifo_struct ANO_data_fifo;

uint8 uart_get_ANO[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_ANO[64];                                                        // fifo 输出读出缓冲区


uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART_INDEX 的接收中断处理函数 这个函数将在 UART_INDEX 对应的中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     uart_rx_interrupt_handler();
//-------------------------------------------------------------------------------------------------------------------
void uart2_rx_interrupt_handler (uint32 state, void *ptr)
{ 
//    get_data = uart_read_byte(UART_INDEX);                                    // 接收数据 while 等待式 不建议在中断使用
//    uart_query_byte(UART_INDEX, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
//    fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 将数据写入 fifo 中
//	ANO();
	Dirve_IRQ();
}

void uart0_rx_interrupt_handler (uint32 state, void *ptr)
{ 
//    get_data = uart_read_byte(UART_INDEX);                                    // 接收数据 while 等待式 不建议在中断使用
//    uart_query_byte(UART_INDEX, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
//    fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 将数据写入 fifo 中
	vofa();
}


void Drive_Init(void)
{
//uart_init(UART_2, UART_BAUDRATE, UART2_TX_B15, UART2_RX_B16);  //只需要初始化串口

fifo_init(&ANO_data_fifo, FIFO_DATA_8BIT, uart_get_ANO, 64);              // 初始化 fifo 挂载缓冲区	
	
uart_init(UART_2, 921600, UART2_TX_B15, UART2_RX_B16);             // 初始化串口
	
uart_set_interrupt_config(UART_2, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
	
interrupt_set_priority(UART2_INT_IRQn, 1);                                   // 设置对应 UART_INDEX 的中断优先级为 0
	
uart_set_callback(UART_2, uart2_rx_interrupt_handler, NULL);			    // 定义中断接收函数
}



void vofa_debug_init(void){
	
	
fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区	
	
uart_init(UART_INDEX, 921600, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
	
uart_set_interrupt_config(UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
	
interrupt_set_priority(UART_PRIORITY, 1);                                   // 设置对应 UART_INDEX 的中断优先级为 0
	
uart_set_callback(UART_INDEX, uart0_rx_interrupt_handler, NULL);			    // 定义中断接收函数

}

//void imu_init(void)
//{
//    // 初始化陀螺仪
//    while(1)
//    {
//        if(imu963ra_init())
//        {
//           printf("\r\n imu963ra init error.");                                 // imu963ra 初始化失败
//        }
//        else
//        {
//					 printf("\r\n imu963ra init success.");                                 // imu963ra 初始化成功
//           break;
//        }
//    }
//}

void init(void)
{
	//vofa初始化	--串口中断
	vofa_debug_init();
	system_delay_ms(50);				
	//通信帧初始化
	ano_frame_init();	
	
//	wit_Init();
	
#ifndef UART1_DMA
	
//	ANO_It_init();
	
#else	
	
	ANO_DMA_init();
	
#endif
	gpio_init(B4, GPO, 0, GPO_PUSH_PULL);		// 激光笔
	
	Drive_PID_Init();
	
	motor_init();
	encoder_init();
	track_init();	
	Drive_Init();  //步进电机
	Key_Init(0);
//屏幕初始化
//	display_init();
	LCD_Init();	
	
	while(imu963ra_init());				// 特别注意：一定要接陀螺仪	不接就注释掉！！！！！！
	while(imu963ra_init_iic());
system_delay_ms(50);	
////陀螺仪初始化--iic

gyroOffsetInit();//角速度零漂值获取	
//IIR_imu();//IIR滤波因子获取	

	//定时器初始化
	imu_flag = 1;
	while(imu_flag)
	{
		imu_flag_();
		//校准时 啥也不干  等待校准
		//校准结束  
		if(imu_flag==0)
		{
			break;
		}
	}
//	servo_init();  //舵机
	
	
	
	pit_1ms_init();		
	
	
	
}
#ifndef _COREDRIVE_H
#define _COREDRIVE_H
	
#include "zf_common_headfile.h"


//这里是初始化所需的一些定义
//默认使用逐飞库上面现成的串口位置 那四个地方对应串口2
//#define UART_INDEX                 ( UART_2 )                              // 默认  
//#define UART_BAUDRATE              ( 115200 )                                // 默认 9600
//#define UART_TX_PIN                ( UART2_TX_B15 )                        // 默认 UART2_TX_B15
//#define UART_RX_PIN                ( UART2_RX_B16 )                        // 默认 UART2_RX_B16


//驱动之前要初始化串口 串口的初始化都有封装好的函数 输入对应的参数就行 一般在zf_common_debug.h的开头有define定义




void Drive_Init(void);

//***********************************Drive_Move函数*************************************

void Drive_Move(uint8_t Address,uint8_t Clockwise,uint16_t Angle,uint16_t Speed,uint8_t Acceleration,uart_index_enum uart_index);

void Drive_Move_speed(uint8_t Address,uint8_t Clockwise,uint16_t Angle,uint16_t Speed,uint8_t Acceleration,uart_index_enum uart_index);
void Dirve_IRQ(void);

void Drive_Get(void);
#endif
#ifndef _INIT_H_
#define _INIT_H_
#include "zf_common_headfile.h"
/*****************UART_VOFA*******************/

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默认 UART_1
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默认 UART0_TX_A10
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // 默认 UART1_RX_A11

#define UART_PRIORITY           (UART0_INT_IRQn)                                // 对应串口中断的中断编号 在 MIMXRT1064.h 头文件中查看 IRQn_Type 枚举体

void vofa_debug_init(void);

void imu_init(void);

void display_init(void);

void init(void);

void Drive_It_init(void);
#endif


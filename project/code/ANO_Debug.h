#ifndef ANO_DEBUG_H_
#define ANO_DEBUG_H_
#include "zf_common_headfile.h"
#include "stdint.h"
//目标地址宏定义
#define FRAME_HEADER          0XAA   //<匿名协议固定帧头
#define GENERAL_OUTPUT_ADDR   0XFF   //<广播型输出
#define HOST_ADDR             0XAF   //<向上位机输出
#define PRO_ADDR              0X05   //<拓空者PRO飞控
#define SHUCHUAN_ADDR         0X10   //<匿名数传
#define GUANGLIU_ADDR         0X22   //<匿名光流
#define UWB_ADDR              0X30   //<匿名UWB
#define IMU_ADDR              0X60   //<匿名凌霄IMU
#define LINGXIAO_ADDR         0X61   //<匿名凌霄飞控

#define ANO_ID                0xF1  //功能码
//32位数据进行四个字节剥离拆分，从低位到高位
#define BYTE0(temp)    (*(char*)(&temp))
#define BYTE1(temp)    (*((char*)(&temp)+1))
#define BYTE2(temp)    (*((char*)(&temp)+2))
#define BYTE3(temp)    (*((char*)(&temp)+3))
	
#ifndef UART1_DMA //使用UART1 DMA 通道1

//#define DMA_CH0_CHAN_ID                                                      (1)

#else             //使用UART1 DMA 通道1
#define DMA_CH1_CHAN_ID                                                      (1)
#endif

/****参数对象结构体****/
typedef struct
{
    uint16_t par_id;                 //<参数id
    int32_t  par_val;                //<参数值
}par_struct;
/****通信帧对象结构体****/
#define MAX_LENGTH 40
#define ANOSIZE 46

typedef struct
{
    uint8_t head;                    //<帧头
    uint8_t target_addr;             //<目标地址
    uint8_t function_id;             //<该帧要实现某功能的功能码id
    uint8_t data_len;                //<数据长度
    uint8_t data[MAX_LENGTH];        //<数据内容，协议最高只支持*字节数据
    uint8_t sum_check;               //<和校验
    uint8_t add_check;               //<附加校验

    par_struct* parameter;           //<参数
}ano_frameStruct;



//暴露ano接收变量
extern ano_frameStruct   rec_frame_struct;
extern uint8_t ReadBuffAno[ANOSIZE];
//定义ano回调函数
typedef void (*ano_recv_callback_proc)(ano_frameStruct rec_ano_frame);


/*共同点：形参都含ano_frameStruct*类型指针*/

void ano_frame_init(void);
void ano_frame_reset(ano_frameStruct* frame);
void ano_check_calculate(ano_frameStruct* frame);
uint8_t ano_check(ano_frameStruct* frame);
void ano_send_string(uint8_t color,char* str);
void frame_turn_to_array(ano_frameStruct* frame,uint8_t*str);
void ano_send_flexible_frame(uint8_t id,int32_t data1,int32_t data2,int32_t data3,int32_t data4,int32_t data5,int32_t data6,int32_t data7,int32_t data8,int32_t data9,int32_t data10);
void ANO_polling(void);
void ano_receive(uint8_t byte);
void push_frame();
void ano_recv_process();
void ano_set_callback(ano_recv_callback_proc NewCallback);
int ano_printf(const char* format_str,...);
int ano_scanf(const char* format_str,...);
void ano_set_address(uint8_t d_addr);
void ano_set_type(uint8_t id);
void ANO_DMA_init(void);
void ANO_Getdata(void);

void ANO(void);
void ANO_debug(void);
void Get_ANO(ano_frameStruct rec_frame_struct);
#endif /* SRC_APPSW_TRICORE_USER_ANO_DT_H_ */

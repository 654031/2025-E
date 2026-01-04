/*
*使用方法：
*1.在初始化时调用通信帧结构体初始函数“ano_frame_init”
*2.把需要传递的参数，写进ano_send_flexible_frame中（），总共11个参数，第一个参数是“ANO_ID”（宏定义在.h文件中）剩下的10个参数是自己的数据
*/


#include "zf_common_headfile.h"
#include <stdarg.h>
uint8_t ReadBuffAno[ANOSIZE];

uint32_t ANO_1;
uint32_t ANO_2;
uint32_t ANO_3;
uint32_t ANO_4;
uint32_t ANO_5;
uint32_t ANO_6;
uint32_t ANO_7;
uint32_t ANO_8;
uint32_t ANO_9;
uint32_t ANO_10;
uint8_t ANO_Flag;

uint32 fifo_ano_count = 0;                                                     // fifo 数据个数

extern uint8 get_ano ;// 接收数据变量
extern fifo_struct ANO_data_fifo;
extern uint8 uart_get_ANO[64];                                                        // 串口接收数据缓冲区
extern uint8 fifo_get_ANO[64];                                                        // fifo 输出读出缓冲区


static par_struct      send_parameter;       //<发送帧中的参数;
static par_struct      rec_parameter;        //<接收帧的参数;
static ano_frameStruct send_frame_struct;    //<(发送)通信帧结构体
ano_frameStruct   rec_frame_struct;     //<(接收)通信帧结构体,因不止在本.c文件使用,故不用static修饰

static uint8_t RecvIndex = 0;//接收缓存索引,供ano_receive使用
static uint8_t DataLength = 0;//数据长度，临时保存，仅当读取每个包中间过程该值有效,供ano_receive使用

ano_recv_callback_proc AnoCallVar = NULL;//回调函数保存变量


/**
  * @brief   初始化通信帧结构体,使用前必须调用
  * @param   无输入参数
  * @retval  无返回
  **/
void ano_frame_init(void)
{
    /*参数结构体初始化*/
    send_frame_struct.parameter=&send_parameter;
    rec_frame_struct.parameter=&rec_parameter;
    send_frame_struct.parameter->par_id=0;
    send_frame_struct.parameter->par_val=0;
    rec_frame_struct.parameter->par_id=0;
    rec_frame_struct.parameter->par_val=0;

    send_frame_struct.head=rec_frame_struct.head=FRAME_HEADER;//帧头固定是0XAA
    send_frame_struct.target_addr=rec_frame_struct.target_addr=GENERAL_OUTPUT_ADDR;//地址->上位机输出 × 广播型输出 √
    send_frame_struct.function_id=0XFF;//<协议中没有定义的功能ID,这样初始化目的是为了启动瞬间不做任何动作


    memset(send_frame_struct.data,0,40);//<缓存默认全部置0
    memset(rec_frame_struct.data,0,40);
	
		RecvIndex = 0;
		DataLength = 0;
}

/**
  * @brief   复位通信帧结构体，ano_frame_init()必须要运行过一次
  * @param   通信帧结构体对象
  * @retval  无返回
  **/
void ano_frame_reset(ano_frameStruct* frame)
{
  frame->function_id=0XFF;
  frame->data_len=0;
  memset(frame->data,0,MAX_LENGTH);
  frame->add_check=0;
  frame->sum_check=0;
}

/**
  * @brief   通信帧中参数结构体内成员的配置
  * @param   通信帧结构体对象，参数ID与参数值
  * @retval
  **/
void ano_par_struct_config(ano_frameStruct* frame,uint16_t id,int32_t val)
{
    frame->parameter->par_id=id;
    frame->parameter->par_val=val;
}

/**
  * @brief   通信帧校验计算
  * @param   通信帧结构体对象
  * @retval  无返回值
  **/
void ano_check_calculate(ano_frameStruct* frame)
{
    frame->sum_check=0;
    frame->add_check=0;

    //除去和校验，附加校验及数据部分，有4个部分4个字节，长度固定。结构体的首地址即head的地址，0，1，2，3依次是帧头、目标地址、ID、长度
    for(uint32_t i=0;i<4;i++)
    {
      frame->sum_check+= *(uint8_t*)(&frame->head+i);//转数组
      frame->add_check+=frame->sum_check;
    }
    //获取数据长度部位,把数据部分全加上
    for(uint32_t i=0;i<frame->data_len;i++)
    {
      frame->sum_check+=*((uint8_t*)(frame->data)+i);
      frame->add_check+=frame->sum_check;
    }
 }

 /**
  * @brief   通信帧校验检查(接收上位机通信帧时用)
  * @param   通信帧结构体对象
  * @retval   1：校验成功 0:校验失败
  **/
uint8_t ano_check(ano_frameStruct* frame)
{
    uint8_t sum_check=0;
    uint8_t add_check=0;

    for(uint32_t i=0;i<4;i++)
    {
      sum_check+= *(uint8_t*)(&frame->head+i);
      add_check+=sum_check;
    }
    for(uint32_t i=0;i<frame->data_len;i++)
    {
      sum_check+=*((uint8_t*)(frame->data)+i);
      add_check+=sum_check;
    }
    //如果计算与获取的相等，校验成功
    if((sum_check==frame->sum_check)&&(add_check==frame->add_check))
     return 1;
    else
     return 0;
}

/**
  * @brief  匿名串口发送
  * @param  字符串，数据字节个数
  * @retval
  */
static void ano_usart_send(uint8_t*str,uint16_t num)
{
 uint16_t cnt=0;
 do
    {
			uart_write_buffer (UART_INDEX, ((uint8_t*)(str))+cnt, 1);   //---M0
//      HAL_UART_Transmit(&huart1,((uint8_t*)(str))+cnt,1,1000);  //---STM32
//      UART_PutChar(UART0, ((uint8_t*)(str))+cnt);               //---英飞凌
        cnt++;
    }while(cnt<num);
}


/**
  * @brief   通信帧结构体转化为线性数组
  * @param   要转换的通信帧，缓存数组
  * @retval
  **/
void frame_turn_to_array(ano_frameStruct* frame,uint8_t*str)
{
    memcpy(str,(uint8_t*)frame,4);//先复制前四个字节
    memcpy(str+4,(uint8_t*)frame->data,frame->data_len);//从4个字节后开始复制数据长度（长度按字节数）
    memcpy(str+4+frame->data_len,(uint8_t*)(&frame->sum_check),2);//最后的校验位，和校验与附加校验
}

/**
 * @brief  向上位机发送ASCII字符串
 * @param  color:希望上位机显示的字符串颜色，str：要发送的字符串
 * @retval 无返回值
 */
void ano_send_string(uint8_t color,char* str)
{
   uint8_t i=0,cnt=0;
   uint8_t buff[MAX_LENGTH+6];
   memset(send_frame_struct.data,0,MAX_LENGTH);
   send_frame_struct.function_id=0XA0;           //<信息输出--字符串(功能码0XA0)
   send_frame_struct.data[cnt++]=color;          //<选择上位机打印的颜色
   /*字符串数据直接存入数据部分*/
   while(*(str+i)!='\0')
   {
       send_frame_struct.data[cnt++]=*(str+i++);
       if(cnt>MAX_LENGTH)                                //<若字符串长度超过40，强制结束
           break;
   }
   send_frame_struct.data_len=cnt;               //<记录下数据部分长度

   ano_check_calculate(&send_frame_struct);      //<计算校验和
   frame_turn_to_array(&send_frame_struct,buff); //<通信帧转线性数组
   ano_usart_send(buff,6+send_frame_struct.data_len);
   
}

///**
// * @brief  向上位机发送ASCII字符串+数字组合
// * @param  value:32位的数值，str：要发送的字符串
// * @retval
// */
// void ano_send_message(char* str,int32_t value)
// {
//   uint8_t i=0,cnt=0;
//   uint8_t buff[46];
//   memset(send_frame_struct.data,0,40);
//   send_frame_struct.function_id=0XA1;           //信息输出--字符串+数字
//
//   /*协议规定VAL在前，先对要求的32位数据进行截断*/
//   send_frame_struct.data[cnt++]=BYTE0(value);
//   send_frame_struct.data[cnt++]=BYTE1(value);
//   send_frame_struct.data[cnt++]=BYTE2(value);
//   send_frame_struct.data[cnt++]=BYTE3(value);
//   /*再轮到字符串数据*/
//   while(*(str+i)!='\0')
//   {
//       send_frame_struct.data[cnt++]=*(str+i++);
//       if(cnt>40)
//           break;
//   }
//
//   send_frame_struct.data_len=cnt;               //<记录下数据部分长度
//
//   ano_check_calculate(&send_frame_struct);      //<计算校验和
//   frame_turn_to_array(&send_frame_struct,buff); //<通信帧转线性数组
//   ano_usart_send(buff,6+send_frame_struct.data_len);
// }

/**
  * @brief  发送灵活格式帧
  * @param  id:0xF1~0XFA，x_coordinate:x轴坐标值 ，y_coordinate：y轴坐标值
  *         !!!要传多少个参数完全可以自己进行计算，最高只支持40字节的数据，低位先输出
  *         一般10个以内够用，40个字节限制，一个32位数据占4个字节，可以发送10个
  * @retval
**/
void ano_send_flexible_frame(uint8_t id,int32_t data1,int32_t data2,int32_t data3,int32_t data4,int32_t data5,int32_t data6,int32_t data7,int32_t data8,int32_t data9,int32_t data10)
{
    uint8_t buff[MAX_LENGTH+6];//帧头，地址，ID，长度，40个字节，两个校验位

    memset(send_frame_struct.data,0,40);
    send_frame_struct.function_id=id;
    send_frame_struct.data_len=40;//10个int32_t类型数据，4*10=40字节              //<根据自己的参数数填写

    /*第一个data1数据从低位到高位截断*/
    send_frame_struct.data[0]=BYTE0(data1);
    send_frame_struct.data[1]=BYTE1(data1);
    send_frame_struct.data[2]=BYTE2(data1);
    send_frame_struct.data[3]=BYTE3(data1);
    /*第二个数据data2从低位到高位截断*/
    send_frame_struct.data[4]=BYTE0(data2);
    send_frame_struct.data[5]=BYTE1(data2);
    send_frame_struct.data[6]=BYTE2(data2);
    send_frame_struct.data[7]=BYTE3(data2);
    /*第三个数据data3从低位到高位截断*/
    send_frame_struct.data[8]=BYTE0(data3);
    send_frame_struct.data[9]=BYTE1(data3);
    send_frame_struct.data[10]=BYTE2(data3);
    send_frame_struct.data[11]=BYTE3(data3);
    /*第四个数据data4从低位到高位截断*/
    send_frame_struct.data[12]=BYTE0(data4);
    send_frame_struct.data[13]=BYTE1(data4);
    send_frame_struct.data[14]=BYTE2(data4);
    send_frame_struct.data[15]=BYTE3(data4);
    /*第四个数据data5从低位到高位截断*/
    send_frame_struct.data[16]=BYTE0(data5);
    send_frame_struct.data[17]=BYTE1(data5);
    send_frame_struct.data[18]=BYTE2(data5);
    send_frame_struct.data[19]=BYTE3(data5);
    /*第四个数据data6从低位到高位截断*/
    send_frame_struct.data[20]=BYTE0(data6);
    send_frame_struct.data[21]=BYTE1(data6);
    send_frame_struct.data[22]=BYTE2(data6);
    send_frame_struct.data[23]=BYTE3(data6);
    /*第四个数据data7从低位到高位截断*/
    send_frame_struct.data[24]=BYTE0(data7);
    send_frame_struct.data[25]=BYTE1(data7);
    send_frame_struct.data[26]=BYTE2(data7);
    send_frame_struct.data[27]=BYTE3(data7);
    /*第四个数据data8从低位到高位截断*/
    send_frame_struct.data[28]=BYTE0(data8);
    send_frame_struct.data[29]=BYTE1(data8);
    send_frame_struct.data[30]=BYTE2(data8);
    send_frame_struct.data[31]=BYTE3(data8);
    /*第四个数据data9从低位到高位截断*/
    send_frame_struct.data[32]=BYTE0(data9);
    send_frame_struct.data[33]=BYTE1(data9);
    send_frame_struct.data[34]=BYTE2(data9);
    send_frame_struct.data[35]=BYTE3(data9);
    /*第四个数据data10从低位到高位截断*/
    send_frame_struct.data[36]=BYTE0(data10);
    send_frame_struct.data[37]=BYTE1(data10);
    send_frame_struct.data[38]=BYTE2(data10);
    send_frame_struct.data[39]=BYTE3(data10);

    ano_check_calculate(&send_frame_struct);
    frame_turn_to_array(&send_frame_struct,buff);

    //UART_PutBuff(UART0, buff, 6+send_frame_struct.data_len);
    ano_usart_send(buff,6+send_frame_struct.data_len);
}

/**
  * @brief  发送指定格式帧
  * @param  无
  * @retval 无
**/
void ANO_polling(void){

/**************************ANO****************************/			
			
ano_send_flexible_frame(ANO_ID,
(int32_t)gzz1,                      //云台陀螺仪角速度
(int32_t)gzz,                       //小车陀螺仪角速度
(int32_t)Yaw_Angle_Pid.out,         //角度环输出
(int32_t)(Yaw_Angle_Pid.error*100), //角度误差值
(int32_t)(Yaw_Angle_Pid.target*100),      //角度目标值
(int32_t)(yaw*100),                 //角度值
(int32_t)Angle2,
(int32_t)Task4_Turn_flag*1000,
(int32_t)ANO_2,
(int32_t)FB_Velo*100
			);
}

/**
  * @brief  接收数据帧
  * @param  byte:从串口发回的数据，一个字节。处理函数对串口返回值一个一个字节处理，
  * 确保最大限度捕获住包
  * @retval 无
**/
inline void ano_receive(uint8_t byte){
  //主处理函数
  switch(byte){
    //当读取到帧头
    case FRAME_HEADER:
    //写入帧头
      rec_frame_struct.head = byte;
      //增加读取的index，当RecvIndex从0到1的时候，打开下面的default case
      RecvIndex++;
      break;

    default:
    //当RecvIndex 大于0表示 进入一个帧的 "读取中" 状态
      if(RecvIndex>0){
        //前三个字节是固定的
        if(RecvIndex<3){
          *(((uint8_t*)(&rec_frame_struct.head)) + RecvIndex) = byte;

        //第四个字节传回长度，一方面传给struct，一方面传给DataLength方便使用
        }else if(RecvIndex==3){
          *(((uint8_t*)(&rec_frame_struct.head)) + RecvIndex) = byte;
          DataLength = byte;
					if(DataLength>MAX_LENGTH){
						//读取到的长度就是错的，重置
						RecvIndex = 0;
						return;
					}
          //清空data缓冲区
					memset(&rec_frame_struct.data,-1,MAX_LENGTH);

        //读取数据部分
        }else if(RecvIndex<DataLength+4){
          *(((uint8_t*)(&rec_frame_struct.head)) + RecvIndex) = byte;

        //读取sum_check
        }else if(RecvIndex == DataLength+4){
          *(((uint8_t*)(&rec_frame_struct.sum_check))) = byte;

        //读取add_check
        }else if(RecvIndex == DataLength+5){
          *(((uint8_t*)(&rec_frame_struct.add_check))) = byte;

          //进入push_frame，进一步处理
          push_frame();
          //重置函数状态
					RecvIndex = 0;
					return;
        }else{
          //当长度大于以上所有可能，直接硬重置
					RecvIndex = 0;
					return;
				}

        RecvIndex++;
      }
  }
}

/**
  * @brief  推送数据帧
  * @param  对数据帧进行校验，没有问题就可以做处理了
  * @retval 无
**/
void push_frame(){
  if(ano_check(&rec_frame_struct)==1){
    //没问题
		ano_recv_process();
  }else{
    //大问题
		uint8_t ok[] = "Transfer error";
    uart_write_buffer(UART_INDEX, ok, 9);  
  }
}

/**
  * @brief  调用回调函数
  * @param  push_frame在此处调用回调函数
  * @retval 无
**/
void ano_recv_process(){
  if(AnoCallVar!=NULL){
    AnoCallVar(rec_frame_struct);
  }
}

/**
  * @brief  设置回调函数
  * @param  NewCallback:设置ano回调函数
  * @retval 无
**/
void ano_set_callback(ano_recv_callback_proc NewCallback){
  AnoCallVar = NewCallback;
}

/**
  * @brief  格式化输入函数
  * @param  format_str:通过格式化方式从接收ano缓冲区读取数据
  * @param  ...:传入待赋值的变量地址(&var)来获取数值
  * @retval 读取字节数
**/
int ano_scanf(const char* format_str,...){
  va_list va;
  va_start(va,format_str);

  uint16_t index = 0;
  for(const char *p=format_str;*p!='\0';p++){
    switch(*p){
      case 'b'://byte for uint8_t
      do{
        uint8_t *pSet = va_arg(va,uint8_t*);
        *pSet = rec_frame_struct.data[index];
        index += 1;
      }while(0);
      break;
      case 'w'://word for uint16_t
      do{
        uint16_t *pSet = (uint16_t*)va_arg(va,uint8_t*);
				memcpy(pSet,rec_frame_struct.data+index,2);
        index += 2;
      }while(0);
      break;
      case 'i'://int for uint32_t
      do{
        uint32_t *pSet = (uint32_t*)va_arg(va,uint8_t*);
				memcpy(pSet,rec_frame_struct.data+index,4);
        index += 4;
      }while(0);
      break;
      default:
      //do nothing here.
      break;
    }

    if(index >  rec_frame_struct.data_len){
      //读取超限，立即失败
      return -1;
    }
  }
  va_end(va);

  //返回读取字节数
  return index;
}

/**
  * @brief  格式化输出函数
  * @param  format_str:通过格式化方式从输出ano数据
  * @param  ...:传入待赋值的变量地址(&var)来获取数值
  * @retval 输出字节数
**/
int ano_printf(const char* format_str,...){
  va_list va;
	
  va_start(va,format_str);

  uint16_t index = 0;
  uint8_t buff[MAX_LENGTH+6];
	memset(buff,0,MAX_LENGTH+6);
	
	const char *p=format_str;
  for(;*p!='\0';p++){
    switch(*p){
      case 'b'://byte for uint8_t
      do{
        uint8_t *pSet = va_arg(va,uint8_t*);
        *((uint8_t*)(send_frame_struct.data + index)) = *pSet;
        index += 1;
      }while(0);
      break;
      case 'w'://word for uint16_t
      do{
        uint16_t *pSet = (uint16_t*)va_arg(va,uint8_t*);
				memcpy(send_frame_struct.data + index,pSet,2);
        index += 2;
      }while(0);
      break;
      case 'i'://int for uint32_t
      do{
        uint32_t *pSet = (uint32_t*)va_arg(va,uint8_t*);
				memcpy(send_frame_struct.data + index,pSet,4);
        index += 4;
      }while(0);
      break;
      default:
      //do nothing here.
      break;
    }
    if(index >  MAX_LENGTH){
      //写入超限，立即失败
      return -1;
    }
  }
  send_frame_struct.data_len = index;
  
  ano_check_calculate(&send_frame_struct);      //<计算校验和
  frame_turn_to_array(&send_frame_struct,buff); //<通信帧转线性数组
  ano_usart_send(buff,6+send_frame_struct.data_len);

  //返回读取字节数
  return index;
}

/**
  * @brief  设置输出的地址
  * @param  d_addr:待设置地址
  * @retval 无
**/
void ano_set_address(uint8_t d_addr){
	send_frame_struct.target_addr = d_addr;
}

/**
  * @brief  设置输出的包的类型(id)
  * @param  id:待设置id
  * @retval 无
**/
void ano_set_type(uint8_t id){
	send_frame_struct.function_id = id;
}



void ANO_DMA_init(void){
#ifndef UART1_DMA	//UART1_DMA CH1
	
//uart_dma_init (UART_1, 115200, UART1_TX_B6, UART1_RX_B7 ,UART1_INT_IRQn , 2 , 1);//初始化DMA，后两位是优先级和通道
//    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART2->RXDATA));
//	//将DMA的源地址设置为UART的数据接收寄存器（RXDATA）,当UART收到数据时，DMA自动从RXDATA读取数据，无需CPU干预。
//    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ReadBuffWit[0]);
//  //将DMA的目标地址指向全局数组ReadBuffWit。  
//	  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, WITSIZE);
//  //设置DMA单次传输的数据长度（单位为字节）。 
//	  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
//  //启用DMA通道0，并通过循环等待确保启用成功。
//    /* Confirm DMA channel is enabled */
//    while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH0_CHAN_ID)) {
//        __BKPT(0);
//    }
//    NVIC_EnableIRQ(UART1_INT_IRQn);
//		//使能UART2的中断，用于处理接收完成、错误等事件。
//	
//                                          // 输出换行
//	
	
	
	
	
#else   //UART2_DMA CH0
	  
		ano_set_callback(Get_ANO);
		
	  uart_dma_init (UART_1, 115200, UART1_TX_B6, UART1_RX_B7 ,UART1_INT_IRQn , 2 );//初始化DMA，后两位是优先级和通道
	    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&UART1->RXDATA));
	//将DMA的源地址设置为UART的数据接收寄存器（RXDATA）,当UART收到数据时，DMA自动从RXDATA读取数据，无需CPU干预。
    	DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &ReadBuffAno[0]);
  //将DMA的目标地址指向全局数组ReadBuffWit。  
	  DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, ANOSIZE);
  //设置DMA单次传输的数据长度（单位为字节）。 
	  DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
  //启用DMA通道0，并通过循环等待确保启用成功。
    /* Confirm DMA channel is enabled */
    while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH1_CHAN_ID)) {
        __BKPT(0);
    }
    NVIC_EnableIRQ(UART1_INT_IRQn);
		//使能UART1的中断，用于处理接收完成、错误等事件。

#endif


}

void ANO_Getdata(void)
{
if(ANO_Flag){	
	
for(int i=0;i<ANOSIZE;i++)
	{
	ano_receive(ReadBuffAno[i]);
	}
	ANO_Flag=0;
}
}



void ANO_debug(void){

		fifo_ano_count = fifo_used(&ANO_data_fifo);                             // 查看 fifo 是否有数据
		if(fifo_ano_count != 0)                                                  // 读取到数据了
        {
    fifo_read_buffer(&ANO_data_fifo, fifo_get_ANO, &fifo_ano_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
		for(int a=0;a<fifo_ano_count;a++){
			ano_receive(fifo_get_ANO[a]);
		}		                                      
    }
}

void Drive_Get(void){
	if( get_ano == 0x6B)                                                       //确认是否是以回车结尾
	{
		fifo_ano_count = fifo_used(&ANO_data_fifo);                             // 查看 fifo 是否有数据
		if(fifo_ano_count != 0 )                                                  // 读取到数据了
        {
    fifo_read_buffer(&ANO_data_fifo, fifo_get_ANO, &fifo_ano_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
//	  uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);
		if(
			 fifo_get_ANO[0]==0x01&&
			 fifo_get_ANO[1]==0xFD&&
			 fifo_get_ANO[2]==0x9F&&
       fifo_get_ANO[3]==0x6B
		)
		
		{
		if( fifo_get_ANO[0]==0x01)
		{
//		uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);	
		Drive_Flag1=0;//俯仰角已到位
//		memset(fifo_get_ANO, '\0', 64);	
//		uart_write_byte(UART_0,1);	
		}
		}
    else
	   return;

}
}
}
//————————————————
//
//版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
//
//原文链接：https://blog.csdn.net/csol1607408930/article/details/123929485












//void ANO_polling(void){

///**************************ANO****************************/			
//			
//ano_send_flexible_frame(ANO_ID,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0,
//(int32_t)0
//			);
//}

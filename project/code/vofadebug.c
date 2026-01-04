#include "zf_common_headfile.h"

extern fifo_struct uart_data_fifo;
extern uint8 fifo_get_data[64];                // fifo 输出读出缓冲区
extern uint8 get_data;                         // 接收数据变量

uint32 fifo_data_count = 0;                                                     // fifo 数据个数
uint8_t VofaDataBuff[64];                     //串口接收缓冲区
uint16_t VofaIndex = 0;
uint8_t VofaString[50];                        //串口接收的数据前的变量名称

uint32_t ANO_tx_1;
uint32_t ANO_tx_2;
uint32_t ANO_tx_3;

float velo;
int shuaige=200;

// x^y 次方
float vofapower(float x, int y){
    float temp = 1;
    for(int i = 0; i < y; i++){
        temp *= x;
    }
    return temp;
}

/**
  * @brief 从收到的字符数组中提取出数字和前置字符
  * @param 
  * @retval 
  */

float vofa_get_number(void){
    int FloatIndex = 0;         //小数点所在位置
    int StartIndex = 0;         //数据开始位置
    int EndIndex = 0;               //数据结束位置
    int NegaFlag = 0;               //负数标志位
    float re = 0.0f;

    for(int i = 0; i < 64; i++){
        if(VofaDataBuff[i] == '='){         // '=' + 1为数据开始位
            VofaString[i] = '\0';           //获取字符串结束，加空字符
            if(VofaDataBuff[i + 1] == '-'){ //数据开始为为负数
                NegaFlag = 1;
                StartIndex = i + 2;
            }else
                StartIndex = i + 1;
        }
        if(StartIndex == 0){                    //此时还未获取到数字，存储字符串
                VofaString[i] = VofaDataBuff[i];
        }
        if(VofaDataBuff[i] == '.')          // '.'
            FloatIndex = i;

        if(VofaDataBuff[i] == '\n'){        // '\n' 为数据结束位
            EndIndex = i;
            break;
        }
    }

    if(FloatIndex > StartIndex){            //收到小数点
        for(int i = StartIndex; i < FloatIndex; i++){               //整数位
            re += (VofaDataBuff[i] - '0') * vofapower(10, FloatIndex - i - 1);
        }

        for(int i = FloatIndex + 1; i < EndIndex; i++){             //小数位
            re += (VofaDataBuff[i] - '0') * vofapower(0.1, i - FloatIndex);
        }
    }
    else{
        for(int i = StartIndex; i < EndIndex; i++){             //整数位
            re += (VofaDataBuff[i] - '0') * vofapower(10, EndIndex - i - 1);
        }
    }
    if(NegaFlag)
        re = -re;

    return re;
}

VofaDataTypeDef VofaDataStruct[NUMSIZE] = 
{
        {
                      .string = "velo",
                      .num = &velo
        },
        {
                      .string = "shuaige",
                      .num = &shuaige
        },			
        {
                      .string = "Incremental_KP_2",
                      .num = &Incremental_KP_2
        },		
        {
                      .string = "Incremental_KI_2",
                      .num = &Incremental_KI_2
        },		
        {
                      .string = "Incremental_KD_2",
                      .num = &Incremental_KD_2
        },		
        {
                      .string = "Incremental_KP_1",
                      .num = &Incremental_KP_1
        },		
        {
                      .string = "Incremental_KI_1",
                      .num = &Incremental_KI_1
        },		
        {
                      .string = "Incremental_KD_1",
                      .num = &Incremental_KD_1
        },		
        {
                      .string = "Position_KP",
                      .num = &Position_KP
        },		
        {
                      .string = "Position_KI",
                      .num = &Position_KI
        },		
        {
                      .string = "Position_KD",
                      .num = &Position_KD
        },	
				{
                      .string = "ANO_tx_1",
                      .num = &ANO_tx_1
        },
				{
                      .string = "ANO_tx_2",
                      .num = &ANO_tx_2
        },
				{
                      .string = "ANO_tx_3",
                      .num = &ANO_tx_3
        },
				
			  {
                      .string = "Angle1",
                      .num = &Angle1
        },
				{
                      .string = "Angle2",
                      .num = &Angle2
        },
				{
                      .string = "Dir1",
                      .num = &Dir1
        },
				{
                      .string = "Dir2",
                      .num = &Dir2
        },	
			 {
                      .string = "Roll_Inc_Pid.kp",//@
                      .num = &Roll_Inc_Pid.kp
        },
			 {
                      .string = "jishu",//%
                      .num = &Time
        },				
			  {
                      .string = "RollX_step",//*
                      .num = &RollX_step
        },
				{
                      .string = "Angle_Speed",//*
                      .num = &Angle_Speed
        },
				
				{
                      .string = "Yaw_Angle_Pid_max",//@
                      .num = &Yaw_Angle_Pid.pwm_out_max
        },		
				
				{
                      .string = "Yaw_Angle_Pid_kp",//@
                      .num = &Yaw_Angle_Pid.kp
        },		
				{
                      .string = "Yaw_Angle_Pid_ki",//@
                      .num = &Yaw_Angle_Pid.ki
        },
				{
                      .string = "Yaw_Angle_Pid_kd",//@
                      .num = &Yaw_Angle_Pid.kd
        },
				{
                      .string = "xishu1",//@
                      .num = &xishu1
        },
				{
                      .string = "xishu2",//@
                      .num = &xishu2
        },	
				{
                      .string = "Speed_Kp1",//@
                      .num = &Speed_Kp1
        },
				{
                      .string = "Speed_Kp2",//@
                      .num = &Speed_Kp2
        },				
//jishu			 
				
};


void change_para(void){
    float data = vofa_get_number();
    for(int i = 0; i < (sizeof (VofaDataStruct) / sizeof (VofaDataTypeDef)); i++){
        if(strcmp((char *)VofaDataStruct[i].string , (char *)(VofaString + 1)) == 0){
            if(VofaString[0] =='@'){            //修改的数据类型为float
                float *ptr = (float*)VofaDataStruct[i].num;
                *ptr = data;
                /////////////////////////////////////////////////
//                if(strcmp((char *)"Car_Price.Ture_Error" , (char *)(VofaString + 1)) == 0)
//                    turn_Flag = 1;
            }
            else if(VofaString[0] =='#'){       //修改的数据类型为int16_t
                int16_t *ptr = (int16_t*)VofaDataStruct[i].num;
                *ptr = (int16_t)data;
            }
            else if(VofaString[0] =='&'){       //修改的数据类型为int
                int *ptr = (int*)VofaDataStruct[i].num;
                *ptr = (int)data;
            }
						else if(VofaString[0] =='$'){       //修改的数据类型为uint32_t
                uint32_t *ptr = (uint32_t*)VofaDataStruct[i].num;
                *ptr = (uint32_t)data;
            }
						else if(VofaString[0] =='*'){       //修改的数据类型为uint32_t
                uint16_t *ptr = (uint16_t*)VofaDataStruct[i].num;
                *ptr = (uint16_t)data;
            }
						else if(VofaString[0] =='%'){       //修改的数据类型为uint32_t
                uint8_t *ptr = (uint8_t*)VofaDataStruct[i].num;
                *ptr = (uint8_t)data;
            }
//            else if(VofaString[0] =='t'){       //修改的数据类型为int
//                int *ptr = (int*)VofaDataStruct[i].num;
//                *ptr = (int)data;
//            }
        }

    }
}

void vofa(void){
//VofaDataBuff[VofaIndex++] = UART_GetChar(UART0);
	uart_query_byte(UART_INDEX, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
	fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 将数据写入 fifo 中
}		

void Vofa_debug(void){
	
//	if( get_data == 0x6B)                                                       //确认是否是以回车结尾
//	{
//		fifo_data_count = fifo_used(&uart_data_fifo);                             // 查看 fifo 是否有数据
//		if(fifo_data_count != 0 )                                                  // 读取到数据了
//        {
//    fifo_read_buffer(&uart_data_fifo, VofaDataBuff, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
////	  uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);
//		if(
//			VofaDataBuff[0]==0x01&&
//			VofaDataBuff[1]==0xFD&&
//			VofaDataBuff[2]==0x9F&&
//      VofaDataBuff[3]==0x6B
//		)
//		
//		{
//		if( VofaDataBuff[0]==0x01)
//		{
////		uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);	
//		Drive_Flag1=0;//俯仰角已到位
////		memset(fifo_get_ANO, '\0', 64);	
////		uart_write_byte(UART_0,1);	
//		}
//		}
//    else
//	   return;

//}
//}


	if( get_data == '\n')                                                       //确认是否是以回车结尾
	{
		fifo_data_count = fifo_used(&uart_data_fifo);                             // 查看 fifo 是否有数据
		if(fifo_data_count != 0)                                                  // 读取到数据了
        {
    fifo_read_buffer(&uart_data_fifo, VofaDataBuff, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
	  change_para();                                                            // 修改参数
		//uart_write_buffer(UART_INDEX, VofaDataBuff, fifo_data_count);             // 将读取到的数据发送出去			
		memset(VofaDataBuff, '\0', 64);			                                      // 清空缓存区
    }
	}



}
//uart_data_fifo
//get_data	
//fifo_data_count
//VofaDataBuff
	
//	if( get_data == '\n')                                                       //确认是否是以回车结尾
//	{
//		fifo_data_count = fifo_used(&uart_data_fifo);                             // 查看 fifo 是否有数据
//		if(fifo_data_count != 0)                                                  // 读取到数据了
//        {
//    fifo_read_buffer(&uart_data_fifo, VofaDataBuff, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
//	  change_para();                                                            // 修改参数
//		uart_write_buffer(UART_INDEX, VofaDataBuff, fifo_data_count);             // 将读取到的数据发送出去			
//		memset(VofaDataBuff, '\0', 64);			                                      // 清空缓存区
//    }
//	}
//}

//if( get_ano == 0x6B)                                                       //确认是否是以回车结尾
//	{
//		fifo_ano_count = fifo_used(&ANO_data_fifo);                             // 查看 fifo 是否有数据
//		if(fifo_ano_count == 4)                                                  // 读取到数据了
//        {
//    fifo_read_buffer(&ANO_data_fifo, fifo_get_ANO, &fifo_ano_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
//	  uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);
//		if(
//			 fifo_get_ANO[0]==0xFD&&
//			 fifo_get_ANO[1]==0xFD&&
//			 fifo_get_ANO[2]==0x9F&&
//       fifo_get_ANO[3]==0x6B
//		)
//		
//		{
//		if( fifo_get_ANO[0]==0x01)
//		{
////		uart_write_buffer(UART_0,(uint8_t *)fifo_get_ANO,4);	
//		Drive_Flag1=0;//俯仰角已到位
////		memset(fifo_get_ANO, '\0', 64);	
////		uart_write_byte(UART_0,1);	
//		}
//		}
//    else
//	   return;

//}
//}

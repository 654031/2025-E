#include "zf_common_headfile.h"

///全存float型数据
///数据任意0号页码
#define FLASH_SECTION_INDEX1       (0)                                 // 存储数据用的扇区
#define FLASH_PAGE_INDEX1         (0)                                 // 存储数据用的页码 倒数第一个页码

uint8 xuanze[4] = {1,0,0,0};

uint8 write_flag=0;
float cunchu[20]={1.0,1.2,1.4,1.5,1.7,1.6,1.33,1.54,1.97,1.36,1.0,1.2,1.4,1.5,1.7,1.6,1.33,1.54,1.97,1.36};
float duqu[20];
uint8 length;
///如果不够同样写新的
void flash_init(void)///判断要不要重写
{
	//用哪个扇区复制到哪个扇区
	if(xuanze[0] == 1)
	{
		if(flash_check(FLASH_SECTION_INDEX1, FLASH_PAGE_INDEX1)) 
		{
			write_flag = 1;
		}
		else
		{
			write_flag = 0;	
		}
	}
	
}
void flash_demo(void)
{
	uint8 i;
	uint8 j;
	length = sizeof(cunchu) / sizeof(cunchu[0]);
	if(write_flag==0) ///写一遍位置
	{
		flash_buffer_clear(); /////写之前先清0
		flash_erase_page(FLASH_SECTION_INDEX1, FLASH_PAGE_INDEX1);///擦除原来区域 
		for(i=0;i<20;i++)
		{
			flash_union_buffer[i].float_type = cunchu[i];
		}
		flash_write_page_from_buffer(FLASH_SECTION_INDEX1, FLASH_PAGE_INDEX1);     // 向指定 Flash 扇区的页码写入缓冲区数据
		system_delay_ms(200);
	}
	else ///直接读取位置
	{
		flash_read_page_to_buffer(FLASH_SECTION_INDEX1, FLASH_PAGE_INDEX1);
		for(j= 0; j < 20; j++) 
		{
			duqu[j] = flash_union_buffer[j].float_type;	
		}
	}
}
void jiance(void)
{
	uint8 k;
		for(k= 0; k < 20; k++) 
		{
			printf("\r\n %d : %f", k,duqu[k]);
		}
}



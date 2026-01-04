#ifndef _FLASH_H_
#define _FLASH_H_
#include "zf_common_headfile.h"


extern uint8 write_flag;
extern float cunchu[20];
extern float duqu[20];
extern uint8 length;
extern uint8 xuanze[4];

void flash_init(void);	

/*	
	void flash_demo(void);
	把cunchu[]的内容写进flash
*/
void flash_demo(void);
void jiance(void);

#endif
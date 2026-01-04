#ifndef __PIT_H_
#define __PIT_H_

#include "zf_common_headfile.h"

#define PIT_CH                  (PIT_TIM_G12 )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIMG12_INT_IRQn)                                   // 对应周期中断的中断编号 

extern uint8 jishu;
extern float last_angle_zhuan;
void pit_1ms_init(void);

#endif


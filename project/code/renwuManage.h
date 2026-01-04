#ifndef __RENWUMANAGE_H
#define	__RENWUMANAGE_H

#include "zf_common_headfile.h"

extern uint8 renwu ;//1 2 3 4 5 6
extern uint8 page ;//0第一界面（基本信息） 1参数界面
extern uint8 bianliang;
extern uint8 car_delay_2s_flag;			// 车等两秒再开

void startManage(void);	// 控制电机启动还是小车启动
void controlManage_IT(void);	// 控制调用哪一个问题（哪个control）

#endif

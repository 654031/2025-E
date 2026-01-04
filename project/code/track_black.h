#include "zf_common_headfile.h"

#define ADC_N 8//灯的个数

#define D1 (A0 )
#define D2 (A1 )
#define D3 (A16)
#define D4 (A17)
#define D5 (A28)
#define D6 (A29)
#define D7 (B2 )
#define D8 (B3 )

//根据检测到黑线的状态来改变值
#define BLACK_EXIST 1
#define BLACK_NOT_EXIST 0

extern uint8 track_arr[8];
extern float xun;									// 与4.5作比较
extern float last_xun;

/**************左8右1******************/

void track_init(void);
void read_track_pin(void);

void track_zhixian(void);

void grayscale_get_2(uint8_t *gray_sensor);
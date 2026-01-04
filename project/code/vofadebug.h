#ifndef _VOFADEBUG_H_
#define _VOFADEBUG_H_
#include "zf_common_headfile.h"

typedef struct{
    uint8_t string[50];
    void *num;
}VofaDataTypeDef;

#define NUMSIZE 150         //要修改的变量个数

void vofa(void);
void change_para(void);
void Vofa_debug(void);

extern float velo;
extern int shuaige;

#endif
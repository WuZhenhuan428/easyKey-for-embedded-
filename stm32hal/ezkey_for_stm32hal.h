#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

#define DOUBLECLICK_TIME 1000
#define KEEPON_TIME      1000
#define READ_DELAY       20     //若回调函数时间周期为1s 则读取间隔为20ms

//定义按键的个数
#define numKeys 4

typedef struct key KEY ;

KEY* KEY_Init ( GPIO_TypeDef* KeyGPIOx , uint16_t KeyPIN , uint8_t KeyActiveLevel);
uint8_t KEY_UP( KEY* key );
void KEY_Tick();


#endif
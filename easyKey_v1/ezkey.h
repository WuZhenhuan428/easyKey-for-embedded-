#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

#define DOUBLECLICK_TIME 1000
#define KEEPON_TIME      1000

/*
若定时器回调函数时间周期为1s 则读取间隔为20ms  
If the timer callback function has a period of 1 second, then the reading interval is 20 ms.

*/ 
#define READ_DELAY       20    

typedef void* pGPIO;
typedef struct key* KEY ;

//定义你所需要的最大按键的数量
//define the maximum number of keys you need to use.
#define numKeys 4


//Dependency Injection  :  write your read io function here
uint8_t io_read( pGPIO GPIOx, uint16_t PIN);


KEY KEY_Init ( pGPIO KeyGPIOx , uint16_t KeyPIN , uint8_t KeyActiveLevel);
uint8_t KEY_UP( KEY key );
void KEY_Tick();



#endif
#include "key.h"
#include <stdint.h>

//KEY结构体定义
struct key
{
  GPIO_TypeDef* GPIOx;
  uint16_t      PIN;
  uint8_t       ACTIVE_LEVEL;  //按键的有效电平
  uint8_t       pre_isPress;  
  uint8_t       isPress;
  uint8_t       keepTime;
  uint8_t       isUp;
};

//KEY按键数组（包含全部按键）
KEY KeyArray[numKeys];

//即代表下一个要初始化的按键，代表目前已经初始化的个数
volatile static uint8_t numInitKey = 0 ;

KEY* KEY_Init ( GPIO_TypeDef* KeyGPIOx , uint16_t KeyPIN , uint8_t KeyActiveLevel)
{

  
  //Key初始化
  (&KeyArray[numInitKey])->GPIOx = KeyGPIOx;
  (&KeyArray[numInitKey])->PIN = KeyPIN;
  (&KeyArray[numInitKey])->ACTIVE_LEVEL = KeyActiveLevel;
  (&KeyArray[numInitKey])->pre_isPress = 0 ;
  (&KeyArray[numInitKey])->isPress = 0 ;
  (&KeyArray[numInitKey])->keepTime = 0;

  //IO初始化
  if( KeyArray[numInitKey].GPIOx == GPIOA ) __HAL_RCC_GPIOA_CLK_ENABLE();
  else if( KeyArray[numInitKey].GPIOx == GPIOB ) __HAL_RCC_GPIOB_CLK_ENABLE();
  else if( KeyArray[numInitKey].GPIOx == GPIOC ) __HAL_RCC_GPIOC_CLK_ENABLE();
  else if( KeyArray[numInitKey].GPIOx == GPIOD ) __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};  
  GPIO_InitStruct.Pin = KeyArray[numInitKey].PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( KeyArray[numInitKey].GPIOx, &GPIO_InitStruct );
  
  //指向下一个初始化按键
  numInitKey++;
  
  return &KeyArray[numInitKey-1];

}

void KEY_Scan()
{
  for( int i = 0 ; i < numInitKey ; i++  )
  {
    (&KeyArray[i])->pre_isPress = (&KeyArray[i])->isPress ;
    if( HAL_GPIO_ReadPin( KeyArray[i].GPIOx , KeyArray[i].PIN ) == KeyArray[i].ACTIVE_LEVEL )  
         (&KeyArray[i])->isPress = 1 ;
    else (&KeyArray[i])->isPress = 0 ;

  }
}


void KEY_UP_Compare()
{
  for( int i = 0 ;  i < numInitKey ; i++ )
  {
    if( !KeyArray[i].isPress && KeyArray[i].pre_isPress )  
         (&KeyArray[i])->isUp = 1 ;
  }
}


void KEY_Tick()
{
  static uint8_t key_count ; 
  key_count++;

  //经过READ_DELAY次进入按键扫描
  if( key_count > READ_DELAY ){
    KEY_Scan();
    KEY_UP_Compare();

    key_count = 0 ;
  }
}


uint8_t KEY_UP( KEY* key )
{
  if( key->isUp )
  {
    key->isUp = 0;
    return 1;
  }
  return 0; 
}

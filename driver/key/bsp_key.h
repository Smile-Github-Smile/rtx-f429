#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"

//引脚定义
/*******************************************************/

#define KEY_UP_PIN                  GPIO_Pin_0                 
#define KEY_UP_GPIO_PORT            GPIOA                    
#define KEY_UP_GPIO_CLK             RCC_AHB1Periph_GPIOA

#define KEY_DOWN_PIN                GPIO_Pin_2                 
#define KEY_DOWN_GPIO_PORT          GPIOH                     
#define KEY_DOWN_GPIO_CLK           RCC_AHB1Periph_GPIOH

#define KEY_LEFT_PIN                GPIO_Pin_13                 
#define KEY_LEFT_GPIO_PORT          GPIOC                      
#define KEY_LEFT_GPIO_CLK           RCC_AHB1Periph_GPIOC


#define KEY_RIGHT_PIN               GPIO_Pin_3                 
#define KEY_RIGHT_GPIO_PORT         GPIOH                    
#define KEY_RIGHT_GPIO_CLK          RCC_AHB1Periph_GPIOH


typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_UP_DOWN,			/* 上键按下 */
	KEY_UP_UP,				/* 上键弹起 */
	KEY_UP_LONG,			/* 上键长按 */

	KEY_DOWN_DOWN,			/* 下键按下 */
	KEY_DOWN_UP,			/* 下键弹起 */
	KEY_DOWN_LONG,			/* 下键长按 */

	KEY_LEFT_DOWN,			/* 左键按下 */
	KEY_LEFT_UP,			/* 左键弹起 */
	KEY_LEFT_LONG,			/* 左键长按 */

	KEY_RIGHT_DOWN,			/* 右键按下 */
	KEY_RIGHT_UP,			/* 右键弹起 */
	KEY_RIGHT_LONG,			/* 右键长按 */
	
}KEY_ENUM;


/* 按键FIFO大小 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
	
}KEY_FIFO_T;

/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

#endif /* __LED_H */


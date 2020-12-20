#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"
#include "stdio.h"

#define KEY_COUNT    4	   					/* 按键个数, 4个独立建 */

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



/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
typedef enum
{
	KID_JOY_U = 0,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	
}KEY_ID_E;



/*
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* 单位10ms， 持续1秒，认为长按事件 */


/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键是否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	
}KEY_T;


/* 按键FIFO大小 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
	
}KEY_FIFO_T;



/*
* 按键按下标置宏
* 按键按下为高电平，设置 KEY_ON=0， KEY_OFF=1
*/
#define KEY_ON	1
#define KEY_OFF	0

void bsp_InitKey(void);
uint8_t bsp_GetKey(void);
void bsp_KeyScan(void);

#endif /* __LED_H */


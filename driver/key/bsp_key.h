#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"

//���Ŷ���
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
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_UP_DOWN,			/* �ϼ����� */
	KEY_UP_UP,				/* �ϼ����� */
	KEY_UP_LONG,			/* �ϼ����� */

	KEY_DOWN_DOWN,			/* �¼����� */
	KEY_DOWN_UP,			/* �¼����� */
	KEY_DOWN_LONG,			/* �¼����� */

	KEY_LEFT_DOWN,			/* ������� */
	KEY_LEFT_UP,			/* ������� */
	KEY_LEFT_LONG,			/* ������� */

	KEY_RIGHT_DOWN,			/* �Ҽ����� */
	KEY_RIGHT_UP,			/* �Ҽ����� */
	KEY_RIGHT_LONG,			/* �Ҽ����� */
	
}KEY_ENUM;


/* ����FIFO��С */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
	
}KEY_FIFO_T;

/*******************************************************/

 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

#endif /* __LED_H */


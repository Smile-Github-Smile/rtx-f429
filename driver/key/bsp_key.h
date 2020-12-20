#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"
#include "stdio.h"

#define KEY_COUNT    4	   					/* ��������, 4�������� */

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



/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_JOY_U = 0,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	
}KEY_ID_E;



/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */


/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����Ƿ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatCount;	/* �������������� */
	uint8_t  RepeatSpeed;	/* ������������ */
	
}KEY_T;


/* ����FIFO��С */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
	
}KEY_FIFO_T;



/*
* �������±��ú�
* ��������Ϊ�ߵ�ƽ������ KEY_ON=0�� KEY_OFF=1
*/
#define KEY_ON	1
#define KEY_OFF	0

void bsp_InitKey(void);
uint8_t bsp_GetKey(void);
void bsp_KeyScan(void);

#endif /* __LED_H */


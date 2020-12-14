/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  */ 
#include "FreeRTOS.h"
#include "task.h" 
#include "bsp_key.h" 

/// ����ȷ����ʱ
void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��������GPIO�ڵ�ʱ�� */
	RCC_AHB1PeriphClockCmd(KEY0_GPIO_CLK | KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);

	/* ��������Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 

	/* ѡ�񰴼������� */
	GPIO_InitStructure.GPIO_Pin = KEY0_PIN; 
	
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/* ��ʼ������ */
	GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure); 

	/* ѡ�񰴼������� */
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN; 
	
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/* ��ʼ������ */
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);  	
	
	/* ѡ�񰴼������� */
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN; 
	
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/* ��ʼ������ */
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);  
}

/**
  * @brief   ����Ƿ��а�������     
  *	@param 	GPIOx:����Ķ˿�, x�����ǣ�A...K�� 
  *	@param 	GPIO_PIN:����Ķ˿�λ�� ������GPIO_PIN_x��x������0...15��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )  
	{	 
		/* ��ʱ������ */
		Key_Delay(100);
		
		/* �ٴμ�ⰴ���Ƿ��� */
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON ) 
		{
			/* �ȴ������ͷ� */
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
			{
				vTaskDelay(20);
			} 
			
			return 	KEY_ON;	
		}
		return KEY_OFF;
	}
	else
	{
		return KEY_OFF;
	}
}
/*********************************************END OF FILE**********************/

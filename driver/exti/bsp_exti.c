/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  */
  
#include "bsp_exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �����ж�Դ������WKUP */
	NVIC_InitStructure.NVIC_IRQChannel = WKUP_INT_EXTI_IRQ;
	/* ������ռ���ȼ���10 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	/* ���������ȼ���0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* ��������GPIO�ڵ�ʱ�� */
	RCC_AHB1PeriphClockCmd(WKUP_INT_GPIO_CLK, ENABLE);

	/* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* ���� NVIC */
	NVIC_Configuration();

	/* ѡ�񰴼�WKUP������ */ 
	GPIO_InitStructure.GPIO_Pin = WKUP_INT_GPIO_PIN;
	/* ��������Ϊ����ģʽ */ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	/* ʹ������Ľṹ���ʼ������ */
	GPIO_Init(WKUP_INT_GPIO_PORT, &GPIO_InitStructure); 

	/* ���� EXTI �ж�Դ��WKUP���� */
	SYSCFG_EXTILineConfig(WKUP_INT_EXTI_PORTSOURCE, WKUP_INT_EXTI_PINSOURCE);

	/* ѡ�� EXTI �ж�Դ */
	EXTI_InitStructure.EXTI_Line = WKUP_INT_EXTI_LINE;
	/* �ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½��ش��� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	/* ʹ���ж�/�¼��� */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}
/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    bsp.h
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   Ӧ�ú����ӿ�
  ******************************************************************************
  */

#ifndef _BSP_H_
#define _BSP_H


/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.0"

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()



#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_debug_usart.h"


/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

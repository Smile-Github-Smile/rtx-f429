/**
  ******************************************************************************
  * @file    bsp.h
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   应用函数接口
  ******************************************************************************
  */

#ifndef _BSP_H_
#define _BSP_H


/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.0"

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()



#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_debug_usart.h"


/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

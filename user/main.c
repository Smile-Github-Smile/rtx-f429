/**
  ******************************************************************************
  * @file    main.c
  * @author  jfy
  * @version V1.0
  * @date    2020-12-14
  * @brief   控制多彩流水灯
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "bsp_led.h"
#include "includes.h"			

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void AppTaskCreate (void);
__task void AppTaskLED(void);
__task void AppTaskStart(void);

/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/
static uint64_t AppTaskLEDStk[256/8];     /* 任务栈 */
static uint64_t AppTaskStartStk[512/8];   /* 任务栈 */

/* 任务句柄 */
OS_TID HandleTaskLED = NULL;

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main (void) 
{	
	/* 初始化外设 */
	LED_GPIO_Config();
	
	/* 创建启动任务 */
 	os_sys_init_user (AppTaskStart,             /* 任务函数 */
	                  2,                        /* 任务优先级 */
	                  &AppTaskStartStk,         /* 任务栈 */
	                  sizeof(AppTaskStartStk)); /* 任务栈大小，单位字节数 */
	while(1);
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskLED
*	功能说明: LED闪烁	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
__task void AppTaskLED(void)
{
    while(1)
    {
		LED2( ON );			 // 亮 
        os_dly_wait(200);
		LED2( OFF );		 // 灭
		os_dly_wait(200);

    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务，也就是最高优先级任务。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
BOOL flag = 0;
__task void AppTaskStart(void)
{
	AppTaskCreate();
	
	
    while(1)
    {
		flag = 1;
		LED1( ON );			 // 亮 
        os_dly_wait(500);
		flag = 0;
		LED1( OFF );		 // 灭
		os_dly_wait(500);
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	HandleTaskLED = os_tsk_create_user(AppTaskLED,              /* 任务函数 */ 
	                                   1,                       /* 任务优先级 */ 
	                                   &AppTaskLEDStk,          /* 任务栈 */
	                                   sizeof(AppTaskLEDStk));  /* 任务栈大小，单位字节数 */
}



/*********************************************END OF FILE**********************/


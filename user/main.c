/**
  ******************************************************************************
  * @file    main.c
  * @author  jfy
  * @version V1.0
  * @date    2020-12-14
  * @brief   控制多彩流水灯
  ******************************************************************************
  */
#include "includes.h"			

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
__task void AppTaskStart(void);
__task void AppTaskLED(void);
__task void AppTaskUserIF(void);
__task void AppTaskMsgPro(void);


static void AppTaskCreate (void);

/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/
/* 任务栈空间 */
static uint64_t AppTaskStartStk[256/8];   /* 任务栈 */
static uint64_t AppTaskLEDStk[256/8];     /* 任务栈 */
static uint64_t AppTaskUserIFStk[256/8];  /* 任务栈 */
static uint64_t AppTaskMsgProStk[256/8];  /* 任务栈 */

/* 任务句柄 */
OS_TID HandleTaskLED = NULL;
OS_TID HandleTaskUserIF = NULL;
OS_TID HandleTaskMsgPro = NULL;



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
	bsp_Init();
	
	/* 创建启动任务 */
 	os_sys_init_user (AppTaskStart,             /* 任务函数 */
	                  4,                        /* 任务优先级 */
	                  &AppTaskStartStk,         /* 任务栈 */
	                  sizeof(AppTaskStartStk)); /* 任务栈大小，单位字节数 */
	while(1);
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
__task void AppTaskStart(void)
{
	AppTaskCreate();
	
    while(1)
    {
		/* 按键扫描 */
		bsp_KeyScan();
        os_dly_wait(10);
    }
}



/*
*********************************************************************************************************
*	函 数 名: AppTaskLED
*	功能说明: LED闪烁	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1  
*********************************************************************************************************
*/
__task void AppTaskLED(void)
{
    while(1)
    {
		LED1( ON );			 // 亮 
        os_dly_wait(100);
		LED1( OFF );		 // 灭
		os_dly_wait(100);
    }
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskUserIF
*	功能说明: 按键消息处理	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1  
*********************************************************************************************************
*/
__task void AppTaskUserIF(void)
{
	uint8_t ucKeyCode;
	
    while(1)
    {
		ucKeyCode = bsp_GetKey();
		
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				/* KEY_UP键按下，打印调试说明 */
				case KEY_UP_DOWN:	
					printf("KEY_UP键按下，RTX测试\r\n");
					break;
				
				/* KEY_DOWN键按下，删除任务AppTaskLED */
				case KEY_DOWN_DOWN:	
					printf("KEY_DOWN键按下，删除任务HandleTaskLED\r\n");
				
					if(HandleTaskLED != NULL)
					{
						if(os_tsk_delete(HandleTaskLED) == OS_R_OK)
						{
							HandleTaskLED = NULL;
							printf("任务AppTaskLED删除成功\r\n");
						}
						else
						{
							printf("任务AppTaskLED删除失败\r\n");					
						}
					}
					break;	

				/* KEY_LEFT键按下，打印调试说明 */
				case KEY_LEFT_DOWN:	
					printf("KEY_LEFT键按下，RTX测试\r\n");
					break;					
				
				/* KEY_RIGHT键按下，重新创建任务AppTaskLED */
				case KEY_RIGHT_DOWN:	
					printf("KEY_LEFT键按下，重新创建任务AppTaskLED\r\n");
				
					if(HandleTaskLED == NULL)
					{
						HandleTaskLED = os_tsk_create_user(AppTaskLED,              /* 任务函数 */ 
														   1,                       /* 任务优先级 */ 
														   &AppTaskLEDStk,          /* 任务栈 */
														   sizeof(AppTaskLEDStk));  /* 任务栈大小，单位字节数 */
					}
					break;
					
				/* 其他的键值不处理 */
				default:                     
					break;
			}
		}
		
		os_dly_wait(20);
		
    }
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskMsgPro
*	功能说明: 消息处理	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1  
*********************************************************************************************************
*/
__task void AppTaskMsgPro(void)
{
    while(1)
    {
		LED2( ON );			 // 亮 
        os_dly_wait(500);
		LED2( OFF );		 // 灭
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
	HandleTaskLED = os_tsk_create_user(AppTaskLED,              	/* 任务函数 */ 
	                                   1,                       	/* 任务优先级 */ 
	                                   &AppTaskLEDStk,          	/* 任务栈 */
	                                   sizeof(AppTaskLEDStk));  	/* 任务栈大小，单位字节数 */
	
	HandleTaskUserIF = os_tsk_create_user(AppTaskUserIF,            /* 任务函数 */ 
	                                   3,                       	/* 任务优先级 */ 
	                                   &AppTaskUserIFStk,          	/* 任务栈 */
	                                   sizeof(AppTaskUserIFStk));  	/* 任务栈大小，单位字节数 */
	
	HandleTaskMsgPro = os_tsk_create_user(AppTaskMsgPro,            /* 任务函数 */ 
	                                   2,                       	/* 任务优先级 */ 
	                                   &AppTaskMsgProStk,          	/* 任务栈 */
	                                   sizeof(AppTaskMsgProStk));  	/* 任务栈大小，单位字节数 */
}





/*********************************************END OF FILE**********************/


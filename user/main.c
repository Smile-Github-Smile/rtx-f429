/**
  ******************************************************************************
  * @file    main.c
  * @author  jfy
  * @version V1.0
  * @date    2020-12-14
  * @brief   ���ƶ����ˮ��
  ******************************************************************************
  */
#include "includes.h"			

/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
__task void AppTaskStart(void);
__task void AppTaskLED(void);
__task void AppTaskUserIF(void);
__task void AppTaskMsgPro(void);


static void AppTaskCreate (void);

/*
**********************************************************************************************************
											 ����
**********************************************************************************************************
*/
/* ����ջ�ռ� */
static uint64_t AppTaskStartStk[256/8];   /* ����ջ */
static uint64_t AppTaskLEDStk[256/8];     /* ����ջ */
static uint64_t AppTaskUserIFStk[256/8];  /* ����ջ */
static uint64_t AppTaskMsgProStk[256/8];  /* ����ջ */

/* ������ */
OS_TID HandleTaskLED = NULL;
OS_TID HandleTaskUserIF = NULL;
OS_TID HandleTaskMsgPro = NULL;



/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main (void) 
{	
	/* ��ʼ������ */
	bsp_Init();
	
	/* ������������ */
 	os_sys_init_user (AppTaskStart,             /* ������ */
	                  4,                        /* �������ȼ� */
	                  &AppTaskStartStk,         /* ����ջ */
	                  sizeof(AppTaskStartStk)); /* ����ջ��С����λ�ֽ��� */
	while(1);
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ��������Ҳ����������ȼ�����
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************
*/
__task void AppTaskStart(void)
{
	AppTaskCreate();
	
    while(1)
    {
		/* ����ɨ�� */
		bsp_KeyScan();
        os_dly_wait(10);
    }
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskLED
*	����˵��: LED��˸	
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 1  
*********************************************************************************************************
*/
__task void AppTaskLED(void)
{
    while(1)
    {
		LED1( ON );			 // �� 
        os_dly_wait(100);
		LED1( OFF );		 // ��
		os_dly_wait(100);
    }
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: ������Ϣ����	
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 1  
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
				/* KEY_UP�����£���ӡ����˵�� */
				case KEY_UP_DOWN:	
					printf("KEY_UP�����£�RTX����\r\n");
					break;
				
				/* KEY_DOWN�����£�ɾ������AppTaskLED */
				case KEY_DOWN_DOWN:	
					printf("KEY_DOWN�����£�ɾ������HandleTaskLED\r\n");
				
					if(HandleTaskLED != NULL)
					{
						if(os_tsk_delete(HandleTaskLED) == OS_R_OK)
						{
							HandleTaskLED = NULL;
							printf("����AppTaskLEDɾ���ɹ�\r\n");
						}
						else
						{
							printf("����AppTaskLEDɾ��ʧ��\r\n");					
						}
					}
					break;	

				/* KEY_LEFT�����£���ӡ����˵�� */
				case KEY_LEFT_DOWN:	
					printf("KEY_LEFT�����£�RTX����\r\n");
					break;					
				
				/* KEY_RIGHT�����£����´�������AppTaskLED */
				case KEY_RIGHT_DOWN:	
					printf("KEY_LEFT�����£����´�������AppTaskLED\r\n");
				
					if(HandleTaskLED == NULL)
					{
						HandleTaskLED = os_tsk_create_user(AppTaskLED,              /* ������ */ 
														   1,                       /* �������ȼ� */ 
														   &AppTaskLEDStk,          /* ����ջ */
														   sizeof(AppTaskLEDStk));  /* ����ջ��С����λ�ֽ��� */
					}
					break;
					
				/* �����ļ�ֵ������ */
				default:                     
					break;
			}
		}
		
		os_dly_wait(20);
		
    }
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskMsgPro
*	����˵��: ��Ϣ����	
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 1  
*********************************************************************************************************
*/
__task void AppTaskMsgPro(void)
{
    while(1)
    {
		LED2( ON );			 // �� 
        os_dly_wait(500);
		LED2( OFF );		 // ��
		os_dly_wait(500);
    }
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	HandleTaskLED = os_tsk_create_user(AppTaskLED,              	/* ������ */ 
	                                   1,                       	/* �������ȼ� */ 
	                                   &AppTaskLEDStk,          	/* ����ջ */
	                                   sizeof(AppTaskLEDStk));  	/* ����ջ��С����λ�ֽ��� */
	
	HandleTaskUserIF = os_tsk_create_user(AppTaskUserIF,            /* ������ */ 
	                                   3,                       	/* �������ȼ� */ 
	                                   &AppTaskUserIFStk,          	/* ����ջ */
	                                   sizeof(AppTaskUserIFStk));  	/* ����ջ��С����λ�ֽ��� */
	
	HandleTaskMsgPro = os_tsk_create_user(AppTaskMsgPro,            /* ������ */ 
	                                   2,                       	/* �������ȼ� */ 
	                                   &AppTaskMsgProStk,          	/* ����ջ */
	                                   sizeof(AppTaskMsgProStk));  	/* ����ջ��С����λ�ֽ��� */
}





/*********************************************END OF FILE**********************/


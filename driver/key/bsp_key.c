/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  */ 

#include "bsp_key.h" 

static KEY_T p_tBtn[KEY_COUNT];
static KEY_FIFO_T p_tKey;		/* ����FIFO�������ṹ�� */


static void Key_GPIO_Config(void);
static uint8_t Key_UP_Scan(void);
static uint8_t Key_DOWN_Scan(void);
static uint8_t Key_LEFT_Scan(void);
static uint8_t Key_RIGHT_Scan(void);
static void bsp_PutKey(uint8_t _KeyCode);
static void bsp_InitKeyVar(void);
static void bsp_DetectKey(uint8_t index);


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
	Key_GPIO_Config();		/* ��ʼ������Ӳ�� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (p_tKey.Read == p_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = p_tKey.Buf[p_tKey.Read];

		if (++p_tKey.Read >= KEY_FIFO_SIZE)
		{
			p_tKey.Read = 0;
		}
		return ret;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���,������.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t ii;

	for (ii = 0; ii < KEY_COUNT; ii++)
	{
		bsp_DetectKey(ii);
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
//static void bsp_ClearKey(void)
//{
//	p_tKey.Read = p_tKey.Write;
//}


/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
//static uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
//{
//	return p_tBtn[_ucKeyID].State;
//}


/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//static void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
//{
//	p_tBtn[_ucKeyID].LongTime = _LongTime;			        /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
//	p_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
//	p_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
//}


/*
******************************************************************************
*	ҡ��UP��   : PA0   (�͵�ƽ��ʾ����)
*	ҡ��DOWN�� : PH2   (�͵�ƽ��ʾ����)   KEY1
*	ҡ��LEFT�� : PC13  (�͵�ƽ��ʾ����)   KEY2
*	ҡ��RIGHT��: PH3   (�͵�ƽ��ʾ����)   KEY0
******************************************************************************
*/


/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
static void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��������GPIO�ڵ�ʱ�� */
	RCC_AHB1PeriphClockCmd(KEY_UP_GPIO_CLK | KEY_DOWN_GPIO_CLK | KEY_LEFT_GPIO_CLK | KEY_RIGHT_GPIO_CLK, ENABLE);

	/* ��������Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN; 
	GPIO_Init(KEY_UP_GPIO_PORT, &GPIO_InitStructure); 
	
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = KEY_DOWN_PIN; 
	GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStructure); 
 	
	GPIO_InitStructure.GPIO_Pin = KEY_LEFT_PIN; 
	GPIO_Init(KEY_LEFT_GPIO_PORT, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = KEY_RIGHT_PIN; 
	GPIO_Init(KEY_RIGHT_GPIO_PORT, &GPIO_InitStructure);  
}

/**
  * @brief   ���Key_UP�Ƿ��а�������     
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
static uint8_t Key_UP_Scan(void)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(KEY_UP_GPIO_PORT, KEY_UP_PIN) == 1)  
	{	 
		return 	KEY_ON;	
	}
	else
	{
		return  KEY_OFF;
	}
}

/**
  * @brief   ���Key_DOWN�Ƿ��а�������     
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
static uint8_t Key_DOWN_Scan(void)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(KEY_DOWN_GPIO_PORT, KEY_DOWN_PIN) == 0 )  
	{	 
		return 	KEY_ON;	
	}
	else
	{
		return  KEY_OFF;
	}
}

/**
  * @brief   ���Key_LEFT�Ƿ��а�������     
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
static uint8_t Key_LEFT_Scan(void)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(KEY_LEFT_GPIO_PORT, KEY_LEFT_PIN) == 0 )  
	{	 
		return 	KEY_ON;	
	}
	else
	{
		return  KEY_OFF;
	}
}

/**
  * @brief   ���Key_RIGHT�Ƿ��а�������     
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
static uint8_t Key_RIGHT_Scan(void)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(KEY_RIGHT_GPIO_PORT, KEY_RIGHT_PIN) == 0 )  
	{	 
		return 	KEY_ON;	
	}
	else
	{
		return  KEY_OFF;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_PutKey(uint8_t _KeyCode)
{
	p_tKey.Buf[p_tKey.Write] = _KeyCode;

	if (++p_tKey.Write  >= KEY_FIFO_SIZE)
	{
		p_tKey.Write = 0;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	p_tKey.Read = 0;
	p_tKey.Write = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		p_tBtn[i].LongTime = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		p_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		p_tBtn[i].State = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
		p_tBtn[i].RepeatSpeed = 0;					/* �����������ٶȣ�0��ʾ��֧������ */
		p_tBtn[i].RepeatCount = 0;					/* ���������� */
	}

	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	/* ���磬����ϣ������1���³���1����Զ��ط���ͬ��ֵ */
	p_tBtn[KID_JOY_U].LongTime = 100;
	p_tBtn[KID_JOY_U].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	p_tBtn[KID_JOY_D].LongTime = 100;
	p_tBtn[KID_JOY_D].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	p_tBtn[KID_JOY_L].LongTime = 100;
	p_tBtn[KID_JOY_L].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	p_tBtn[KID_JOY_R].LongTime = 100;
	p_tBtn[KID_JOY_R].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	/* �жϰ������µĺ��� */
	p_tBtn[0].IsKeyDownFunc = Key_UP_Scan;
	p_tBtn[1].IsKeyDownFunc = Key_DOWN_Scan;
	p_tBtn[2].IsKeyDownFunc = Key_LEFT_Scan;
	p_tBtn[3].IsKeyDownFunc = Key_RIGHT_Scan;
	
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t index)
{
	KEY_T *pBtn;
	pBtn = &p_tBtn[index];

	if (pBtn == NULL)
	{
		return;
	}
	
	if (pBtn->IsKeyDownFunc())
	{
		//����˲�������
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * index + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey((uint8_t)(3 * index + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey((uint8_t)(3 * index + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * index + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}


/*********************************************END OF FILE**********************/

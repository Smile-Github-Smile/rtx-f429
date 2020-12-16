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

static KEY_FIFO_T p_tKey;		/* ����FIFO�������ṹ�� */

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
	RCC_AHB1PeriphClockCmd(KEY_UP_GPIO_CLK | KEY_DOWN_GPIO_CLK | KEY_LEFT_PIN | KEY_RIGHT_GPIO_CLK, ENABLE);

	/* ��������Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	/* ������������ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN; 
	GPIO_Init(KEY_UP_GPIO_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = KEY_DOWN_PIN; 
	GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStructure); 
 	
	GPIO_InitStructure.GPIO_Pin = KEY_LEFT_PIN; 
	GPIO_Init(KEY_LEFT_GPIO_PORT, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = KEY_RIGHT_PIN; 
	GPIO_Init(KEY_RIGHT_GPIO_PORT, &GPIO_InitStructure);  
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
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
			
			return 	KEY_ON;	
		}
		return KEY_OFF;
	}
	else
	{
		return KEY_OFF;
	}
}

/*
 ******************************************************************************
 *	ҡ��UP��   : PA0   (�͵�ƽ��ʾ����)
 *	ҡ��DOWN�� : PH2   (�͵�ƽ��ʾ����)   KEY1
 *	ҡ��LEFT�� : PC13  (�͵�ƽ��ʾ����)   KEY2
 *	ҡ��RIGHT��: PH3   (�͵�ƽ��ʾ����)   KEY0
 ******************************************************************************
 */


/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	p_tKey.Buf[p_tKey.Write] = _KeyCode;

	if (++p_tKey.Write  >= KEY_FIFO_SIZE)
	{
		p_tKey.Write = 0;
	}
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
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	p_tKey.Read = p_tKey.Write;
}



/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  */ 

#include "bsp_key.h" 

static KEY_T p_tBtn[KEY_COUNT];
static KEY_FIFO_T p_tKey;		/* 按键FIFO变量，结构体 */


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
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* 初始化按键变量 */
	Key_GPIO_Config();		/* 初始化按键硬件 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
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
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键,非阻塞.
*	形    参:  无
*	返 回 值: 无
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
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
//static void bsp_ClearKey(void)
//{
//	p_tKey.Read = p_tKey.Write;
//}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参:  _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
//static uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
//{
//	return p_tBtn[_ucKeyID].State;
//}


/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参：_ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
//static void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
//{
//	p_tBtn[_ucKeyID].LongTime = _LongTime;			        /* 长按时间 0 表示不检测长按键事件 */
//	p_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* 按键连发的速度，0表示不支持连发 */
//	p_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
//}


/*
******************************************************************************
*	摇杆UP键   : PA0   (低电平表示按下)
*	摇杆DOWN键 : PH2   (低电平表示按下)   KEY1
*	摇杆LEFT键 : PC13  (低电平表示按下)   KEY2
*	摇杆RIGHT键: PH3   (低电平表示按下)   KEY0
******************************************************************************
*/


/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
static void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启按键GPIO口的时钟 */
	RCC_AHB1PeriphClockCmd(KEY_UP_GPIO_CLK | KEY_DOWN_GPIO_CLK | KEY_LEFT_GPIO_CLK | KEY_RIGHT_GPIO_CLK, ENABLE);

	/* 设置引脚为输入模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	
	/* 设置引脚下拉 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN; 
	GPIO_Init(KEY_UP_GPIO_PORT, &GPIO_InitStructure); 
	
	/* 设置引脚上拉 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = KEY_DOWN_PIN; 
	GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStructure); 
 	
	GPIO_InitStructure.GPIO_Pin = KEY_LEFT_PIN; 
	GPIO_Init(KEY_LEFT_GPIO_PORT, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = KEY_RIGHT_PIN; 
	GPIO_Init(KEY_RIGHT_GPIO_PORT, &GPIO_InitStructure);  
}

/**
  * @brief   检测Key_UP是否有按键按下     
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
static uint8_t Key_UP_Scan(void)
{			
	/* 检测是否有按键按下 */
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
  * @brief   检测Key_DOWN是否有按键按下     
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
static uint8_t Key_DOWN_Scan(void)
{			
	/* 检测是否有按键按下 */
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
  * @brief   检测Key_LEFT是否有按键按下     
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
static uint8_t Key_LEFT_Scan(void)
{			
	/* 检测是否有按键按下 */
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
  * @brief   检测Key_RIGHT是否有按键按下     
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
static uint8_t Key_RIGHT_Scan(void)
{			
	/* 检测是否有按键按下 */
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
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	p_tKey.Read = 0;
	p_tKey.Write = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		p_tBtn[i].LongTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
		p_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		p_tBtn[i].State = 0;						/* 按键缺省状态，0为未按下 */
		p_tBtn[i].RepeatSpeed = 0;					/* 按键连发的速度，0表示不支持连发 */
		p_tBtn[i].RepeatCount = 0;					/* 连发计数器 */
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
	/* 比如，我们希望按键1按下超过1秒后，自动重发相同键值 */
	p_tBtn[KID_JOY_U].LongTime = 100;
	p_tBtn[KID_JOY_U].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	p_tBtn[KID_JOY_D].LongTime = 100;
	p_tBtn[KID_JOY_D].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	p_tBtn[KID_JOY_L].LongTime = 100;
	p_tBtn[KID_JOY_L].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	p_tBtn[KID_JOY_R].LongTime = 100;
	p_tBtn[KID_JOY_R].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	/* 判断按键按下的函数 */
	p_tBtn[0].IsKeyDownFunc = Key_UP_Scan;
	p_tBtn[1].IsKeyDownFunc = Key_DOWN_Scan;
	p_tBtn[2].IsKeyDownFunc = Key_LEFT_Scan;
	p_tBtn[3].IsKeyDownFunc = Key_RIGHT_Scan;
	
}


/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参:  按键结构变量指针
*	返 回 值: 无
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
		//软件滤波计数器
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

				/* 发送按钮按下的消息 */
				bsp_PutKey((uint8_t)(3 * index + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
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
							/* 常按键后，每隔10ms发送1个按键 */
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

				/* 发送按钮弹起的消息 */
				bsp_PutKey((uint8_t)(3 * index + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}


/*********************************************END OF FILE**********************/

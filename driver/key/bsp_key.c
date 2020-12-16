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

static KEY_FIFO_T p_tKey;		/* 按键FIFO变量，结构体 */

/// 不精确的延时
void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启按键GPIO口的时钟 */
	RCC_AHB1PeriphClockCmd(KEY_UP_GPIO_CLK | KEY_DOWN_GPIO_CLK | KEY_LEFT_PIN | KEY_RIGHT_GPIO_CLK, ENABLE);

	/* 设置引脚为输入模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	/* 设置引脚上拉 */
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
  * @brief   检测是否有按键按下     
  *	@param 	GPIOx:具体的端口, x可以是（A...K） 
  *	@param 	GPIO_PIN:具体的端口位， 可以是GPIO_PIN_x（x可以是0...15）
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{			
	/* 检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )  
	{	 
		/* 延时消抖动 */
		Key_Delay(100);
		
		/* 再次检测按键是否按下 */
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON ) 
		{
			/* 等待按键释放 */
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
 *	摇杆UP键   : PA0   (低电平表示按下)
 *	摇杆DOWN键 : PH2   (低电平表示按下)   KEY1
 *	摇杆LEFT键 : PC13  (低电平表示按下)   KEY2
 *	摇杆RIGHT键: PH3   (低电平表示按下)   KEY0
 ******************************************************************************
 */


/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	p_tKey.Read = p_tKey.Write;
}



/*********************************************END OF FILE**********************/

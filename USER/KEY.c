
#include "main.h"





uint8_t key;







uint8_t MATRIX_key(void)/*行扫描*/
{
    uint8_t key_num = 0;
		/*第一行*/
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
	 {
//				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0)
				{
						key_num = 1;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
				{
						key_num = 2;
				}
	 			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{
						key_num = 3;
						
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
				{
						key_num = 4;

				}
	 }
	 /*第二行*/
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
	 {
//				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0)
				{
						key_num = 5;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
				{
						key_num = 6;
				}
	 			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{
						key_num = 7;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
				{
						key_num = 8;
				}
	 }
	 /*第三行*/
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
	 {
//				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0)
				{
						key_num = 9;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
				{
						key_num = 10;
				}
	 			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{
						key_num = 11;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
				{
						key_num = 12;
				}
	 }
	 	 /*第四行*/
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0||HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
	 {
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==0)
				{
						key_num = 13;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
				{
						key_num = 14;
				}
	 			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{
						key_num = 15;
				}
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0)
				{
						key_num = 16;
				}
	 }
	return key_num;
}




uint8_t key_loop()
{
	static unsigned char before_key,now_key;
  before_key = now_key;
	now_key = MATRIX_key();		//获取按键标志位

	
    /*1.一开始没按键按下时before_state = 0,now_state = 0
      2.当按键1按下没松开时，now_state = 1,此时before_state = 0，不满足下面任何的判断
      3.20ms后该函数再次被调用，before_state = now_state = 1
      4.按键1松开，now_state = 0，此时before_state = 1，满足下面第一条if判断，key_keyNumber置1
    */
    
	//上一个状态有按键按下，现在状态没有按键按下，说明处于按下后松手阶段
    //若if(before_state == 0 && now_state == 1)，则是按键按下瞬间为真，标志位置1，不用等	
  if(before_key == 1&&now_key ==0)
	{
			key= 1;
	}
	  
	if(before_key == 2&&now_key ==0)
	{
			key= 2;
	}  
	if(before_key == 3&&now_key ==0)
	{
			key= 3;
	} 
	if(before_key == 4&&now_key ==0)
	{
			key= 4;
	}  
	if(before_key == 5&&now_key ==0)
	{
			key= 5;
	}  
	if(before_key == 6&&now_key ==0)
	{
			key= 6;
	}  
	if(before_key == 7&&now_key ==0)
	{
			key= 7;
	}  
	if(before_key == 8&&now_key ==0)
	{
			key= 8;
	}  
	if(before_key == 9&&now_key ==0)
	{
			key= 9;
	}  
	if(before_key == 10&&now_key ==0)
	{
			key= 10;
	}  
	if(before_key == 11&&now_key ==0)
	{
			key= 11;
	}  
	if(before_key == 12&&now_key ==0)
	{
			key= 12;
	}  
	if(before_key == 13&&now_key ==0)
	{
			key= 13;
	}  
	if(before_key == 14&&now_key ==0)
	{
			key= 14;
	}  
	if(before_key == 15&&now_key ==0)
	{
			key= 15;
	}  
	if(before_key == 16&&now_key ==0)
	{
			key= 16;
	}
	if(before_key==0&&now_key==0)
	{
			key=0;
	}
	return key;
}

                        


#include "main.h"





uint8_t key;







uint8_t MATRIX_key(void)/*��ɨ��*/
{
    uint8_t key_num = 0;
		/*��һ��*/
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
	 /*�ڶ���*/
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
	 /*������*/
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
	 	 /*������*/
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
	now_key = MATRIX_key();		//��ȡ������־λ

	
    /*1.һ��ʼû��������ʱbefore_state = 0,now_state = 0
      2.������1����û�ɿ�ʱ��now_state = 1,��ʱbefore_state = 0�������������κε��ж�
      3.20ms��ú����ٴα����ã�before_state = now_state = 1
      4.����1�ɿ���now_state = 0����ʱbefore_state = 1�����������һ��if�жϣ�key_keyNumber��1
    */
    
	//��һ��״̬�а������£�����״̬û�а������£�˵�����ڰ��º����ֽ׶�
    //��if(before_state == 0 && now_state == 1)�����ǰ�������˲��Ϊ�棬��־λ��1�����õ�	
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

                        

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "dht22.h"

int main(void)
{ 
	u8 t;
	u16 temperature;  	    
	u16 humidity;    
	delay_init(168);	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //������ʼ��
	OLED_Init();
	DHT22_Init();
	printf("Initialized successfully");


	
	while(1) 
	{		
    if(t%100==0)			//ÿ100ms��ȡһ��
		{			  
			DHT22_Read_Data(&temperature,&humidity); //��ȡ��ʪ��ֵ
			printf("temperatyre: %d\n",temperature);
			printf("humidity: %d\n",humidity);
			
//			OLED_ShowNum(40,32,temperature,3,16);    //��ʾ�¶�	   		   
//			OLED_ShowNum(40,48,humidity,3,16);		   //��ʾʪ��	 	
//			OLED_Refresh_Gram();
		}				   
	 	delay_ms(2);
		t++;
//		LED_BLUE=!LED_BLUE;
	}	
}


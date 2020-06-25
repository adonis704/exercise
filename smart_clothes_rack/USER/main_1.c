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
	delay_init(168);	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //按键初始化
	OLED_Init();
	DHT22_Init();
	printf("Initialized successfully");


	
	while(1) 
	{		
    if(t%100==0)			//每100ms读取一次
		{			  
			DHT22_Read_Data(&temperature,&humidity); //读取温湿度值
			printf("temperatyre: %d\n",temperature);
			printf("humidity: %d\n",humidity);
			
//			OLED_ShowNum(40,32,temperature,3,16);    //显示温度	   		   
//			OLED_ShowNum(40,48,humidity,3,16);		   //显示湿度	 	
//			OLED_Refresh_Gram();
		}				   
	 	delay_ms(2);
		t++;
//		LED_BLUE=!LED_BLUE;
	}	
}


#include <main.h>

int Is_out_Flag;


void show_background();

int main(void)
{
	u8 t;
	u8 gesture;
	bool last_flag = false;
	Is_out_Flag = true;
	u16 temperature;
	u16 humidity;
	u16 ffff;
	init();

	show_background();
	OLED_Refresh_Gram();
	while (1)
	{
		if (last_flag != Is_out_Flag)
		{
			OLED_16x16Ch(4, 4, 9 + 2 * Is_out_Flag, 1);	// ��/��
			OLED_16x16Ch(5, 4, 10 + 2 * Is_out_Flag, 1);	//��/��
			ENA = 1;
			IN0 = Is_out_Flag;
			IN1 = last_flag;
			ENA = 1;
			delay_ms(1500);
			IN1 = !IN1;
			delay_ms(30);
			ENA = 0;
			last_flag = Is_out_Flag;
		}
		ffff = handleGesture();
		printf("%d", ffff);
		if(ffff!=100 && ffff != 0)
			Is_out_Flag = ! Is_out_Flag;
		delay_ms(100);
		//		LED_BLUE=!LED_BLUE;
	}
}

u8 handleGesture()
{

	if (isGestureAvailable())
	{
		printf("scaning\n");
		LED1 = 0;
		switch (readGesture())
		{
		case DIR_UP:
			LED0 = 0;
			LED1 = 0;
			printf("Up\n");
			Is_out_Flag = true;
			return DIR_UP;
			break;
		case DIR_DOWN:
			LED0 = 1;
			LED1 = 0;
			printf("Down!\n");
			Is_out_Flag = false;
			return DIR_DOWN;
			break;
		case DIR_LEFT:
			LED0 = 0;
			LED1 = 1;
			printf("Left!\n");
			Is_out_Flag = false;
			return DIR_LEFT;
			break;
		case DIR_RIGHT:
			LED0 = 1;
			LED1 = 1;
			printf("Right!\n");
			Is_out_Flag = true;
			return DIR_RIGHT;
			break;
		case DIR_NEAR:
			printf("Near!\n");
			return DIR_NEAR;
			break;
		case DIR_FAR:
			printf("Far!\n");
			return DIR_FAR;
			break;
		default:
			printf("Wrong!\n");
			LED0 = 0;
			return 100;
			break;
		}
	}
}

void show_background()
{
	OLED_Clear();
	OLED_16x16Ch(1, 1, 1, 1);//��
	OLED_16x16Ch(2, 1, 2, 1);//��
	OLED_16x16Ch(3, 1, 3, 1);//ѧ
	OLED_16x16Ch(6, 1, 4, 1);//��
	OLED_16x16Ch(7, 1, 5, 1);//��
	OLED_16x16Ch(8, 1, 6, 1);//��
	OLED_16x16Ch(3, 2, 14, 1);//�¶ȼ�
	OLED_16x16Ch(4, 2, 15, 1);//��
	OLED_ShowChar(80, 16, '.', 16, 1);
	OLED_ShowChar(80, 32, '.', 16, 1);
	OLED_16x16Ch(7, 2, 16, 1);//���϶�
	OLED_16x16Ch(3, 3, 13, 1);//RH
	OLED_16x16Ch(4, 3, 15, 1);//��
	OLED_16x16Ch(7, 3, 17, 1);//%
	OLED_16x16Ch(1, 4, 7, 1);//��
	OLED_16x16Ch(2, 4, 8, 1);//��
	OLED_16x16Ch(3, 4, 15, 1);//��
	OLED_16x16Ch(4, 4, 9, 1);	// ��/��
	OLED_16x16Ch(5, 4, 10, 1);	//��/��
}

void init()
{
	delay_init(168);	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();			     //LED�˿ڳ�ʼ��
	IO_Init();
	KEY_Init();          //������ʼ��
//	EXTIX_Init();
	delay_ms(10);
	DHT22_Init();
	delay_ms(100);
	OLED_Init();
	SparkFun_APDS9960();
	if (SparkFun_APDS9960_init())
	{
		delay_ms(10);
		if (enableGestureSensor(true))
			printf("Initialized successfully");
		
	}
	else printf("+9956236513");
	delay_ms(10);
	TIM3_Int_Init(15000 - 1, 8400 - 1);	//��ʱ��ʱ��84M,����15000��Ϊ1.5s     
}
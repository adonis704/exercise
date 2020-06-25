#ifndef MAIN_H
#define MAIN_H
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "dht22.h"
#include "APDS_9960.h"
#include "exti.h"
#include <stdbool.h>
#include "IO.h"
#include "myiic.h"
#include "timer.h"

void init();
u8 handleGesture();

#endif
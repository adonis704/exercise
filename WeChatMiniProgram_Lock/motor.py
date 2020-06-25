import RPi.GPIO as GPIO
#导入时间库，用于控制延时
import time
#导入硬件控制相关的库文件
import atexit
#GP/IO信息的清理
atexit.register(GPIO.cleanup)  
#设定控制端口号
servopin = 21
#设定GP/IO工作在BCM模式下
GPIO.setmode(GPIO.BCM)
#顺背GPIO口
GPIO.setup(servopin, GPIO.OUT, initial=False)
#设置GPIO脉冲为PWM模式
p = GPIO.PWM(servopin,50) #50HZ
p.start(0)
time.sleep(2)
#首先正转，开门
p.ChangeDutyCycle(5) 
time.sleep(10)     
#接下来反转，放下门把手
p.ChangeDutyCycle(10)        
time.sleep(5)
#最后停止运行。
p.ChangeDutyCycle(0)
time.sleep(1)

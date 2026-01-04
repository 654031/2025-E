##############################################使用说明#################################
#KEY1按下为启动
#KEY2按下为暂停
#KEY1和KEY2同时按下为复位
#####################################################################################

##库导入
import sensor, lcd, image,time
import machine
#-----------------------------GPIO相关库---------------------------#
from Maix import GPIO
from board import board_info
from fpioa_manager import fm
#-----------------------------------------------------------------#
import utime

#lcd.init(freq=15000000) #屏幕初始化


##摄像头初始化---------------------------------------------------------------------------#
sensor.reset() # 重置并初始化感光元件 默认设置为 摄像头频率 24M 不开启双缓冲模式
sensor.set_jb_quality(70)    #图像传给上位机的质量
sensor.set_pixformat(sensor.RGB565)  #设置图像格式为 RGB565 (彩色) 除此之外 还可设置格式为 GRAYSCALE 或者 YUV422
sensor.set_framesize(sensor.QVGA) #画面质量  QVGA  或  QQVGA
sensor.set_auto_gain(0,12)      # 设置画面增益 17 dB 影响实时画面亮度
sensor.set_auto_whitebal(1) # 设置RGB增益 0 0 0 dB 影响画面色彩呈现效果
sensor.run(1)

#sensor.set_vflip(1)   # 开启垂直翻转 如果是 01Studio 的 K210 不开启会导致画面方向与运动方向相反
sensor.set_hmirror(0) # 开启水平镜像 如果是 01Studio 的 K210 不开启会导致画面方向与运动方向相反 #如果摄像头装反了，可以不开
#--------------------------------------------------------------------------------------#




###外设引脚配置------------------------------------------------------------------------------#
fm.register(26, fm.fpioa.GPIOHS26)
fm.register(28,fm.fpioa.GPIOHS28)
fm.register(30,fm.fpioa.GPIOHS30)
fm.register(31, fm.fpioa.GPIOHS31)

#蜂鸣器接在30脚（蜂鸣器高/高电平触发）      灯接在28脚（灯是高电平亮）
bee = GPIO(GPIO.GPIOHS30,GPIO.OUT)
led = GPIO(GPIO.GPIOHS28,GPIO.OUT)


key =  GPIO(GPIO.GPIOHS31, GPIO.IN, GPIO.PULL_UP) #低电平触发将按键设置成上拉
key2 = GPIO(GPIO.GPIOHS26, GPIO.IN, GPIO.PULL_UP)


bee.value(0)  #arduino的是高电平触发
led.value(1)

#----------------------------------------------------------------------------------------#




###舵机PWM波初始化--------------------------------------------------------------------------#

Duty1 = 7.9 #纵   #DUTY1+  ---->上
Duty2 = 7.5 #横   #DUTY2+ ---->右(从后面看往我左)


tim0 = machine.Timer(machine.Timer.TIMER0, machine.Timer.CHANNEL0, mode=machine.Timer.MODE_PWM)
ch0 = machine.PWM(tim0, freq=50, duty=Duty1, pin=10, enable=False)
ch0.enable()
tim1 = machine.Timer(machine.Timer.TIMER0, machine.Timer.CHANNEL1, mode=machine.Timer.MODE_PWM)
ch1 = machine.PWM(tim1, freq=50, duty=Duty2, pin=11, enable=False)
ch1.enable()
ch1.duty(Duty2)
ch0.duty(Duty1)
#---------------------------------------------------------------------------------------------#


###阈值和标志位---------------------------------------------------------------------------------#
red_threshold = (26,84,85,26,-33,73) #红点阈值

red_threshold2=(17, 97,15, 73,-34,57)

red_threshold3=(0,100,17,127,-38,127)



green_threshold=(92, 72, -71, -3, -34, 69)#绿点阈值
green_threshold2=(28, 100, -55, -29, -46, 86)

#屏幕中央像素坐标
g_b=100
g_s=100


block_tim=4
flag_begin=0
flag_bee1=0
flag_bee2=0
flag_stop=0

#----------------------------------------------------------------------------------------------#



###按键-----------------------------------------------------------------------------------------#



#----------------------------------------------------------------------------------------------#


###pwm限幅--------------------------------------------------------------------------------------#
def Pwm_limit(duty):
    if duty > 11.5:
        return 11.5
    elif duty < 3.5:
        return 3.5
    else:
        return duty


#-----------------------------------------------------------------------------------------------#


###追光主逻辑函数--------------------------------------------------------------------------------#
#根据距离，实时改变duty的步进值（用函数计算）
while True:
    print(key.value())
    if key.value()==0:  #按键1 启动
        utime.sleep_ms(10)
        if key.value()==0:
            flag_begin=1

    if key2.value()==0: #按键2 停止
        utime.sleep_ms(10)
        if key2.value()==0:
            flag_begin= 0
            #Duty1 = 7.9 #纵   #DUTY1+  ---->上
            #Duty2 = 7.5 #横   #DUTY2+ ---->右(从后面看往我左)
            #ch1.duty(Duty2)
            #ch0.duty(Duty1)
    # print(key.value())

    if key.value()==0 and key2.value()==0: #同时按下连个按键————》复位
        Duty1 = 7.9 #纵   #DUTY1+  ---->上
        Duty2 = 7.5 #横   #DUTY2+ ---->右(从后面看往我左)
        ch1.duty(Duty2)
        ch0.duty(Duty1)

    if flag_begin==1: #按下开始键
        print(1)
        img=sensor.snapshot() #摄像头获取图片
        #lcd.display(img)
        blobs_2 = img.find_blobs([green_threshold2])     #找绿点


        if blobs_2:#如果找到绿点
            for bs in blobs_2:
                tmp=img.draw_rectangle(bs[0:4]) #画矩形 （x,y）:起始坐标；w:宽度；h:长度；color：颜色；thickness：边框粗细；fill:是否填充。
                tmp=img.draw_cross(bs[5], bs[6])#画十字交叉 （x,y）: 交叉坐标
                c=img.get_pixel(bs[5], bs[6]) #获取中心坐标
            g_b=bs[5]
            g_s=bs[6]
           # print(g_b,)


        blobs = img.find_blobs([red_threshold]) #找红点

        if blobs:#如果找到红点
            for b in blobs:
                tmp=img.draw_rectangle(b[0:4])
                tmp=img.draw_cross(b[5], b[6])
                c=img.get_pixel(b[5], b[6])




           # print(b[5], b[6])


#   //规定向上角度加  向下角度减 //     //向右角度加   向左角度减//

##########################duty2 是左右    duty1是上下###########################

            if b[5]> g_b + block_tim:  #绿点的x坐标比红点小
                #Key_detect() ##按键检测
                if Duty2 >2.5:
                    if b[5]-g_b-block_tim>60:
                        Duty2-=0.2*(b[5]-g_b-block_tim)/60
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    elif b[5]-g_b-block_tim>30 and b[5]-g_b-block_tim<=60:
                        Duty2-=0.1*(b[5]-g_b-block_tim)/40
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    elif b[5]-g_b-block_tim>15 and b[5]-g_b-block_tim<=30:
                        Duty2-=0.05*(b[5]-g_b-block_tim)/20
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    else:
                        Duty2-=0.03*(b[5]-g_b-block_tim)/9
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0

            elif b[5]<g_b-block_tim:

                if Duty2 <12.5:
                    #Key_detect() ##按键检测
                    if g_b-block_tim-b[5]>60:
                        Duty2+=0.2*(g_b-block_tim-b[5])/60
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    elif g_b-block_tim-b[5]>30 and g_b-block_tim-b[5]<=60:
                        Duty2+=0.1*(g_b-block_tim-b[5])/40
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    elif g_b-block_tim-b[5]>15 and g_b-block_tim-b[5]<=30:
                        Duty2+=0.05*(g_b-block_tim-b[5])/20
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
                    else:
                        Duty2+=0.03*(g_b-block_tim-b[5])/9
                        Duty2 = Pwm_limit(Duty2)
                        ch1.duty(Duty2)
                        flag_bee1=0
            else:

                flag_bee1=1

            if b[6] >g_s+block_tim:

                if Duty1<12.5:
                    #Key_detect() ##按键检测
                    if b[6]-g_s-block_tim>45:
                        Duty1-=0.15*(b[6]-g_s-block_tim)/60
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    elif b[6]-g_s-block_tim>30 and b[6]-g_s-block_tim<=60:
                        Duty1-=0.075*(b[6]-g_s-block_tim)/50
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    elif b[6]-g_s-block_tim>15 and b[6]-g_s-block_tim<=30:
                        Duty1-=0.05*(b[6]-g_s-block_tim)/20
                        Duty1 = Pwm_limit(Duty1)
                        print(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    else:
                        Duty1-=0.02*(b[6]-g_s-block_tim)/9
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
            elif b[6]<g_s-block_tim:
                if Duty1>2.5:
                    #Key_detect() ##按键检测
                    if g_s-block_tim-b[6]>45:
                        Duty1+=0.15*(g_s-block_tim-b[6])/60
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    elif g_s-block_tim-b[6]>30 and g_s-block_tim-b[6]<=60:
                        Duty1+=0.075**(g_s-block_tim-b[6])/50
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    elif g_s-block_tim-b[6]>15 and g_s-block_tim-b[6]<=30:
                        Duty1+=0.05*(g_s-block_tim-b[6])/20
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
                    else:
                        Duty1+=0.02*(g_s-block_tim-b[6])/9
                        Duty1 = Pwm_limit(Duty1)
                        ch0.duty(Duty1)
                        flag_bee2=0
            else:
                flag_bee2=1

        #print(flag_bee2)
        #print(flag_bee1)

        if flag_bee1==1 and flag_bee2==1:
            bee.value(1)
            led.value(0)

            flag_bee2=0
            flag_bee1=0

            time.sleep_ms(100)

            bee.value(0)
            led.value(1)
    else:
        time.sleep_ms(200)




#-----------------------------------------------------------------------------------------------#

#追光

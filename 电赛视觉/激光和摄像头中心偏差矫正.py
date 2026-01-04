from maix import image, camera, display, app, time,uart, pinmap  # 导入必要的模块
import math  # 导入数学模块用于数学运算

cam = camera.Camera(160, 120) 
disp = display.Display()
green_threshold2=[[92,100,00,1,-11,5]]

def find_purple():
    blobs = img.find_blobs(green_threshold2)  
    if blobs:
        for blob in blobs:
            x_purple=blob[5]
            y_purple=blob[6]
            img.draw_cross(blob[5],blob[6],image.COLOR_GREEN,size=3)
            print("激光的中心坐标是：",x_purple,y_purple)
    disp.show(img)


while True:
    img = cam.read()  # 从摄像头获取一帧图像
    find_purple()
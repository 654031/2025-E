#levi_jia 激光色块找角点（带A4纸比例过滤）with 卡尔曼滤波
from maix import image, camera, display, touchscreen, uart, time
import struct
import time as systime
import math

# 初始化硬件（降低分辨率到480x360）
cam = camera.Camera(480, 360)
disp = display.Display()

# ROI裁切参数（相对于480x360分辨率）
ROI_X = 120      # 裁切区域左上角X坐标
ROI_Y = 90       # 裁切区域左上角Y坐标
ROI_WIDTH = 240  # 裁切区域宽度
ROI_HEIGHT = 180 # 裁切区域高度

# 简单的卡尔曼滤波器实现
class SimpleKalmanFilter:
    def __init__(self, process_noise=0.1, measurement_noise=0.1):
        self.process_noise = process_noise
        self.measurement_noise = measurement_noise
        self.estimate = None
        self.error_estimate = 1.0
        self.last_prediction = None
        
    def update(self, measurement):
        if self.estimate is None:
            self.estimate = measurement
            self.last_prediction = measurement
            return self.estimate
            
        # 预测
        error_prediction = self.error_estimate + self.process_noise
        
        # 更新
        kalman_gain = error_prediction / (error_prediction + self.measurement_noise)
        self.estimate = self.estimate + kalman_gain * (measurement - self.estimate)
        self.error_estimate = (1 - kalman_gain) * error_prediction
        self.last_prediction = self.estimate
        
        return self.estimate
    
    def predict(self):
        if self.last_prediction is None:
            return None
        # 简单预测：假设目标继续按照上次的速度移动
        return self.last_prediction

# 创建两个卡尔曼滤波器，分别用于x和y坐标
kf_x = SimpleKalmanFilter()
kf_y = SimpleKalmanFilter()

# 摄像头参数设置
cam.luma(35)            # 亮度 [0, 100]
cam.constrast(50)       # 对比度 [0, 100]
cam.saturation(10)      # 饱和度 [0, 100]
cam.exp_mode(0)         # 曝光模式

# 颜色阈值
thresholds = [
    [0, 35, -50, 17, -38, 48],    # 黑色
    [54, 85, 8, 46, -5, 0],     # 红色
    [76, 92, -17, -6, -1, 9]    # 绿色
]

# A4纸比例参数
A4_RATIO = math.sqrt(2)  # ≈1.414 (A4纸长宽比)
RATIO_TOLERANCE = 0.2     # 比例容忍度(±20%)

# 全局变量
count = 0
A = 1
sline1 = [None]
sline2 = [None]
sline3 = [None]
sline4 = [None]

# 帧率计算
frame_count = 0
last_time = systime.time()
fps = 0

def calculate_fps():
    """计算并显示帧率"""
    global frame_count, last_time, fps
    frame_count += 1
    current_time = systime.time()
    if current_time - last_time >= 1.0:
        fps = frame_count / (current_time - last_time)
        frame_count = 0
        last_time = current_time
    return fps

def draw_point(img, x, y, color, size=5):
    """绘制点标记"""
    img.draw_rect(int(x-size/2), int(y-size/2), size, size, color, thickness=-1)  # 填充矩形作为点

def is_a4_ratio(blob):
    """检查矩形是否符合A4纸比例(考虑旋转情况)"""
    w, h = blob[2], blob[3]
    
    # 计算长边和短边
    long_side = max(w, h)
    short_side = min(w, h)
    
    if short_side == 0:
        return False
    
    # 计算实际比例
    actual_ratio = long_side / short_side
    
    # 检查是否在容忍范围内
    return abs(actual_ratio - A4_RATIO) <= RATIO_TOLERANCE

# 用于存储上一帧的预测位置
last_predicted_x = None
last_predicted_y = None

while True:
    # 捕获图像（已降低分辨率）
    img = cam.read()
    
    # 裁切ROI区域
    img = img.crop(ROI_X, ROI_Y, ROI_WIDTH, ROI_HEIGHT)
    
    # 计算并显示帧率
    current_fps = calculate_fps()
    img.draw_string(5, 5, f"FPS: {current_fps:.1f}", 
                   image.Color.from_rgb(255, 255, 255), scale=0.8)
    
    target_detected = False
    
    if A == 1:
        # 检测黑色矩形（调整阈值以适应裁切后的画面）
        blob1s = img.find_blobs([thresholds[0]], 
                              area_threshold=300,  # 调整面积阈值
                              pixels_threshold=5,  # 调整像素阈值
                              merge=True)
        
        for blob1 in blob1s:
            # 计算矩形面积
            rect_area = blob1[2] * blob1[3]  # blob[2]是宽度，blob[3]是高度
            
            # 添加面积过滤条件（1500-9000像素）
            if not (1500 <= rect_area <= 9000):
                continue  # 跳过不符合面积要求的矩形
                
            # 检查是否符合A4纸比例
            if not is_a4_ratio(blob1):
                continue  # 跳过不符合比例的矩形
                
            # # 打印检测到的矩形信息（调试用）
            # print(f"检测到有效矩形 - 位置: ({blob1[0]}, {blob1[1]}), 尺寸: {blob1[2]}x{blob1[3]}, 面积: {rect_area} 像素")
            
            # 标记目标已检测到
            target_detected = True
            
            # 绘制黑色矩形外框
            img.draw_rect(blob1[0], blob1[1], blob1[2], blob1[3], 
                         image.COLOR_GREEN, thickness=1)
            
            # 计算中心点（相对于裁切后的ROI）
            cx1, cy1 = blob1[0] + blob1[2]/2, blob1[1] + blob1[3]/2
            # 打印中心点坐标（新增部分）
            print(f"中心点坐标: ({cx1:.1f}, {cy1:.1f})")
            # 使用卡尔曼滤波器更新和预测
            pred_x = kf_x.update(cx1)
            pred_y = kf_y.update(cy1)
            
            # 保存预测位置
            last_predicted_x = pred_x
            last_predicted_y = pred_y
            
            # 绘制实际中心点（红色点）
            draw_point(img, cx1, cy1, image.Color.from_rgb(255, 0, 0))
            
            # 绘制预测中心点（蓝色点）
            draw_point(img, pred_x, pred_y, image.Color.from_rgb(0, 0, 255))
    
    # 如果没有检测到目标，但之前有预测位置，则继续显示预测点
    if not target_detected and last_predicted_x is not None and last_predicted_y is not None:
        # 使用卡尔曼滤波器预测（不更新）
        pred_x = kf_x.predict()
        pred_y = kf_y.predict()
        
        if pred_x is not None and pred_y is not None:
            # 更新最后预测位置
            last_predicted_x = pred_x
            last_predicted_y = pred_y
            
            # 只绘制预测中心点（蓝色点）
            draw_point(img, pred_x, pred_y, image.Color.from_rgb(0, 0, 255))
    
    # 显示图像
    disp.show(img)
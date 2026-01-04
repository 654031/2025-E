from maix import image, camera, display, app, time, uart, pinmap  # MaixPy专用库
import math  # 数学运算
import numpy as np  # 数值计算
import cv2  # OpenCV库，用于图像处理

class QuadDetector:
    """
    四边形检测类 - 移植版 (适配MaixCam平台)
    功能：检测图像中的四边形，计算缩放四边形，标记顶点为五角星
    """
    def __init__(self, max_perimeter=99999, min_perimeter=100, scale=0.8, min_angle=30, line_seg_num=4):
        """
        初始化四边形检测器
        @param max_perimeter: 允许的最大周长 (像素)
        @param min_perimeter: 允许的最小周长 (像素)
        @param scale: 缩放比例 (0-1)
        @param min_angle: 允许的最小角度 (度)
        @param line_seg_num: 每条边的分段数量
        """
        # 参数设置
        self.max_perimeter = max_perimeter
        self.min_perimeter = min_perimeter
        self.scale = scale
        self.min_angle = min_angle
        self.line_seg_num = line_seg_num
        
        # 检测结果存储
        self.vertices = None       # 检测到的四边形顶点
        self.scale_vertices = None # 缩放后的四边形顶点
        self.intersection = None   # 对角线交点
        self.points_list = None    # 每条边上的等分点列表
        
        # 形态学操作核
        self.kernel = np.ones((3,3), np.uint8)
        
        # ROI区域设置 (可根据实际调整)
        self.roi_x1, self.roi_y1 = 0, 0    # 左上角坐标
        self.roi_x2, self.roi_y2 = 320, 240  # 右下角坐标

    def preprocess_image(self, img):
        """
        图像预处理
        @param img: 输入图像 (MaixPy图像对象)
        @return: 预处理后的边缘图像 (OpenCV格式)
        """
        # 转换为OpenCV格式并提取ROI
        img_cv = image.image2cv(img)
        roi = img_cv[self.roi_y1:self.roi_y2, self.roi_x1:self.roi_x2]
        
        # 灰度转换
        gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        # 高斯模糊 (MaixCam性能有限，使用小核)
        blur = cv2.GaussianBlur(gray, (3,3), 0)
        # Canny边缘检测 (调整阈值适应MaixCam)
        edges = cv2.Canny(blur, 100, 200)
        
        return edges

    def find_max_quad_vertices(self, edges):
        """
        在边缘图像中寻找满足条件的最大四边形轮廓
        @param edges: 经过Canny边缘检测后的二值图像
        @return: 返回满足条件的最大四边形顶点坐标(相对于原图坐标系)，未找到则返回None
        """
        
        # 查找图像中的所有轮廓
        contours, _ = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        
        max_perimeter = 0     
        best_quad = None       

        for cnt in contours:
            epsilon = 0.05 * cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, epsilon, True)
            
            if len(approx) == 4:
                perimeter = cv2.arcLength(approx, True)
                if not (self.min_perimeter <= perimeter <= self.max_perimeter):
                    continue
                    
                angles = []
                valid_angles = True
                
                for i in range(4):
                    p0 = approx[i][0]
                    p1 = approx[(i+1)%4][0]
                    p2 = approx[(i+2)%4][0]
                    
                    vec1 = p0 - p1
                    vec2 = p2 - p1
                    
                    norm1 = np.linalg.norm(vec1)
                    norm2 = np.linalg.norm(vec2)
                    
                    if norm1 == 0 or norm2 == 0:
                        valid_angles = False
                        break
                        
                    cos_theta = np.dot(vec1, vec2) / (norm1 * norm2)
                    cos_theta = np.clip(cos_theta, -1.0, 1.0)
                    angle = math.degrees(math.acos(cos_theta))
                    angles.append(angle)
                
                if (valid_angles and 
                    all(a >= self.min_angle for a in angles) and 
                    perimeter > max_perimeter):
                    
                    max_perimeter = perimeter
                    best_quad = [
                        [p[0][0] + self.roi_x1,  
                        p[0][1] + self.roi_y1]   
                        for p in approx
                    ]
    
        return np.array(best_quad) if best_quad is not None else None

    def calculate_scale_quad(self, vertices):
        """
        计算缩放四边形
        @param vertices: 原始四边形顶点
        @return: 缩放后的四边形顶点
        """
        if vertices is None:
            return None
            
        center = np.mean(vertices, axis=0)
        scale_vertices = []
        for point in vertices:
            scaled_x = center[0] + (point[0] - center[0]) * self.scale
            scaled_y = center[1] + (point[1] - center[1]) * self.scale
            scale_vertices.append([scaled_x, scaled_y])
            
        return np.array(scale_vertices, dtype=np.int32)

    def calculate_intersection(self, vertices):
        """
        计算对角线交点
        @param vertices: 四边形顶点
        @return: 交点坐标 (x,y) 或 None
        """
        if vertices is None or len(vertices) != 4:
            return None
            
        p0, p1, p2, p3 = vertices
        
        a1 = p2[1] - p0[1]
        b1 = p0[0] - p2[0]
        c1 = a1*p0[0] + b1*p0[1]
        
        a2 = p3[1] - p1[1]
        b2 = p1[0] - p3[0]
        c2 = a2*p1[0] + b2*p1[1]
        
        det = a1*b2 - a2*b1
        
        if det == 0:  
            return None
            
        x = (b2*c1 - b1*c2) / det
        y = (a1*c2 - a2*c1) / det
        
        return (int(x), int(y))

    def segment_quad_edges(self, vertices):
        """
        等分四边形边
        @param vertices: 四边形顶点
        @return: 每条边的等分点列表
        """
        if vertices is None:
            return None
            
        def interpolate_points(p1, p2, n):
            return [ [
                int(p1[0] + (p2[0]-p1[0])*i/n),
                int(p1[1] + (p2[1]-p1[1])*i/n)
            ] for i in range(n+1) ]
        
        points_list = [
            interpolate_points(vertices[0], vertices[1], self.line_seg_num),
            interpolate_points(vertices[1], vertices[2], self.line_seg_num),
            interpolate_points(vertices[2], vertices[3], self.line_seg_num),
            interpolate_points(vertices[3], vertices[0], self.line_seg_num)
        ]
        
        return points_list

    def detect(self, img):
        """
        主检测函数
        @param img: 输入图像 (MaixPy图像对象)
        @return: 检测结果元组 (vertices, scale_vertices, intersection, points_list)
        """
        edges = self.preprocess_image(img)
        self.vertices = self.find_max_quad_vertices(edges)
        self.scale_vertices = self.calculate_scale_quad(self.vertices)
        self.intersection = self.calculate_intersection(self.vertices)
        self.points_list = self.segment_quad_edges(self.vertices)
        
        return (self.vertices, self.scale_vertices, self.intersection, self.points_list)

    def draw_results(self, img):
        """
        绘制检测结果（用五角星标记顶点）
        @param img: 原始图像 (MaixPy图像对象)
        @return: 绘制后的图像 (MaixPy图像对象)
        """
        img_cv = image.image2cv(img)
        
        # 绘制ROI区域
        cv2.rectangle(img_cv, (self.roi_x1, self.roi_y1), (self.roi_x2, self.roi_y2), (0,0,255), 2)
        
        if self.vertices is not None:
            # 绘制原始四边形顶点（红色五角星）
            for point in self.vertices:
                cv2.drawMarker(img_cv, tuple(point.astype(int)), 
                               color=(0, 0, 255),    # 红色
                               markerType=cv2.MARKER_STAR,
                               markerSize=8,
                               thickness=2)
            
            # 绘制缩放四边形顶点（黄色五角星）
            if self.scale_vertices is not None:
                for point in self.scale_vertices:
                    cv2.drawMarker(img_cv, tuple(point.astype(int)), 
                                   color=(0, 255, 255),  # 黄色
                                   markerType=cv2.MARKER_STAR,
                                   markerSize=8,
                                   thickness=2)
            
            # 绘制原始四边形 (绿色边框)
            cv2.polylines(img_cv, [self.vertices.astype(int)], True, (0,255,0), 2)
            
            # 绘制缩放四边形 (蓝色边框)
            if self.scale_vertices is not None:
                cv2.polylines(img_cv, [self.scale_vertices.astype(int)], True, (255,0,0), 2)
            
            # 绘制对角线交点 (紫色圆圈)
            if self.intersection is not None:
                cv2.circle(img_cv, self.intersection, 5, (255,0,255), -1)
            
            # # 绘制等分点 (青色圆点)
            # if self.points_list is not None:
            #     for edge_points in self.points_list:
            #         for point in edge_points:
            #             cv2.circle(img_cv, tuple(point), 3, (255,255,0), -1)
        
        return image.cv2image(img_cv)

# 主程序
# 主程序
# 主程序
if __name__ == "__main__":
    cam = camera.Camera(320, 240)
    disp = display.Display()
    
    detector = QuadDetector(
        max_perimeter=1000,
        min_perimeter=200,
        scale=0.9,  # 缩放比例，会影响缩放四边形的大小
        min_angle=30,
        line_seg_num=4
    )
    
    while not app.need_exit():
        t_start = time.ticks_ms()
        
        img = cam.read()
        detector.detect(img)
        result_img = detector.draw_results(img)
        
        # 获取并打印原始四边形的四个顶点坐标
        vertices = detector.vertices
        if vertices is not None:
            print("原始四边形四个顶点的坐标：")
            # 计算四个顶点的中心
            center_x = sum(x for x, y in vertices) / 4
            center_y = sum(y for x, y in vertices) / 4
            
            # 对四个顶点进行分类
            top_left = None
            top_right = None
            bottom_right = None
            bottom_left = None
            
            for x, y in vertices:
                if x < center_x and y < center_y:
                    top_left = (x, y)
                elif x >= center_x and y < center_y:
                    top_right = (x, y)
                elif x >= center_x and y >= center_y:
                    bottom_right = (x, y)
                elif x < center_x and y >= center_y:
                    bottom_left = (x, y)
            
            # 打印带有位置描述的顶点坐标
            if top_left:
                print(f"左上顶点: x = {top_left[0]}, y = {top_left[1]}")
            if top_right:
                print(f"右上顶点: x = {top_right[0]}, y = {top_right[1]}")
            if bottom_right:
                print(f"右下顶点: x = {bottom_right[0]}, y = {bottom_right[1]}")
            if bottom_left:
                print(f"左下顶点: x = {bottom_left[0]}, y = {bottom_left[1]}")
        
        # 获取并打印缩放四边形的四个顶点坐标
        scale_vertices = detector.scale_vertices
        mid_points = {}  # 存储中点坐标
        
        if scale_vertices is not None:
            print("\n缩放四边形四个顶点的坐标：")
            # 计算缩放后四个顶点的中心
            center_x = sum(x for x, y in scale_vertices) / 4
            center_y = sum(y for x, y in scale_vertices) / 4
            
            # 对缩放后的四个顶点进行分类
            top_left_scale = None
            top_right_scale = None
            bottom_right_scale = None
            bottom_left_scale = None
            
            for x, y in scale_vertices:
                if x < center_x and y < center_y:
                    top_left_scale = (x, y)
                elif x >= center_x and y < center_y:
                    top_right_scale = (x, y)
                elif x >= center_x and y >= center_y:
                    bottom_right_scale = (x, y)
                elif x < center_x and y >= center_y:
                    bottom_left_scale = (x, y)
            
            # 打印带有位置描述的缩放后顶点坐标
            if top_left_scale:
                print(f"左上顶点: x = {top_left_scale[0]}, y = {top_left_scale[1]}")
            if top_right_scale:
                print(f"右上顶点: x = {top_right_scale[0]}, y = {top_right_scale[1]}")
            if bottom_right_scale:
                print(f"右下顶点: x = {bottom_right_scale[0]}, y = {bottom_right_scale[1]}")
            if bottom_left_scale:
                print(f"左下顶点: x = {bottom_left_scale[0]}, y = {bottom_left_scale[1]}")
        
        # 确保两个四边形的顶点都已成功分类
        if (vertices is not None and scale_vertices is not None and
            top_left and top_left_scale and
            top_right and top_right_scale and
            bottom_right and bottom_right_scale and
            bottom_left and bottom_left_scale):
            
            # 计算对应顶点的中点坐标
            mid_points = {
                'top_left': ((top_left[0] + top_left_scale[0]) // 2, (top_left[1] + top_left_scale[1]) // 2),
                'top_right': ((top_right[0] + top_right_scale[0]) // 2, (top_right[1] + top_right_scale[1]) // 2),
                'bottom_right': ((bottom_right[0] + bottom_right_scale[0]) // 2, (bottom_right[1] + bottom_right_scale[1]) // 2),
                'bottom_left': ((bottom_left[0] + bottom_left_scale[0]) // 2, (bottom_left[1] + bottom_left_scale[1]) // 2)
            }
            
            # 打印中点坐标
            print("\n两个四边形对应顶点的中点坐标：")
            for position, (x, y) in mid_points.items():
                print(f"{position}中点: x = {x}, y = {y}")
        
        disp.show(result_img)
        
        fps = 1000 / (time.ticks_ms() - t_start)
        print(f"FPS: {fps:.1f}")







# 2025-E
这是北京化工大学团队2025年电赛项目，除张大头步进电机控制板外，完全按项目要求使用MSPM0系列芯片完成。使用的是逐飞科技开源库（使用过程中发现开源库有部分问题，已修正，会在项目中提及）

问题1：外部中断回调异常，无法通过外部中断获取编码器数据
原因：经测试，发现外部中断回调函数中未清除标志位，本项目中已修正

问题2：串口异常，初始化后时序问题
解决手段：初始化后添加延时（工程已添加）

问题3：IIC异常，陀螺仪IIC模式下CS引脚异常，不可接
解决手段：逐飞新版本已修正

逐飞库链接:
https://gitee.com/seekfree/MSPM0G3507_Library

项目简介：
以 MSPM0 系列 MCU 作为控制核心，结合步进电机与 Jetson 视觉模块，设计制作了一套高效稳定运行
的寻迹及自动瞄准机器人。（四天三夜完成）
移动底盘：采用亚博智能编码器电机，八路模拟灰度模块，IMU660RA 陀螺仪:TB6612 电机驱动。 
视觉模块：采用 NVIDIA JETSON ORIN NX 边缘 AI 视觉模块，高达 157TOPS 算力，为高帧率图像处理提供算力支撑。使用 HIKVISION MV-CS016 相机 4MMF1.8 镜头，全局快门保证在运动中无畸变;相机最大帧率高达 265FPS, 拥有极强的动态追踪性能。 
（比赛时由于成本限制，采用的maxicam摄像头，帧率只有30FPS，后期无法更换视觉模块，控制端采用前馈，项目目标由满分完成，转变为在当时的硬件限制下，尽可能的提高精度。
最终实现1.5m外二环<=4cm移动打靶，但比赛时未能达到这一指标，不过由于今年该题目难度较高，尤其是限制工程必须在算力资源，外设资源极其有限的MO芯片上完成，更是加大了难度。因而最终仍旧取得了一个相对不错的成绩）
（后续打算借鉴TI杯小组视觉方案，进行项目重构）
瞄准装置：微控制器上实现瞄准装置整体姿态解算，利用电机编码器反馈值及 IMU 传感器输出的角度、角速度信息，结合视觉回传数据，对二维云台进行控制。保证在寻迹时，使用云台控制激光笔完成打靶任务。

This is the 2025 Electronic Design Competition project by the team from Beijing University of Chemical Technology. Except for the Zhang Datou stepper motor control board, the project was fully implemented using the MSPM0 series microcontrollers in strict compliance with the competition requirements. The ZhuFei Technology open-source library was utilized during development (issues identified in the library were addressed and will be detailed in the project documentation).

Issue 1: External Interrupt Callback Exception - Unable to Read Encoder Data via External Interrupt
Cause: Testing revealed that the interrupt flag was not cleared within the external interrupt callback function. This has been corrected in this project.

Issue 2: Serial Port Exception - Timing Issue After Initialization
Solution: A delay was added after initialization (already implemented in the project).

Issue 3: I2C Exception - Abnormal CS Pin Behavior in Gyroscope I2C Mode, Cannot Be Connected
Solution: This issue has been fixed in the newer version of the ZhuFei library.

ZhuFei Library Link:
https://gitee.com/seekfree/MSPM0G3507_Library

Project Overview:
Using the MSPM0 series MCU as the control core, combined with stepper motors and a Jetson vision module, we designed and built an efficient and stable line-following and auto-aiming robot (completed over four days and three nights).

Mobile Chassis: Utilizes Yabo Smart encoder motors, an eight-channel analog grayscale sensor module, an IMU660RA gyroscope, and TB6612 motor drivers.
Vision Module: Employs the NVIDIA Jetson Orin NX edge AI vision module, providing up to 157 TOPS of computing power to support high-frame-rate image processing. It uses a Hikvision MV-CS016-10GM camera with a 4mm F1.8 lens; the global shutter ensures no distortion during motion. The camera supports a maximum frame rate of 265 FPS, offering excellent dynamic tracking performance.
(During the competition, due to cost constraints, we used a MaxiCam camera with only 30 FPS. The vision module could not be replaced later. The control system adopted a feedforward approach, and the project goal shifted from achieving a perfect score to maximizing accuracy under the given hardware limitations. Ultimately, we achieved a moving target accuracy of ≤4cm within the second ring at a distance of 1.5 meters. However, this target was not fully met during the competition. Due to the high difficulty of this year's task, especially the requirement to complete the entire project on the resource-limited MSPM0 chip, the challenge was significant. Nonetheless, we still achieved a relatively good result.)
(We plan to reference the visual solution from the TI Cup team for a project refactoring in the future.)
Aiming Device: The overall attitude calculation for the aiming mechanism is implemented on the microcontroller. By utilizing motor encoder feedback, angle and angular velocity data from the IMU sensor, and visual feedback data, we control a two-axis gimbal. This ensures that during line-following, the gimbal can control a laser pointer to complete the target-hitting task.

# Infineon EZ-USB CX3 搭配 FX3 SDK 驱动 OV5640 图像传感器

## 〇. 环境配置

评估板: DENEBOLA (See3CAM_CX3_RDK_H01R2) - Infineon EZ-USB™ CX3 Reference Design
SDK: Version 1.3.5
Windows 版本: Windows11 23H2

## 一. 创建 CX3 工程

参考 Infineon 知识库文章:https://community.infineon.com/t5/知识库文章/基于CX3的UVC摄像头应用学习笔记-二-建立工程模板/ta-p/246095

## 二. 添加 OV5640 驱动

根据 OV5640 寄存器手册与 MIPI 应用指南添加寄存器配置(下载地址: https://www.corecourse.cn/forum.php?mod=viewthread&tid=29331). 本文章文末的 Github 链接中的 cy_ov5640.c/.h 文件可供读者参考.

## 三. 调用驱动文件中的函数

修改 cycx3_uvc.c 与 cycx3_videostreaming.c 文件中以下函数中调用的函数为驱动文件中定义的函数:

|函数名|原始文件中的函数|修改为|
|-----|---------------|------|
|**CyCx3AppStart(void)**|_CyCx3ImageSensorWakeup()_|_CyCx3_ImageSensor_Wakeup()_|
||_CyCx3ImageSensorTriggerAutofocus()_|_CyCx3_ImageSensor_Trigger_Autofocus()_|
|**CyCx3AppStop(void)**|_CyCx3ImageSensorSleep()_|_CyCx3_ImageSensor_Sleep()_|
|**CyCx3AppInit(void)**|_CyCx3ImageSensorInit()_|_CyCx3_ImageSensor_Init()_|
||_CyCx3ImageSensorSleep()_|_CyCx3_ImageSensor_Sleep()_|
|**CyCx3AppThread_Entry(uint32_t input)**|_CyCx3ImageSensorSleep()_|_CyCx3_ImageSensor_Sleep()_|
||_CyCx3ImageSensorWakeup()_|_CyCx3_ImageSensor_Wakeup()_|
|**CyCx3UvcAppImageSensorSetVideoResolution(uint8_t formatIndex,uint8_t resolutionIndex)**|_CyCx3_Set_OV5640_YUY2_ResolutionX()_|驱动文件中对应的分辨率设置函数|

## 四. 修正 MIPI 时延

注释掉 cycx3_videostreaming.c 中所有 CyU3PMipicsiSetPhyTimeDelay()

## 五. 编译烧录运行

右键工程, 点击 Build Project. 编译完成后将自动生成.img 文件. 将评估板 Boot Mode 切换到 USB, 打开 Control Center, 选择 Program-FX3-RAM, 选择到生成的.img 文件进行烧录. 烧录完成后计算机将成功识别评估板为 UVC 摄像头. 此时打开 PotPlayer, 右键, 选择打开-摄像头/其他设备, 应可以正常看到画面. 选择设备设置-摄像头, 可以切换格式.

## 六. 如何了解设备 USB 属性

下载 UsbTreeView: https://storage.googleapis.com/files.pixelink.com/uploads/Customer%20Files/UsbTreeView.exe

设备连接时打开设备树, 右侧屏幕可以看到设备描述符. 其中 Connection Information-Device Bus Speed 描述了设备的速率. 除此之外, 也可查看设备的其他属性, 如支持的视频帧格式等.

## 七. 示例代码

https://github.com/WangHaoZhe/Cx3UvcOV5640_Modified

You are now viewing in [Chinese Simplified](DOCS/CN.md) !

#  原神自动地图（天理地图） TianLi_Map [![Build status](https://ci.appveyor.com/api/projects/status/8kmemyn4jssya2xs?svg=true)](https://ci.appveyor.com/project/GengGode/genshinimpact-automap)
 
 [![](https://img.shields.io/github/downloads/GengGode/GenshinImpact_AutoMap/total)](https://github.com/GengGode/GenshinImpact_AutoMap/releases) ![](https://img.shields.io/github/v/release/GengGode/GenshinImpact_AutoMap?include_prereleases) ![](https://img.shields.io/github/languages/top/GengGode/GenshinImpact_AutoMap)

* 自动发现小地图上出现的神瞳并记录

* 自动记忆神瞳是否收集，判断显示哪些神瞳没被发现

* 神瞳已收集的话，神瞳就会变成半透明。

* `Alt+T` 或者地图任意位置左键双击开启关闭自动追踪，如果显示神瞳的话就会自动开始识别记录。

* 开启自动追踪，右下角图标会变成蓝色，根据电脑性能，3至60秒即可加载完成，悬浮窗会自动显示角色所在位置的地图

<Details>
<summary>分流下载</summary>
    
### 百度云
 
### `2.6.2 版`
 
#### 添加了2.7版本地图，以及宝箱资源 
 
[下载链接](http://8.134.219.60:5244/d/WeiXiTianLi/download/GenshinImpactAutoMap/last/GenshinImpact_AutoMap.exe)   
`2.6.2版本`
</Details>

# 更新记录
    
    2022.06.09.09:03 添加更新记录
    2021.10.03.21:52 添加了免责声明
    2021.10.03.21:45 添加了对【腾讯云游戏】的支持
    ---- 之前的忘了

# 使用方法 / Shortcuts

* `Alt + T` 开启关闭自动追踪

* `Alt + F` 添加删除自定义标记

* `Alt + Enter` 切换为无边框模式

* `Alt + M`  截图

* `Alt + F8` 选择原神游戏窗口

* `左键拖动`地图

* `滚轮` 缩放地图显示

* `中键`移动窗口位置

* `左键双击`左上角派蒙退出

* `左键单击`右下角Home图标显示更多物品

## 视频教程

### Bilibili

* [安装](https://www.bilibili.com/video/BV1Wy4y1x754)

* [使用](https://www.bilibili.com/video/BV1ar4y1A7c5)  

## 交流

#### [QQ](538198823):538198823
----

# 无法使用？

1. 项目仅在有限的条件下测试过，如需排查错误，强烈建议按照以下描述进行环境配置。
   * 原神客户端 > 右上角派蒙 > 设置 > 抗锯齿，设置为 SMAA
   * 系统 > 显示设置 > 文字缩放，设置为100%

2. 请查看：[视频教程](#视频教程)
3. 提交[Issues](/issues)
3. 无论有没有问题，你都可以加群[♂](#交流)

# 已知问题

* 手柄适配 无
 
  ## 已经实现

  * [x] 通过游戏内小地图，自动识别角色所在位置以及角色朝向

  * [x] 添加了自动识别UID

  * [x] 检测游戏状态

  * [x] 添加标记

  * [x] 缩放改为以指针为中心

  * [x] 根据数据库显示周围资源分布

  * [x] 保存存档  
 
  * [x] 保存日志，包含可追溯数据，如存档损坏，可以此追溯

  * [x] 除1920 * 1080外分辨率的 uid 识别

  * [x] 除 16 ：9 分辨率外的自动追踪

  ## 计划实现

  * [ ] 添加更多点位

  * [ ] 优化UI（慢慢来）

  * [ ] 优化匹配

  * [ ] 添加图论部分实现导航收集某种资源的最短路径
 
  * [ ] 添加惯性导航部分
 
# Devs

[DLL动态链接库](https://github.com/GengGode/GenshinImpact_AutoTrack_DLL)
<details>
  <summary>此项目的开发</summary>
 解压resource.rar在项目目录下，释放资源文件，否则编译不过。
 编译 MSBuild 即可，需要VS安装Qt VS Tools插件。
</details>

  ## 开发环境

  * Visual Studio 2017 

  * Qt5.14.2

  * opencv4.5.0

  ## 工作原理

  本地图是通过调用Windows Api来对原神的游戏窗口进行截屏，然后对画面进行图像处理，对地图与世界地图进行匹配来实现的自动追踪。

  通过Api来设置地图为顶层窗口，使其保持在原神游戏窗口的上方。

  目前所有涉及对原神窗口的操作如下：

  ``` C++
   giHandle = FindWindowA(NULL, "原神"); //获取原神窗口句柄

   giIsDisplayFlag = !IsIconic(giHandle); //判断原神是否处于最小化

   GetWindowRect(giHandle, &giRect); //获取原神窗口区域

   HDC hScreen = GetDC(giHandle); //获取原神窗口画面，作用是截屏获取原神画面

   SetForegroundWindow(giHandle); //激活原神窗口为焦点

   if (giHandle != NULL) //判断原神窗口句柄是否为空
    
   ```
  
  ## 系统权限
 
  涉及到数据文件的保存与读取，目前会在路径为：`C:\Users\GengG\AppData\LocalLow\天理系统\天理地图`

  日后可能会开发GPU加速图像处理的功能，需要使用GPU运算。

  除此之外不存在其他系统资源的使用，也没有获取系统权限的需求。



----

# 免责声明

天理地图不提供任何形式的保证。所有与使用本软件的直接风险均由用户承担。

如果您发现本软件包含错误或者有更好的建议，请联系我以便这些错误和建议得到及时的应用。

联系方式：ysx20201029@163.com 
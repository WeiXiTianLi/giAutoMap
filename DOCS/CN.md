You are now viewing in [Chinese Simplified](DOCS/CN.md) !
#  原神自动地图 TianLi_Map [![Build status](https://ci.appveyor.com/api/projects/status/8kmemyn4jssya2xs?svg=true)](https://ci.appveyor.com/project/GengGode/genshinimpact-automap)
 [![](https://img.shields.io/github/downloads/GengGode/GenshinImpact_AutoMap/total)](https://github.com/GengGode/GenshinImpact_AutoMap/releases) ![](https://img.shields.io/github/v/release/GengGode/GenshinImpact_AutoMap?include_prereleases) ![](https://img.shields.io/github/languages/top/GengGode/GenshinImpact_AutoMap)
* 自动发现小地图上出现的神瞳并记录

* 自动记忆神瞳是否收集，判断显示哪些神瞳没被发现

* 神瞳已收集的话，神瞳就会变成半透明。

* `Alt+T` 或者地图任意位置左键双击开启关闭自动追踪，如果显示神瞳的话就会自动开始识别记录。

* 开启自动追踪，右下角图标会变成蓝色，根据电脑性能，3至60秒即可加载完成，悬浮窗会自动显示角色所在位置的地图
    <details>
     <summary> 效果演示 </summary>
  
    ## 新分支

    ![](https://github.com/GengGode/GenshinImpact_AutoMap/blob/master/Image/Show1.png?raw=true)

    ## 主分支

    ![](https://github.com/GengGode/GenshinImpact_AutoMap/blob/master/Image/Show2.png?raw=true)

    </details>

# 使用方法 / Shortcuts

* `Alt + T` 开启关闭自动追踪

* `Alt + F` 添加删除自定义标记

* `Alt + Enter` 切换为无边框模式

* `Alt + M` 激活「空荧酒馆」原神地图客户端的大地图覆盖模式

* `Alt + F8` 选择原神游戏窗口

* `左键拖动`地图

* `滚轮` 缩放地图显示

* `中键`移动窗口位置

* `左键双击`左上角派蒙退出
# 无法使用？
1. 项目仅在有限的条件下测试过，如需排查错误，强烈建议按照以下描述进行环境配置。
   * 原神客户端 > 右上角派蒙 > 设置 > 抗锯齿，设置为 SMAA
   * 原神客户端 > 右上角派蒙 > 设置 > 分辨率，设置为 1920x1080  
2. 请查看：[视频教程](#视频教程)
3. 提交[Issues](/issues)
3. 无论有没有问题，你都可以加群[♂](#交流)
# Devs
[DLL动态链接库](https://github.com/GengGode/GenshinImpact_AutoTrack_DLL)
<details>
  <summary>此项目的开发</summary>
  
  ## 已经实现

  * [x] 通过游戏内小地图，自动识别角色所在位置以及角色朝向

  * [x] 添加了自动识别UID

  * [x] 检测游戏状态

  * [x] 添加标记

  * [x] 缩放改为以指针为中心

  * [x] 根据数据库显示周围资源分布

  * [x] 保存存档  

  ## 计划实现

  * [ ] 添加更多点位

  * [ ] 优化UI（慢慢来）

  * [ ] 优化匹配

  * [ ] 添加图论部分实现导航收集某种资源的最短路径
  ## 开发环境

  * Visual Studio 2017 

  * Qt5.12

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
  之后的开发中将会涉及到数据文件的保存与读取，目前会在Temp（目前版本应该会改到My Game目录下，Temp下容易被删除）目录中建立名为GenshinImpactAutoMap的文件夹，并在该文件夹中建立数据存档。

  （说不定会改到AppData下的LocalLow里）

  如：`C:\Users\%USERNAME%\AppData\Local\Temp\GenshinImpactAutoMap`

  日后可能会开发GPU加速图像处理的功能，需要使用GPU运算。

  目前为了与“「空荧酒馆」原神地图”客户端通信还使用了本机的23333端口用来发送原神角色的位置、朝向以及UID。

  还有读写文件存档。

  除此之外不存在其他系统资源的使用，也没有获取系统权限的需求。

</details>

----
# 已知问题

* 自动追踪在城镇中表现得非常鬼畜，来回乱闪。

    * 这是由于图像匹配完成后，得到了匹配的特征点对。通过一个点对，以及它们各自在图像中的位置，可以计算出小地图对应在大地图的位置。
但是由于原神内小地图与匹配用地图的大小不一样，其中有个缩放系数，
角色的当前位置需要通过一个映射才能正确获得。
通常原神小地图的显示是均匀的，所以一个固定的缩放系数没有问题，
但是在城镇内部，原神小地图放大了，所以使用的缩放因子便不适合了。
以至于角色位置识别误差非常大。
目前的解决方案是判断当误差变大时尝试更新缩放因子来识别。（还没做）

## 视频教程
### Bilibili
* [安装](https://www.bilibili.com/video/BV1Wy4y1x754)

* [使用](https://www.bilibili.com/video/BV1ar4y1A7c5)  
## 交流
#### [QQ](538198823):538198823
----
<Details>
<summary>分流下载</summary>
    
### 百度云
### `v1.070` 
 #### 添加了部分新资源，提高了追踪的稳定性
[下载链接](https://pan.baidu.com/s/1J0VcqqFcUQ3d59skq6m0WQ)
提取码：esh8 

### `v2.011 预览版`
#### 添加了雷神瞳+稻妻地图，更新了启动器 
[下载链接](https://pan.baidu.com/s/1ZWTO_az6ONBqL7UbTXFQ6Q) 
提取码：88yg  
`链接版本只经过简单测试，如有任何问题可以提交反馈。`
</Details>
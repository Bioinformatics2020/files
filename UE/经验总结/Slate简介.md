# Slate框架做了哪些

1. 平台层：解决各个硬件或配置之间的差异，例如：

   用户输入：不论用户是从Windows还是Mac，不论是键盘输入还是触摸输入，全部通过FSlateApplication的输入相关函数处理。

   渲染输出：不论用户是Android还是平板，不论平台DirectX还是OpenGL，







为什么需要UMG：slate由纯c++实现，提供用户输入事件、焦点事件、控件布局、裁剪变换、渲染显示等。所有UI相关的工作均可以由slate完成、并且保持了c++高效、易于直接调用底层渲染方法等优点。但是slate继承c++优点的同时也继承了c++的缺点：代码逻辑复杂且无法实时修改调试与预览，对设计人员不友好等问题。ue通过使用UWidget，继承了UObject的反射特性，通过蓝图实现了简单好用的页面设计功能。需要注意的是UMG出现的核心目的是便于在蓝图中设计UI。



通过什么方法调用GPU的绘制接口：

1. 我们所有UI都是通过FSlateDrawElement来间接调用GPU的，并且FSlateDrawElement用法相对单纯。

2. FSlateDrawElement暴露的接口有：MakeBox(绘制盒体)、MakeText(绘制文本)、MakeSpline(绘制曲线)、MakeCustom(绘制自定义形状)等等。以MakeBox为例，看看传入参数

   ```c++
   /**
    * 根据下图创建 box 元素。 允许调整box大小的同时保持图像边框宽度
    * 如果没有边框，则生成一个简单的四边形
    *     ___LeftMargin    ___RightMargin
    *    /                /
    *  +--+-------------+--+
    *  |  |c1           |c2| ___TopMargin
    *  +--o-------------o--+
    *  |  |             |  |
    *  |  |c3           |c4|
    *  +--o-------------o--+
    *  |  |             |  | ___BottomMargin
    *  +--+-------------+--+
    *
    * @param ElementList           待渲染元素的列表
    * @param InLayer               要在其上绘制元素的图层(类似画布面板的zOrder)
    * @param PaintGeometry         渲染区域位置和尺寸;请参阅 FPaintGeometry
    *
    * @param InBrush               笔刷，Box的属性，包括大小、边距、着色等
    * @param InDrawEffects         Optional draw effects to apply
    * @param InTint                着色，可以理解为滤镜
    */
   SLATECORE_API static void MakeBox( 
      FSlateWindowElementList& ElementList,
      uint32 InLayer,
      const FPaintGeometry& PaintGeometry,
      const FSlateBrush* InBrush,
      ESlateDrawEffect InDrawEffects = ESlateDrawEffect::None,
      const FLinearColor& InTint = FLinearColor::White );
   ```

   前3个参数为所有接口都有的参数，后3个参数为绘制box特有的属性

3. 通过以上API可以看出，调用GPU的接口非常的简单粗暴，我们需要将复杂的页面整理成最简单的形式传递给GPU。







为什么需要slot：ue设计页面时设计为  父节点--父节点与子节点的连接方法(slot)--子节点。这种设计的最大好处在于子节点与父节点之间没有联系，所有布局上的问题全部由slot解决。一个父节点根据类型不同，可以有不同数量的slot，每一个slot必然需要绑定一个子项



slate里面怎么完成页面布局的定义：slate使用链式编程风格，通过c++的运算符重载，使得各个代码段的含义非常清晰。

1. 使用SNew或SAssignNew函数创建控件
2. 通过"."运算符后方的函数设置控件的属性
3. 通过"+"运算符后方的函数向控件中添加slot
4. 通过"."运算符后方的函数设置slot的属性
5. 通过"[]"运算符内部的代码段向slot中添加子项
6. 回到第一条

以上编码风格显然与一般的构造函数差异较大，那么这一系列的函数又是怎么完成对slate控件的初始化呢



## Slate介绍

### 简介

核心特点：纯C++实现、高性能、跨平台、易扩展、控件库丰富、易调试。

主要功能：控件布局、裁剪变换、焦点控制与输入响应、多种控件库、主题与样式、UI动画、多语言、高性能渲染、性能分析等。

与UMG如何分工：

​	当UE封装的Slate控件不满足需求且需求比较复杂时再考虑使用Slate实现，Slate只应该作为底层框架发挥作用。
​	作为独立应用模块或UI需求简单时使用纯Slate实现基本的UI也许更合理。

### Slate基础控件Demo

- 演示独立应用程序创建
- 演示普通曲线绘制
- 演示自定义图形绘制
- 演示贝塞尔曲线绘制
- 演示布局控件 -- 水平框嵌套自定义图形

### Slate输入响应功能

以Windows的鼠标点击事件为例，处理流程如下：

1. 按照操作系统的规范获取系统消息，并执行绑定的函数

   windows系统规范参考[使用消息和消息队列 - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/using-messages-and-message-queues)

   UE4实际处理流程参考与`FWindowsApplication::RegisterClass`、`FWindowsApplication::PumpMessages`

2. 通过FWindowsApplication的消息处理，将系统数字编码信号转化为FSlateApplication对应的事件并执行。

   FSlateApplication对应事件例如：键盘事件、鼠标事件、触摸事件(要求设备支持)、窗口事件(激活、关闭、移动等)、拖拽事件(文件拖入等)、控制器事件(手柄)。

   具体参考FGenericApplicationMessageHandler接口与FSlateApplication对应的实现。

3. 开始响应鼠标输入事件，封装鼠标相关的信息给到正式处理鼠标输入的函数。

   若开启鼠标模拟触摸功能且按下按钮为左键，则直接转发消息到OnTouchStarted函数，否则继续向下执行。

   将输入事件封装到FPointerEvent结构体，其中包含用户索引、鼠标坐标、事件对应的FKey等鼠标事件相关信息。

4. 正式处理鼠标输入

   完成Window窗口聚焦

   若处于拖拽中则忽略本次点击事件

   查看当前鼠标是否被捕获于某一个控件(参考`FReply::Handled().CaptureMouse(AsShared())`、焦点控制系统)

5. 若鼠标被控件捕获

   获取到被捕获控件的路径

   通过FEventRouter::Route<FReply>(FEventRouter::FToLeafmostPolicy())函数，依据捕获控件路径找到子节点控件，向控件传递鼠标点击事件，处理逻辑如下

   1. 第一步执行传入的lambda表达式，这里的lambda仅执行SWidget中的`OnPreviewMouseButtonDown`函数。

   2. 第二步执行`FSlateApplication::ProcessReply`，完成鼠标捕获与聚焦控制、鼠标进入离开判断、拖拽判断、控件导航等控件公共功能。

   若`OnPreviewMouseButtonDown`函数执行完毕后依然为FReply::Unhandled()状态，则重复上一步逻辑，但是在Lambda中传入`OnMouseButtonDown`或`OnTouchStarted`函数。

6. 若鼠标未被控件捕获

   查找鼠标位置对应的控件。

   1. 遍历每一个SWindow以及对应的子SWidget，在每个SWidget内查找控件，找到后返回子控件的路径`FWidgetPath`。
   2. 在SWidget内查找时，若鼠标坐标确实在窗口内，则在SWidget内的FHittestGrid容器中查找控件路径。
   3. FHittestGrid是SWidget专门用来查找指定位置控件的类，使用网络结构来加速。[【UEInside】HittestGrid点击机制剖析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/346460371)

   控件开始响应鼠标点击事件。

   1. 通过FEventRouter::Route<FReply>(FEventRouter::FTunnelPolicy())函数，依据捕获控件路径，从根节点循环的一层层向子节点传递，直到某一层返回FReply::Handled()或遍历到叶子节点。遍历的内部逻辑同第5步的处理逻辑一致，但是在Lambda中传入`OnPreviewMouseButtonDown`函数。
   2. 若`OnPreviewMouseButtonDown`函数执行完毕后依然为FReply::Unhandled()状态，则重复上一步逻辑，但是遍历顺序改为从子节点一层层向根节点传递，同时在Lambda中传入`OnMouseButtonDown`或`OnTouchStarted`函数。
   3. 控件处理过程中未手动聚焦输入，则自动完成聚焦。

7. 子控件输入处理完毕后，判断是否需要限制某些任务的运行，来提高接下来的输入响应速度。
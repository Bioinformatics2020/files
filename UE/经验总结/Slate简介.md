# Slate框架做了哪些

1. 平台层：解决各个硬件或配置之间的差异，例如：

   用户输入：不论用户是从Windows还是Mac，不论是键盘输入还是触摸输入，全部通过FSlateApplication的输入相关函数处理。

   渲染输出：不论用户是Android还是平板，不论平台DirectX还是OpenGL，







为什么需要UMG：slate由纯c++实现，提供用户输入事件、焦点事件、控件布局、裁剪变换、渲染显示等。所有UI相关的工作均可以由slate完成、并且保持了c++高效、易于直接调用底层渲染方法等优点。但是slate继承c++优点的同时也继承了c++的缺点：代码逻辑复杂且无法实时修改调试与预览，对设计人员不友好等问题。ue通过使用UWidget，继承了UObject的反射特性，通过蓝图实现了简单好用的页面设计功能。需要注意的是UMG出现的核心目的是便于在蓝图中设计UI。



通过什么方法调用GPU的绘制接口：

1. GPU如何绘制的这一部分比较难，我还没有看，我目前确定我们所有UI都是通过FSlateDrawElement来间接调用GPU的，并且FSlateDrawElement用法比较单纯。

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




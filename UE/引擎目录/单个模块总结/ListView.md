## 一、SListView的基本介绍

### 什么是view

view(视图)可以理解为原始数据的一个简便的可交互窗口。string通过对应的string_view获取子字符串，数据库通过视图来显示与管理筛选查找的结果，listview通过view显示特定的数据。listview的特殊之处在于需要与slate结合，将数据以可视化的形式展现出来。



### SListView的核心功能：

1. 需要显示子控件并支持滚动

   基于slate框架的特性完成控件显示。展开来说分为3部分来完成

   - UI显示：需要动态创建子控件，并对子控件进行合理的布局与显示
   - UI输入：需要响应用户的触摸或点击操作
   - 程序化输入与输出：需要创建或修改ListView属性、发出各种类型的回调事件

2. 需要达到足够的优化效果

   主要优化手段有两类

   - 仅对需要显示的数据创建对应的控件类
   - 使用控件缓存，通过控件类生成器(FWidgetGenerator)来管理控件的创建与销毁



### SListView的高扩展性结构

只有扩展性足够强，才能适应复杂多变的需求

1. 在STableViewBase完成基本的框架布局、通用函数声明、计算滚动偏移与滚动效果。
2. 在SListView计算、创建、更新需要显示的子控件，并通知SListPanel完成显示。在SListView管理控件选中与高亮事件，SListView中有大量代码与此功能相关
4. 在SListPanel完成具体的子控件的排版与显示
5. 在SScrollBar完成滚动条的显示与操作
6. 提供SHeaderRow来显示标题行
7. SListView不要求数据类一定是UObject，只要求提供对应的模板函数来返回对应控件
7. 通过ITableRow的接口来调用控件类的各种事件



### UI设计细节

做出来的UI效果只有足够好，才会在未来得到广泛应用

1. 支持水平滚动与垂直滚动
2. 支持惯性滚动与过度滚动
3. 支持给定偏移值直接定位
4. 支持根据数据类找控件类、根据控件类找数据类
5. 支持鼠标捕获，保证手势超过控件区域依然生效
6. 支持鼠标手势、鼠标滚轮、触摸手势操作



### SListView的扩展控件

**STileView：将水平或垂直排列扩展为网格状排列**

1. STileView要求子控件大小是固定的，必须是统一的高度或宽度
2. 由STileView完成子控件排列规则



**STreeView：提供了类似文件浏览器的子控件折叠与展开功能**

1. 数据类的管理与使用上主要区别是多了一个OnGetChildren功能
2. STreeView控制子控件折叠与展开状态



**SCListView：适配项目的特殊需求**

1. 滑动选择功能
2. 居中显示功能
3. 上拉或下拉滚动刷新功能
4. 翻页功能
5. 自定义滚动条属性
6. 额外的底部填充



## 二、实现细节

### UI显示

#### UI创建

基本框架搭建

1. 使用常规的slate创建方法new一个SListView，参考如下代码

   ```c++
   SNew( SListView< ItemType > )//new 一个SListView，UListView传入的数据类类型是UObject*
       .ListItemsSource( &Items )//传入数据
       .OnGenerateRow_UObject(Implementer, &UListViewBaseT::HandleGenerateRow)//传入根据数据生成控件类的函数指针
       ...//其它非必须的属性、如聚焦事件、选择模式、滚动回调等等
   ```

2. SNew的执行过程中发生了什么：

   ```c++
   //1. 原生c++构造SListView
   MakeShared<SListView>();
   //2. 创建参数列表，读取SNew函数后方传入的slate参数，并保存到FArguments内
   SListView<ItemType>::FArguments();
   //3. 执行slate(SListView)的Construct函数，读取FArguments的参数，然后根据参数创建控件的各个子控件
   _Widget->SWidgetConstruct(InArgs);
   //4. 创建控件的各个子控件，
   ConstructChildren();
   ```

   











UListView的基本介绍

以上SListView的功能虽然基本上已经完善，但是Slate代码不能由蓝图直接使用，必须需要经过UWidget封装。

1. UListView主要是对SListView常用功能进行封装，让控件使用者尽量不需要关注SListView细节，只需要简单使用UListView就可以满足大部分功能。
2. UListView按照SListView要求，实现了根据传入数据类创建对应控件类的功能，我们可以自定义根据传入数据类应该返回什么控件类。
3. UListView在SListView对slate初步管理的基础之上，通过FUserWidgetPool实现了UserWidget的缓存





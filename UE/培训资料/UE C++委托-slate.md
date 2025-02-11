# UE C++基础

## 蓝图、c++混合编程

### 蓝图 vs C++

**蓝图优势：**
非常直观，可视化界面对设计与美术非常友好。蓝图提供了各种编辑器可以直观的看到接近真实运行状态的效果。
由于可见即可得，非常利于原型设计与快速迭代。
蓝图系统已经足够强大，完全可以依靠纯蓝图实现简单的游戏。

**蓝图的缺陷：**
蓝图运行于虚拟机，性能开销相比c++要大许多。
蓝图默认的资源引用比较简单粗暴，资源动态加载放在C++更好管理。
二进制文件难以进行版本管理。
UE绝大部分的函数没有被封装成蓝图。例如复杂容器、网络通信、控制台解析、第三方库调用等。
项目需求复杂之后难免需要扩展与修改引擎源码。如多平台适配、扩展媒体播放器、扩展浏览器的支持、修改渲染管线。
c++是一门非常成熟的语言，拥有大量第三方库、框架设计参考、学习资料等。

|      | 编辑器 | 独立进程 | 开发版 | 发行版 |
| ---- | ------ | -------- | ------ | ------ |
| CPP  | 2.3    | 2.8      | 2.58   | 2.43   |
| 蓝图 | 191    | 28       | 24     | 16     |

注：蓝图原生化方法可以进一步提升性能，但UE官方在5.0以后已放弃这套功能。

### 蓝图执行逻辑

1. 蓝图编译器：将节点整理成线性的运行逻辑，每个节点依次翻译为最基本的字节码。
2. 蓝图虚拟机：从c++调用蓝图函数时开始创建蓝图虚拟机FFrame。虚拟机本身开销很小，只提供了蓝图函数堆栈、字节码依次执行等基本的虚拟机功能。+
3. 异步节点：等同于将异步节点后方部分翻译成单独的函数，满足特定条件后触发回调来执行函数。
4. 蓝图调用c++函数：通过反射找到c++的函数并执行。
5. c++调用蓝图函数：调用BlueprintImplementableEvent等宏标记的函数、FindFunctionByName方法查找蓝图函数查找。
6. 通用方法：绑定与触发动态委托。

### 平衡使用蓝图与C++

**优先蓝图**

1. 方案探索阶段，需要对不同的设计方案快速作出demo时优先使用蓝图开发。注意方案确定后及时重构。
2. 如果团队中初级开发或美术成员较多，不要强行要求使用C++。
3. 对于频繁需要更新或难以使用代码的部分，如UI图标、物体摆放、材质效果、角色或UI动画等。

**优先C++**

1. 项目框架或模块框架搭建时，建议通过C++手动管理各个模块与子系统的生命周期，手动管理模块之间的引用关系。

 	2. 网络通信等大量逻辑处理的模块，建议充分利用C++的性能优势。
 	3. 基础函数库、可复用组件等比较独立的功能，C++比蓝图更容易迁移、蓝图封装的函数C++很难调用、蓝图难以继承与扩展。
 	4. 当业务逻辑逐渐复杂或混乱之后，蓝图的可维护性与性能开销直线下降，建议及时重构并改为C++。

**平衡考虑**

1. 简单的业务逻辑可以根据项目规范或开发的习惯决定使用蓝图还是C++。
2. 当某个蓝图可能被多个开发修改时，应该从模块或项目框架去解决问题，若难以解决时建议用C++来避免冲突。
3. 一般而言项目越大那么逻辑越复杂，那么就更侵向于C++开发。
4. 在C++中实现功能逻辑，在蓝图中完成场景或UI搭建是一个不错的选择。

## 委托详解

### 委托原理的demo

1. 回调函数举例

2. 封装出单播委托功能

3. 支持不同类型的回调函数(一)

   任意参数与返回值的函数

4. 支持不同类型的回调函数(二)

   任意可调用对象

5. 多播委托与宏封装

### UE实现的委托展示

**Delegate** 包含了委托系统基本的说明书；定义了实例化委托类的宏，这里的宏表示了委托本质是什么类；通过宏封装了动态委托的声明与实现，以及一些基本的绑定与触发函数。

**DelegateBase** 定义单播委托与多播委托的基类；基类中包含可调用的函数对象(TBaseUFunctionDelegateInstance / IDelegateInstance)，在DelegateAllocator内存储；封装了一些间接调用IDelegateInstance的方法。

**DelegateCombinations** 基于Delegate.h文件，封装了所有常用的委托类实例化方法，包括单播委托、多播委托、事件、动态单播委托、动态多播委托。

**DelegateInstanceInterface** IBaseDelegateInstance定义了函数对象(委托实例)的接口，包括创建与触发函数；TMemFunPtrType声明了成员函数类型；TPayload用于定义UFunction的参数与返回值类型；TPlacementNewer用于临时存储UFunction的参数与返回值数据。

**DelegateInstancesImpl** IBaseDelegateInstance针对多种类型函数的实现，分别为：

TBaseUFunctionDelegateInstance，针对UFunction的实现，通过函数名称进行初始化，通过Weak指针保存对象；
TBaseSPMethodDelegateInstance，针对智能指针保护的类的成员函数，通过Weak指针保存对象；
TBaseRawMethodDelegateInstance，针对普通类的成员函数，无保护；
TBaseUObjectMethodDelegateInstance，针对UClass的成员函数，通过Weak指针保存对象；
TBaseStaticDelegateInstance，针对静态函数，将静态函数转换为普通函数指针进行保存，也可用于普通c++函数；
TBaseFunctorDelegateInstance，对任意一个的可调用对象均可使用，包括普通函数、函数指针、仿函数、Lambda表达式、std::function等，本质上直接调用Invoke，无保护；
TWeakBaseFunctorDelegateInstance，针对与智能指针关联的任意的可调用对象，当Weak指针有效时执行。

**DelegateSettings** 配置文件，发行版不可通过DelegateInstance获取UFunction名称。

**DelegateSignatureImpl** 委托类的函数声明与实现，将所有委托的公开方法整理到一起便于查看，文件后缀为.ini。分为4类委托类型：

TDelegate，单播委托，委托的核心，在TDelegateBase保存的函数指针基础之上封装了各种绑定函数、解绑函数、拷贝函数等，根据委托类型定义了多种DelegateInstances用于可能需要的便捷操作；
TMulticastDelegate，多播委托，在TMulticastDelegateBase基础之上封装了各种绑定函数、解绑函数操作；
TBaseDynamicDelegate，动态单播委托，在TScriptDelegate基础上封装了一个绑定函数；
TBaseDynamicMulticastDelegate，动态多播委托，在TMulticastScriptDelegate基础上封装了多种绑定函数

**IDelagateInstance** FDelegateHandle作为一个唯一标识符用来在多播委托中对每一个委托进行标识；IDelegateInstance作为DelegateInstance的纯虚基类，声明了多个纯虚函数。

**MulticastDelegateBase** 多播委托的基类，通过一个数组包含了多个单播委托。

**ScriptDelegates** 动态委托的基类，在“UObject/ScriptDelegates.h”目录下。需要与UPROPERTY(BlueprintAssignable)配合使用，分为两类委托：

TScriptDelegate，动态单播委托，与普通的委托类似，但是提供了额外的序列化函数。
TMulticastScriptDelegate，动态多播委托，通过一个数组包含了多个动态单播委托。

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
- 将Demo工程导入Root\Engine\Source\Programs\，然后重新运行Root\GenerateProjectFiles.bat

### Slate输入响应功能

**以Windows的鼠标点击事件为例，处理流程如下：**

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

5. 若鼠标被控件捕获，则直接转发事件到子控件

   获取到被捕获控件的路径

   通过FEventRouter::Route<FReply>(FEventRouter::FToLeafmostPolicy())函数，依据捕获控件路径找到子节点控件，向控件传递鼠标点击事件，处理逻辑如下

   1. 第一步执行传入的lambda表达式，这里的lambda仅执行SWidget中的`OnPreviewMouseButtonDown`函数。

   2. 第二步执行`FSlateApplication::ProcessReply`，完成鼠标捕获与聚焦控制、鼠标进入离开判断、拖拽判断、控件导航等控件公共功能。

   若`OnPreviewMouseButtonDown`函数执行完毕后依然为FReply::Unhandled()状态，则重复上一步逻辑，但是在Lambda中传入`OnMouseButtonDown`或`OnTouchStarted`函数。

6. 若鼠标未被控件捕获，则按照UE定义的规则一层层处理事件

   查找鼠标位置对应的控件。

   1. 遍历每一个SWindow以及对应的子SWidget，在每个SWidget内查找控件，找到后返回子控件的路径`FWidgetPath`。
   2. 在SWidget内查找时，若鼠标坐标确实在窗口内，则在SWidget内的FHittestGrid容器中查找控件路径。
   3. FHittestGrid是SWidget专门用来查找指定位置控件的类，使用网络结构来加速。[【UEInside】HittestGrid点击机制剖析 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/346460371)

   控件开始响应鼠标点击事件。

   1. 通过FEventRouter::Route<FReply>(FEventRouter::FTunnelPolicy())函数，依据捕获控件路径，从根节点循环的一层层向子节点传递，直到某一层返回FReply::Handled()或遍历到叶子节点。遍历的内部逻辑同第5步的处理逻辑一致，但是在Lambda中传入`OnPreviewMouseButtonDown`函数。
   2. 若`OnPreviewMouseButtonDown`函数执行完毕后依然为FReply::Unhandled()状态，则重复上一步逻辑，但是遍历顺序改为从子节点一层层向根节点传递，同时在Lambda中传入`OnMouseButtonDown`或`OnTouchStarted`函数。
   3. 控件处理过程中未手动聚焦输入，则自动完成聚焦。

7. 子控件输入处理完毕后，判断是否需要限制某些任务的运行，来提高接下来的输入响应速度。

**总结：** Windows输入系统绑定与监听----系统鼠标事件响应----目标控件查找----事件下发与传递响应。

## 宏

- 宏定义

- 宏替换

- 可变参数宏替换
- 标准库默认宏、编译器默认宏
- 宏展开顺序
- 总结：原理核心只有宏定义与宏替换；用法主要是对代码封装，让大家写代码更简洁。

## 模板

模板知识浩如烟海，我讲不清楚，推荐一本书大家看看吧CPP-Templates-2nd

对应翻译https://github.com/Walton1128/CPP-Templates-2nd--与[r00tk1ts/cpp-templates-2nd: Translation of C++ Templates 2nd | Powered by Obsidian (github.com)](https://github.com/r00tk1ts/cpp-templates-2nd)

- 第1章 函数模板（Function Templates）
- 第2章 类模板（Class Templates）
- 第3章 非类型模板参数
- 第4章 变参模板
- 第5章 基础技巧
- 第6章 移动语义和enable_if<>
- 第7章 按值传递还是按引用传递？
- 第8章 编译期编程
- 第9章 在实践中使用模板
- 第10章 模板基本术语
- 第11章 泛型库
- 第12章 深入模板基础
- 第13章 模板中的名称
- 第14章 实例化
- 第15章 模板实参推导
- 第16章 特化与重载
- 第17章 通往未来
- 第18章 模板的多态性
- 第19章 萃取的实现
- 第20章 基于类型属性的重载（Overloading on Type Properties）
- 第21章 模板和继承
- 第22章 桥接static和dynamic多态
- 第23章 元编程
- 第24章 类型列表（Typelists）
- 第25章 元组（Tuples）


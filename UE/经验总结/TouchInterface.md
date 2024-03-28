定义一个界面，通过该界面可以使用触摸输入来模拟轴输入。通过项目设置->输入设置->DefaultTouchInterface配置来修改轴输入界面



**生命周期**

1. 玩家控制器初始化输入输入系统时开始创建UTouchInterface。
2. 读取输入配置文件InputSettings的DefaultTouchInterface参数，并加载对应的UTouchInterface。
3. 创建SVirtualJoystick并添加到视口，ZOrder为0。
4. 通过UTouchInterface的属性初始化SVirtualJoystick。
5. 在SVirtualJoys的tick内监听触摸输入，并将输入转换为对应轴输入
6. EndPlay时销毁UTouchInterface。

**技术细节**

APlayerController:

读取对应配置文件来管理需要控制杆的创建与显示



SVirtualJoystick：

使用一个简单的状态机模型来维护控件显示透明度

通过触摸开始的位置来决定聚焦哪个控制杆，以及对应控件显示的位置

将触摸移动距离计算后，通过FSlateApplication的OnControllerAnalog来模拟为轴输入

使用for循环依次绘制每个控制杆，每个控制杆由两个box组成



UTouchInterface：

用于通过蓝图给SVirtualJoystick传递参数
# InputCore

**InputCoreTypes** 包含多种摄入设备相关的枚举类型定义。

​	EControllerHand：用于某个输入设备标识，主要用于VR、XR等情景的多设备标识。
​	EPairedAxis：定义轴输入设备的XYZ轴，例如鼠标的XY轴。
​	FKey：一个键的标准定义结构，由键名与FKeyDetails组成。
​	FKeyDetails：定义键的详细类型，如普通按键、修饰键、手柄、触摸、轴、鼠标等。
​	ETouchIndex：定义触摸输入的手指索引。
​	EConsoleForGamepadLabels：定义手柄的类型，PS4、XBoxOne。
​	EKeys：详细定义了所有按键，本质是一个结构体，在FEngineLoop::PreInitPreStartupScreen初始化。
​	FInputKeyManager：用于将uint32类型的Key代码与FKey相互转换。

**GenericPlatformInput** 各个平台拥有不同的实现。定义输入键的编码到FKey之间的转换规则，用于FInputKeyManager生成转换表。
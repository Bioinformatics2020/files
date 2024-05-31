**DelayAction** 继承自FPendingLatentAction，用于倒计时任务执行，通过LatentActionManager发挥作用

# GameFramework(游戏框架)

**TouchInterface**：一个用于接收触摸输入的UI摇杆框架，移动端默认启用，由PlayController管理。

# Engine

**LatentActionManager** 潜在事件管理器，管理delay执行的事件，可以通过继承FPendingLatentAction来定制各种定时功能。在GameInstance构造时完成初始化，在每个UserWidget、Actor、ActorComponent执行Tick的最后触发delay事件，若Object内部未执行Tick，则在LeverTick中完成剩余潜在事件的Tick。

运行逻辑如下：绑定新的潜在事件-->由Actor或Lever每帧触发事件检测-->若满足触发条件则根据时间名称找到对应函数并执行。
支持的潜在事件类型举例：delay时间触发、delay帧数触发、插值移动(事件检测时移动，移动完成后触发事件)等等。

**TimerManager** 定时事件管理器。提供了定时触发委托的功能，支持循环触发、暂停继续等功能，使用UE委托功能完成回调。在LeverTick中完成函数触发，使用堆机制来排列回调事件，保证了事件的时间顺序。
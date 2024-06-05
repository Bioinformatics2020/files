# Slate

**SlateSetting** 配置文件

## Widgets

### Colors 

**SColorBlock** 一个支持半透明的纯色色块，点击时会触发回调。

**SColorGradingWheel** 用于属性参数面板的色轮面板，功能类似SColorWheel，样式存在差异。

**SColorSpectrum** 色谱面板，由色谱图片和采样点图片构成，鼠标点击或移动是会计算当前点的颜色并触发回调。

**SColorWheel** 更常用的色轮面板，由色轮图片和采样点图片构成，鼠标点击或移动是会计算当前点的颜色并触发回调。

### Image

**STrobber** 动态浏览图示，文件内包含一个转圈圈的loading控件，一个条状的loading控件。使用slate完成控件动画，代码非常简洁。

**SSpinningImage** 快速旋转的图片。最终调用了FSlateDrawElement::MakeRotatedBox绘制带角度的Box。

### Layout

**SPopup** 基础的SCompoundWidget，但是是在一般的控件渲染完成之后渲染，位于界面最上层。

**SSpacer** 布局填充控件，仅参与控件大小与位置计算，不渲染。

# SlateCore

### Animation

**CurveHandle** 配合CurveSequence完成曲线计算，作为计算的核心节点存在。

**FCurveSequence** 管理时间进度与流动，基于其管理的时间，在CurveHandle中查找对应的值。同时提供了一个Tick委托绑定函数。

**SlateSprings** 模拟弹簧运动曲线计算。一个独立的数值计算功能，仅依赖FMath完成计算。
## 代码运行流程

### 初始化

#### UQNXDisplayManagerBPLibrary::CreateDisplaySurface()

**Game线程**

1. 创建FQNXDisplaySurface来管理新窗口
2. 创建一个应用窗口：SDL_CreateWindow
3. 设置窗口位置：SDL_SetWindowPosition
4. 绑定初始化函数到Rander线程
5. 初始化一个新的线程，专门用于新窗口的渲染与更新，等待发送信号后新线程才会执行
6. 尝试创建自定义渲染Present

**Rander线程**

1. new一个RHI命令：FRHICommandCreateDisplaySurface
2. 刷新命令缓冲区，完成所有RHI命令执行

**RHI线程**

1. 初始化SDL_CreateWindow创建的窗口
   1. 使用SDL创建OpenGL上下文
   2. 编译顶点与片元着色器，链接到OpenGL
   3. 初始化OpenGL的各种属性
   4. 更新新窗口的屏幕缓冲区

#### UQNXDisplayManagerBPLibrary::AssignDisplaySurface()

**Game线程**

​	在FQNXDisplaySurface中绑定UTextureRenderTarget2D

### 更新窗口

#### UQNXDisplayManagerBPLibrary::TickSurfaces()

Game线程：若CreateDisplaySurface()在Game线程的第6步因为执行时机等特殊原因执行失败，则重新尝试创建自定义渲染Present

#### UQNXDisplayManagerBPLibrary::RenderWidgetToRenderTarget()

Game线程：重新渲染UserWidget到RenderTarget2D

#### FQNXDisplayCustomPresent::Present()

RHI线程：

1. 若新线程创建成功则在新线程完成窗口渲染，否则在RHI线程完成渲染。
2. 通知新线程继续渲染

### 新线程

1. 等待RHI线程发送渲染信号。
2. 获取RenderTarget2D对应的纹理
3. 将纹理指针转换为OpenGL所需要的纹理指针类型
4. 更新纹理，完成OpenGL渲染
5. 更新新窗口的屏幕缓冲区
6. 阻塞Run循环，回到第一步。

## 总结

1. 使用DrawWidget函数将Widget渲染到RenderTarget
2. 使用SDL库配合OpenGL将RenderTarget渲染到新窗口
## 技术关键点

1. 命名线程与匿名线程分开处理
2. 单线程程序也能模拟出多线程的效果

## FTaskGraph创建

通过引擎启动阶段(FEngineLoop::PreInitPreStartupScreen)的FTaskGraphInterface::Startup函数完成创建与初始化

1. 通过FTaskGraphInterface创建FTaskGraphImplementation，此处传入的CPU core数量没有使用
2. 判断是否需要不同优先级的线程
   - 检查平台是否支持：iOS不支持
   - 检查IS_PROGRAM：与UBT有关，作用不明
   - 检查WITH_ENGINE：仅链接引擎core时禁用
   - 检查UE_SERVER：服务器模式下禁用
   - 检查系统是否支持多线程或是否启动多线程
3. 统计线程数量
   - 命名线程数量：包括StatsThread、RHIThread、AudioThread、GameThread、ActualRenderingThread。StatsThread只有启动STATS时存在。
   - Task线程优先级数量：包括高、标准、低优先级线程 或 仅一种线程
   - Task基本线程数量：参考CPU核心数量自动计算
   - 总工作线程数量：Task基本线程数量*Task线程优先级数量 + 命名线程数量
4. 添加引用计数，保证FTaskGraphImplementation只创建一次。
5. 分配TLS，为每一个线程创建一个变量用来保存当前线程的FWorkerThread地址
6. 对每一个WorkerThreads创建一个FRunnable对象
   - 命名线程：创建FNamedTaskThread
   - Task线程：创建FTaskThreadAnyThread
7. 对每一个Task线程，根据不同的优先级创建不同的线程实例(FRunnableThread)。对每一个命名线程，需要由该线程调用AttachToThread(ENamedThreads::Type CurrentThread)来完成线程绑定
8. 通过WorkerThreads记录与管理所有的工作线程

## 命名线程运行流程

命名线程运行流程：

1. 通过AttachToThread(ENamedThreads::Type CurrentThread)将当前线程(各种命名线程)绑定到WorkerThreads

## TaskThreadAnyThread的基本介绍

**基本特点介绍：**

- 继承自FTaskThreadBase：拥有单独线程或在主线程模拟单独线程
- 用来管理匿名线程的Task执行
- 具体执行的内容被FTaskGraphImplementation管理

**多线程模式运行流程如下：**

1. 构造函数与初始化函数：记录线程优先级、线程Id、线程TLS、FWorkerThread(线程信息管理结构体)
2. 线程初始化：在TLS记录当前线程对应的FWorkerThread
3. Run：
   1. 启动性能监测
   2. 对标准线程或高优先级线程预分配线程局部存储空间
   3. 循环执行ProcessTasks()，直到RequestQuit()为止(这里功能与ProcessTasks()内部的循环重复，怀疑是版本更迭导致的功能冗余)。
      ProcessTasks()内部是一个循环，也是直到RequestQuit()且FindWork()为空时停止。ProcessTasks逻辑如下：
      1. 通过FindWork()查找任务
      2. 若未找到，随机Sleep一段时间（Shipping或Test版本无延迟），然后执行默认的Event事件
      3. 若存在任务，执行FBaseGraphTask对应的任务
      4. 循环执行以上步骤

**单线程模式运行流程如下：**

1. 执行构造函数
2. 循环执行Tick函数，在Tick中执行ProcessTasks()，内部如下：
   1. 通过FindWork()查找任务
   2. 若未找到，随机Sleep一段时间（Shipping或Test版本无延迟），并直接结束ProcessTasks()
   3. 若存在任务，执行FBaseGraphTask对应的任务，回到第一步

## FTaskGraphImplementation的基本介绍

FTaskGraphImplementation是管理TaskGraph的核心。它的Public函数可以直接由用户调用，用来管理整个TaskGraph的运行状态。包括创建、销毁、修改任务系统，包括向某个指定线程添加Task、执行Task、停止Task等。

### Public函数介绍

**Get**

​	静态函数，获取FTaskGraph单例。

**FTaskGraphImplementation**

​	构造函数，创建WorkerThreads，在FEngineLoop::PreInitPreStartupScreen调用

**~FTaskGraphImplementation**

​	析构函数，销毁每一个线程

**QueueTask**

​	待定

**GetNumWorkerThreads**

​	获取工作线程数量，这里的数量为Task基本线程数量，而不是真实创建的匿名线程数量

**GetCurrentThreadIfKnown**

​	通过一个枚举类型的int值返回当前线程的索引以及线程的优先级，若当前线程未被TaskGraph管理则返回AnyThread

**IsThreadProcessingTasks**

​	返回当前命名线程是否正在执行Task

**AttachToThread**

​	将当前命名线程添加到TaskGraph进行管理，在每一个命名线程初始化时执行。

**ProcessThreadUntilIdle**

​	待定

**ProcessThreadUntilRequestReturn**

​	待定

**RequestReturn**

​	待定

**WaitUntilTasksComplete**

​	待定

**TriggerEventWhenTasksComplete**

​	待定

**AddShutdownCallback**

​	添加TaskGraph关闭时触发的回调，目前在FTickTaskSequencer与FCookedAssetRegistryPreloader时绑定。

**WakeNamedThread**

​	待定

**StartTaskThread**

​	待定

**FindWork**

​	待定

**StallForTuning**

​	待定

**SetTaskThreadPriorities**

​	设置所有Task线程优先级，仅关闭多个Task优先级时有效，通过控制台命令调用。






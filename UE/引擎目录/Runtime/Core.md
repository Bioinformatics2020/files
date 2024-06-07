# Algo 算法(algorithm)

**Accumulate** 累加。对容器进行累加	

**AllOf AnyOf NoneOf** 判断是否为真，分别为全真则真，一真则真，全假则真。

**BinarySearch** 二分查找。对传入的数据按照给定的规则进行二分查找

**Compare** 比较两个容器的每一个元素是否相等

**Copy** 将指定或满足条件的元素拷贝到另一个容器

**Count** 统计容器内满足条件的元素

**Find** 查找容器内满足条件的元素

**FindLast** 逆序查找容器内满足条件的元素

**FindSequence** 查找一串元素。查找容器的子串

**FindSortedStringCaseInsensitive** 查找已排序的字符串，忽略大小写。在已排序的字符串数组中查找给定字符串

**ForEach** 遍历容器，依次执行指定函数

**Heapify HeapSort** 堆排序

**IndexOf** 查找给定元素的下标

**IntroSort**  简单的排序。通过快排与堆排序结合实现高效排序，不稳定排序

**IsHeap** 检查容器是否为堆排序的

**IsSorted** 检查容器是否已排序

**LegacySort** 不稳定排序

**LevenshteinDistance** 对两个数组进行差异比较的算法

**MaxElement MinElement** 返回容器的最大值或最小值

**Partition** 分割。将容器按照给定谓语排序为前后两部分，数据不稳定

**RandomShuffle** 随机洗牌。将容器内元素打乱顺序

**RemoveIf** 移除满足条件的元素

**Replace** 将容器内的某个值替换为新值

**Reverse** 容器倒序

**Rotate** 旋转。将给定元素从范围的前面移动到范围的后面

**SelectRandomWeighted** 加权随机选择。随机获取容器内一个元素，每个元素被获取的概率权重不同

**Sort** 同IntroSort，简单的排序

**StableSort** 稳定的排序。冒泡排序与归并排序结合

**Transform** 转换。将数据从一个容器移动到另一个容器

# Containers 容器

参考“UE4容器”文章

**Ticker** Tick执行工具，在FEngineLoop::Tick()里执行Tick操作，便于简单自由的添加定时操作。

# Delegates 委托

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

# Hash

**CityHash** google开发的一套非常高效的Hash算法。

**xxHash** 针对小数据集速度更快的Hash算法。引擎中目前未使用。

# Internationalization 国际化

# Logging 日志

**IMessageLog** 基于原始的FMsg::Logf方法，封装了类似于UE_LOG的另一套log输出方法，期望用于包含丰富标记信息的log，例如颜色、大量标签、按页分类等。

**LogCategory** 用于UE_LOG中对log添加标签以及定义log级别，以及将log标签添加到FLogSuppressionInterface进行管理。通过DECLARE_LOG_CATEGORY_EXTERN等宏来继承FLogCategory完成新log分类的定义。

**LogMacros** 定义了输出Log相关的多个宏，提供最简单的log输出、分类标签定义、出现Fatal级别log时的堆栈与崩溃、调试时针对指定log的断点等等。

**LogScopedCategoryAndVerbosityOverride** 在TLS中保存log分类与级别。只在UE_SUPPRESS时保存，离开后恢复，只在FOutputDevice用到。不确定使用这个东西的意义是什么。

**LogScopedVerbosityOverride** 用于动态修改日志显示级别，封装成了一个宏，离开作用域时恢复log级别。在某些情况下可能期望出现更详细或更简略的log内容，引擎中有少量地方使用到。

**LogSuppressionInterface** 根据命令行、配置文件管理每个类别的Log开关与输出级别。

**LogVerbosity** 定义log输出级别从无Log到崩溃Log共7级，输出设备会根据Log级别来添加颜色与过滤。

# Math 数学

**Axis** 通过枚举定义了几个常用的轴，X、Y、Z、XY等。

**BigInt** 固定位数的大数存储与计算，提供了常规的数值计算方法，并未进行复杂生涩的算法优化。

**Box** 表示对齐坐标轴的3D盒体，由两个顶点构成。提供了点到盒体距离计算、点是否在盒体内、盒体与盒体求交、盒体空间变换等基本的函数。

**Box2D** 表示对齐坐标轴的2D矩形，功能基本同2D版本的Box。

**Color** 提供了颜色的基础表示、转换、计算方法，如FLinearColor转FColor、去饱和度、RGB转sRGB等。

**Float16** 半精读浮点数，主要应用于2D空间。由1位浮点数、5位指数、10位有效数字构成。提供了与浮点数相互转换方法，未提供计算方法。

**Plane** 由法线与到原点距离构成的平面表达。

**RandomStream** 一套简单高效但随机程度够用的随机数生成器，不建议对随机结果求模，FMath::Rand开销更大但表现更好。

**Ray** 由原点与方向构成的射线表达。

**Sphere** 由中心点与半径构成的球体表达。

**Vector** 由x、y、z构成的3D坐标点或向量。

**Vector2D** 由x、y构成的2D坐标点或向量。

**Vector2DHalf** 由半精度浮点数x、y构成的2D坐标点或向量。

# Memory

**MemoryArena** 一套内存缓存策略，应该已经废弃了，只保留了一层壳，FMemory中包含更完善的分配策略。

**MemPro** 一套内存分析工具，通过在代码中重载内存分配函数并与MemPro程序配合使用。

# Misc 杂项

**DateTime** 通过int64存储以100纳秒为单位，存储0001-01-01 00:00:00.0到9999-12-31 23:59:59.9999999的时间。封装了大量时间格式转换方法可与各种格式表示的时间互相转换，同时额外提供获取当前时间、时间计算函数。

**Guid** 封装了Guid的各种格式转换方法以及Guid的生成方法。

**LazySingleton** 懒惰的单例，用于期望静态分配内存但是动态构造的单例类管理方法。文件内部提供了示例方法。

**Parse** 一个简易的命令行解析工具类，在字符串中查找指定的选项，返回选项是否找到以及选项对应的值。

**VarargsHelper** 封装了可变参数列表解析的方法，将可变参数列表按照字符串Fmt的格式解析到字符串Buffer中，并执行传入的代码片段(需要在这里使用解析完成的Buffer)，执行完成之后自动销毁Buffer。

**Timespan** 表示时间间隔的数据类型，主要用于配合DateTime完成时间计算。

# Templates 模板

​	除下方描述的几个重要模板外，还提供了多个类型萃取traits模板，少量其它特殊功能模板。

**Function** 封装所有的可调用对象。[UE4异步编程专题 - TFunction - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/61368703)

**Atomic** 官方的TAtomic不再维护，请使用std::atomic。[std::atomic - cppreference.com](https://zh.cppreference.com/w/cpp/atomic/atomic)

**Invoke** 触发可调用对象的函数。c++17提供std::invoke。

**RemoveCV** 移除类型的const与volatile属性。

**SharedPointer** 智能指针。[UE4随笔：UE4智能指针 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/433605801)

**Tuple** 元组。[深入分析虚幻源码——TTuple - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/250149211)，[std::tuple - cppreference.com](https://zh.cppreference.com/w/cpp/utility/tuple)

**UniquePtr** 智能指针之一。


UE4为了解决c++标准库提供的容器内存分配效率不够高的问题，重载了容器，使其更适合于游戏引擎。需要注意STL的容器操作算法非常丰富，如果你期望的操作UE容器不支持，可以尝试查询一下STL。

# 容器内存分配器

底层调用的是GMalloc分配内存

## 核心的分配器

分配的空间均为连续空间

### 对齐堆内存分配器

template<uint32 Alignment = DEFAULT_ALIGNMENT><br/>class TAlignedHeapAllocator

模板参数为内存对齐大小

**容器特征TAllocatorTraits**

SupportsMove    = true
IsZeroConstruct = true
SupportsFreezeMemoryImage = false

**最大元素数量**

int32上限

**分配的内存特点**

分配的内存起始位置会对齐到给定的对齐值Alignment 

**增长/保留/收缩策略**

同简单堆内存分配器

### 简单堆内存分配器

template <int IndexSize><br/>class TSizedHeapAllocator

模板参数为最大元素数量的类型

**容器特征TAllocatorTraits**

SupportsMove    = true
IsZeroConstruct = true
SupportsFreezeMemoryImage = false

**最大元素数量**

int8 或int16 或int32 或int64 上限

**分配的内存特点**

直接调用Realloc

**增长策略**

除给定要求外，预留额外3/8，并向上对齐

**保留策略**

将当前空间向上对齐

**收缩策略**

 当(剩余字节数>=16384 || 元素利用率 < 2/3) && (容器内无元素 || 剩余元素数>64)
则收缩容器到当前容器元素向上对齐后的大小

### 内联内存分配器

template <uint32 NumInlineElements, typename SecondaryAllocator = FDefaultAllocator><br/>class TInlineAllocator

模板参数分别为内联分配的元素数目与额外补充所使用的内存分配器(又名第二分配器)
默认第二分配器为简单堆内存分配器

**容器特征**

SupportsMove    同第二分配器的SupportsMove
IsZeroConstruct = false
SupportsFreezeMemoryImage = false

**最大元素数量**

int32上限

**分配的内存特点**

该分配器会在构造该分配器的区域直接创建一块内存，其大小为预设数目

当所分配元素小于预设元素数目时，使用预设的区域，若大于该区域，则使用第二分配器申请一块新内存容纳所有元素，并将预设空间的元素拷贝到第二分配器分配的内存

**增长/保留/收缩策略**

对于内联的空间则忽略，否则同第二内存分配器

### 不可浮动的内联内存分配器

template <uint32 NumInlineElements>
class TNonRelocatableInlineAllocator

模板参数为内联分配的元素数目

**容器特征**

SupportsMove = true
IsZeroConstruct = false
SupportsFreezeMemoryImage = false

**最大元素数量**

int32上限

**分配的内存特点**

分配过程类似于内联分配器，但是当元素数量超过预设时，不使用第二分配器，而是直接Malloc，相比内联内存分配器的优势在于，在调用GetAllocation()这个常用函数时，函数实现不必使用if判断，加快获取内存地址的过程

**增长/保留/收缩策略**

对于内联的空间则忽略，否则同简单堆内存分配器

### 固定内存分配器

template <uint32 NumInlineElements>
class TFixedAllocator

模板参数为内联分配的元素数目

**容器特征**

SupportsMove = true
IsZeroConstruct = false
SupportsFreezeMemoryImage = false

**最大元素数量**

int32上限

**分配的内存特点**

类似内联分配器，但是不提供第二分配器，使用者必须保证分配的大小小于预设大小

**增长/保留/收缩策略**

忽略变化

## 扩展的内存分配器

由核心分配器组成的，更适用于特定容器的内存分配器

### SparseArray内存分配器

template<typename InElementAllocator = FDefaultAllocator,typename InBitArrayAllocator = FDefaultBitArrayAllocator>
class TSparseArrayAllocator

默认模板参数分别为简单堆内存分配器<32>，内联内存分配器<4>

该分配器由两个分配器组成，即为模板参数所示的两个分配器

### Set内存分配器

template<
	typename InSparseArrayAllocator               = TSparseArrayAllocator<>,
	typename InHashAllocator                      = TInlineAllocator<1,FDefaultAllocator>,
	uint32   AverageNumberOfElementsPerHashBucket = DEFAULT_NUMBER_OF_ELEMENTS_PER_HASH_BUCKET,
	uint32   BaseNumberOfHashBuckets              = DEFAULT_BASE_NUMBER_OF_HASH_BUCKETS,
	uint32   MinNumberOfHashedElements            = DEFAULT_MIN_NUMBER_OF_HASHED_ELEMENTS>
class TSetAllocator

模板参数为两个内存分配器，哈希桶最大平均元素数量，哈希桶元素数量基数，最小总元素数量(小于此值将不分桶)

提供一个函数，根据桶元素数量，桶平均元素数量，返回需要的哈希桶的数量。

# 基础的顺序存储容器

## TArray

template<typename InElementType, typename InAllocator>
class TArray

模板参数分别为 元素类型与内存分配器
默认使用简单堆内存分配器，使用连续空间

TArray与STL vector原理类似

### 公开成员与方法

#### 构造函数/析构函数

| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| TArray()                                                     | 无参构造                                                     |
| TArray(const ElementType* Ptr, SizeType Count)               | 使用原生数组进行初始化                                       |
| TArray(const TArrayView<OtherElementType, OtherSizeType>& Other) | 使用TArrayView进行初始化                                     |
| TArray(std::initializer_list<InElementType> InitList)        | 使用成员初始化列表进行初始化                                 |
| TArray(const TArray<OtherElementType, OtherAllocator>& Other) | 使用不同类型的TArray进行初始化                               |
| TArray(TArray<OtherElementType, OtherAllocator>&& Other)     | 上一函数的右值版本                                           |
| TArray(const TArray& Other)                                  | 使用相同类型的TArray进行初始化                               |
| TArray(TArray&& Other)                                       | 上一函数的右值版本                                           |
| TArray(const TArray& Other, SizeType ExtraSlack)             | 使用相同类型的TArray进行初始化并在TArray末尾额外分配ExtraSlack大小的空间 |
| TArray(TArray<OtherElementType, Allocator>&& Other, SizeType ExtraSlack) | 上一函数的右值版本                                           |
| ~TArray()                                                    | 销毁TArray                                                   |

#### 操作符重载
| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| TArray& operator=(std::initializer_list<InElementType> InitList) | 使用初始化列表给自己赋值                                     |
| TArray& operator=(const TArray& Other)                       | 使用相同类型的TArray给自己赋值                               |
| TArray& operator=(TArray&& Other)                            | 上一函数的右值版本                                           |
| TArray& operator=(const TArrayView<OtherElementType, OtherSizeType>& Other); | 使用不同类型的TArray给自己赋值                               |
| ElementType& operator[](SizeType Index)                      | 按索引获取TArray的元素                                       |
| const ElementType& operator[](SizeType Index) const          | 上一函数的const版本                                          |
| bool operator==(const TArray& OtherArray) const              | 判断两个TArray是否元素个数相同且每个元素均相同               |
| bool operator!=(const TArray& OtherArray) const              | 判断两个TArray是否存在不同上一个函数取反                     |
| TArray& operator+=(const TArray& Other)                      | 向TArray中添加另一个TArray中的元素                           |
| TArray& operator+=(TArray&& Other)                           | 上一个函数的右值版本                                         |
| TArray& operator+=(std::initializer_list<ElementType> InitList) | 向TArray中添加初始化列表中的元素                             |
| void* operator new( size_t Size, TArray<T,Allocator>& Array ) | 非成员函数，在TArray末尾构造元素，元素的可使用有参数的构造函数eg:TArray<FVector> new(Array) FVector(0,0,0); |
| void* operator new( size_t Size, TArray<T,Allocator>& Array, typename TArray<T, Allocator>::SizeType Index ) | 非成员函数，在TArray的指定位置构造元素，元素的可使用有参数的构造函数eg:TArray<FVector> new(Array,Index) FVector(0,0,0); |
| friend FArchive& operator<<(FArchive& Ar, TArray& A)         | 使用文档类完成TArray的序列化                                 |

#### 查询
| 方法                                             | 作用                                       |
| :----------------------------------------------- | ------------------------------------------ |
| ElementType* GetData()                           | 返回指向第一个元素的指针                   |
| const ElementType* GetData() const               | 上一函数的const版本                        |
| uint32 GetTypeSize() const                       | 返回元素类型大小                           |
| SIZE_T GetAllocatedSize(void) const              | 返回容器可用于元素存储的内存大小           |
| SizeType GetSlack() const                        | 返回当前剩余的可容纳元素数量               |
| void CheckInvariants() const                     | 检查当前TArray是否为有效的                 |
| void RangeCheck(SizeType Index) const            | 检查传入的元素索引是否为有效的             |
| bool IsValidIndex(SizeType Index) const          | 检查传入的元素索引是否为有效的，返回真假   |
| SizeType Num() const                             | 返回当前TArray的元素数量                   |
| SizeType Max() const                             | 返回当前TArray最大的元素数量               |
| bool Contains(const ComparisonType& Item) const  | 检查是否存在满足operator==(Key)的元素      |
| bool ContainsByPredicate(Predicate Pred) const   | 检查是否存在满足给定谓词的元素             |
| void CheckAddress(const ElementType* Addr) const | 检查给定元素地址，是否不存在于当前TArray中 |

#### 查找
| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| ElementType& Top()                                           | 返回最后一个元素                                             |
| const ElementType& Top() const                               | 上一函数的const版本                                          |
| ElementType& Last(SizeType IndexFromTheEnd = 0)              | 返回倒数第n个元素，从0开始                                   |
| const ElementType& Last(SizeType IndexFromTheEnd = 0) const  | 上一函数的const版本                                          |
| bool Find(const ElementType& Item, SizeType& Index) const    | 查找元素，返回是否找到及其索引                               |
| SizeType Find(const ElementType& Item) const                 | 查找元素，返回其索引                                         |
| bool FindLast(const ElementType& Item, SizeType& Index) const | 从后向前查找元素，返回是否找到及其索引                       |
| SizeType FindLast(const ElementType& Item) const             | 从后向前查找元素，返回其索引                                 |
| SizeType IndexOfByPredicate(Predicate Pred) const            | 查找满足给定谓词的元素，返回其索引                           |
| SizeType FindLastByPredicate(Predicate Pred, SizeType Count) const | 从第Count个元素开始，从后向前查找满足给定谓词的元素，返回其索引 |
| SizeType FindLastByPredicate(Predicate Pred) const           | 从后向前查找满足给定谓词的元素，返回其索引                   |
| SizeType IndexOfByKey(const KeyType& Key) const              | 查找满足operator==(Key)的元素，返回其索引                    |
| ElementType* FindByKey(const KeyType& Key)                   | 返回满足operator==(Key)的元素                                |
| const ElementType* FindByKey(const KeyType& Key) const       | 上个函数const版本                                            |
| ElementType* FindByPredicate(Predicate Pred)                 | 返回满足给定谓词的元素                                       |
| const ElementType* FindByPredicate(Predicate Pred) const     | 上个函数const版本                                            |
| TArray<ElementType> FilterByPredicate(Predicate Pred) const  | 返回所有满足给定谓词的元素TArray                             |
| bool FindItemByClass(SearchType **Item = nullptr, SizeType *ItemIndex = nullptr, SizeType StartIndex = 0) const | UE4特有的查找，要求元素是UObject*或其子类类型。传入类的指针，查找的起始索引，返回是否找到、首个继承自传入类型的元素指针、该元素的索引 |

#### 插入
| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| typename TEnableIf<!TIsAbstract<ET>::Value, ElementType>::Type Pop(bool bAllowShrinking = true) | 移除TArray最后一个元素，并安全的返回该元素(避免元素为抽象类) |
| void Push(const ElementType& Item)                           | 向TArray末尾添加一个元素                                     |
| void Push(ElementType&& Item)                                | 上一函数的移动版本                                           |
| SizeType AddUninitialized(SizeType Count = 1)                | 添加任意数量未初始化元素到末尾                               |
| void InsertUninitialized(SizeType Index, SizeType Count = 1) | 在给定索引处插入任意数量未初始化元素                         |
| void InsertZeroed(SizeType Index, SizeType Count = 1)        | 在给定索引处插入任意数量0初始化的元素                        |
| ElementType& InsertZeroed_GetRef(SizeType Index)             | 在给定索引处插入0初始化元素，并返回其引用                    |
| void InsertDefaulted(SizeType Index, SizeType Count = 1)     | 在给定索引处插入任意数量默认初始化的元素                     |
| ElementType& InsertDefaulted_GetRef(SizeType Index)          | 在给定索引处插入默认初始化元素，并返回其引用                 |
| SizeType Insert(std::initializer_list<ElementType> InitList, const SizeType InIndex) | 在给定索引处插入初始化列表的元素，返回插入的首元素索引       |
| SizeType Insert(const TArray<ElementType, OtherAllocator>& Items, const SizeType InIndex) | 在给定索引处插入另一个TArray的元素，要求当前TArray的元素可以被另一个TArray的元素初始化 |
| SizeType Insert(TArray<ElementType, OtherAllocator>&& Items, const SizeType InIndex) | 上一函数的右值版本                                           |
| SizeType Insert(const ElementType* Ptr, SizeType Count, SizeType Index) | 在给定索引处插入另一个元素数组的任意数量元素，返回插入的首元素索引 |
| SizeType Insert(const ElementType& Item, SizeType Index)     | 在给定索引插入元素，要求该元素当前不存在于当前TArray内，返回插入的元素索引 |
| SizeType Insert(ElementType&& Item, SizeType Index)          | 上一函数的右值版本                                           |
| ElementType& Insert_GetRef(const ElementType& Item, SizeType Index) | 在给定索引插入元素，要求该元素当前不存在于当前TArray内，返回插入的元素引用 |
| ElementType& Insert_GetRef(ElementType&& Item, SizeType Index) | 上一函数的右值版本                                           |
| void Append(const TArray<OtherElementType, OtherAllocator>& Source) | 在最后插入另一个TArray中的元素                               |
| void Append(TArray<OtherElementType, OtherAllocator>&& Source) | 上一函数的右值版本                                           |
| void Append(const ElementType* Ptr, SizeType Count)          | 在最后插入元素数组中的元素                                   |
| void Append(std::initializer_list<ElementType> InitList)     | 在最后插入成员初始化列表中的元素                             |
| SizeType Emplace(ArgsType&&... Args)                         | 在TArray的末尾使用有参数的构造函数插入一个新项，返回元素索引 |
| ElementType& Emplace_GetRef(ArgsType&&... Args)              | 在TArray的末尾使用有参数的构造函数插入一个新项，返回元素引用 |
| voidEmplaceAt(SizeType Index, ArgsType&&... Args)            | 在给定索引处使用有参数的构造函数插入一个新项                 |
| ElementType& EmplaceAt_GetRef(SizeType Index, ArgsType&&... Args) | 在给定索引处使用有参数的构造函数插入一个新项，返回元素引用   |
| SizeType Add(ElementType& Item)                              | 向TArray末尾添加一个元素，返回元素索引                       |
| SizeType Add(ElementType& Item)                              | 上一函数的右值版本                                           |
| ElementType& Add_GetRef(const ElementType& Item)             | 向TArray末尾添加一个元素，返回元素引用                       |
| ElementType& Add_GetRef(ElementType&& Item)                  | 上一函数的右值版本                                           |
| SizeType AddZeroed(SizeType Count = 1)                       | 向TArray末尾0初始化任意数量元素，返回元素索引                |
| ElementType& AddZeroed_GetRef()                              | 向TArray末尾0初始化一个元素，返回元素引用                    |
| SizeType AddDefaulted(SizeType Count = 1)                    | 向TArray末尾默认初始化任意数量元素，返回元素索引             |
| ElementType& AddDefaulted_GetRef()                           | 向TArray末尾默认初始化一个元素，返回元素引用                 |
| SizeType AddUnique(ElementType& Item)                        | 若元素不存在则Add该元素，返回元素索引                        |
| SizeType AddUnique(ElementType&& Item)                       | 上一函数的右值版本                                           |

#### 删除

| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| void RemoveAt(SizeType Index)                                | 删除给定索引处元素，会收缩TArray                             |
| void RemoveAt(SizeType Index, CountType Count, bool bAllowShrinking = true) | 删除给定索引处任意数量的元素，默认会收缩TArray               |
| void RemoveAtSwap(SizeType Index)                            | 删除给定索引处元素，将最后的元素填充至此，会收缩TArray       |
| void RemoveAtSwap(SizeType Index, CountType Count, bool bAllowShrinking = true) | 删除给定索引处任意数量元素，将最后的元素填充至此，默认会收缩TArray |
| void Reset(SizeType NewSize = 0)                             | 析构所有元素，将元素数量置零，若传入大小大于当前最大元素数量则扩容TArray，反之不变 |
| void Empty(SizeType Slack = 0)                               | 析构所有元素，将元素数量置零，改变容器大小到输入值           |
| SizeType RemoveSingle(const ElementType& Item)               | 删除并析构第一个匹配元素，维持TArray顺序，返回删除的元素数量 |
| SizeType Remove(const ElementType& Item)                     | 删除并析构所有匹配元素，维持TArray顺序，返回删除的元素数量   |
| SizeType RemoveAll(const PREDICATE_CLASS& Predicate)         | 删除并析构所有满足谓词的元素，维持TArray顺序，返回删除的元素数量 |
| SizeType RemoveAllSwap(const PREDICATE_CLASS& Predicate, bool bAllowShrinking = true) | 删除并析构所有满足谓词的元素，高效但不维持TArray顺序，默认收缩容器 |
| SizeType RemoveSingleSwap(const ElementType& Item, bool bAllowShrinking = true) | 删除并析构第一个匹配元素，高效但不维持TArray顺序，返回删除的元素数量，默认收缩容器 |
| SizeType RemoveSwap(const ElementType& Item, bool bAllowShrinking = true) | 删除并析构所有匹配元素，高效但不维持TArray顺序，返回删除的元素数量，默认收缩容器 |
|                                                              |                                                              |
| 交换/Swap                                                    |                                                              |
| void SwapMemory(SizeType FirstIndexToSwap, SizeType SecondIndexToSwap) | 交换索引处的两块内存(不检查索引是否有效)                     |
| void Swap(SizeType FirstIndexToSwap, SizeType SecondIndexToSwap) | 交换索引处的两个元素                                         |

#### 容器操作
| 方法                                                         | 作用                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| void Shrink()                                                | 收缩TArray，使其仅容纳ArrayNum个元素                         |
| void BulkSerialize(FArchive& Ar, bool bForcePerElementSerialization = false) | 从FArchive反序列化到当前TArray                               |
| void CountBytes(FArchive& Ar) const                          | 计算序列化当前TArray所需要的字节大小                         |
| void SetNum(SizeType NewNum, bool bAllowShrinking = true)    | 设置元素数量，若大于当前数量，则默认构造新元素，若小于当前数量，则析构多余的元素，默认收缩TArray |
| void SetNumZeroed(SizeType NewNum, bool bAllowShrinking = true) | 设置元素数量，若大于当前数量，则0初始化新元素，若小于当前数量，则析构多余的元素，默认收缩TArray |
| void SetNumUninitialized(SizeType NewNum, bool bAllowShrinking = true) | 设置元素数量，若大于当前数量，则添加新元素但不初始化，若小于当前数量，则析构多余的元素，默认收缩TArray |
| void SetNumUnsafeInternal(SizeType NewNum)                   | 仅在数组中设置新的元素数，新数量要小于当前元素数。不析构多余的旧元素，不收缩内存。 |
| void Reserve(SizeType Number)                                | 保留内存，使仅其容纳所有元素且大于给定大小                   |
| void Init(const ElementType& Element, SizeType Number)       | 设置容器为给定大小，并用指定元素填充                         |
| ElementAllocatorType& GetAllocatorInstance()                 | 获取TArray的内存分配器                                       |
| const ElementAllocatorType& GetAllocatorInstance() const     | 获取TArray的内存分配器的const版本                            |

#### 获取迭代器

| 方法                                       | 作用                                                |
| ------------------------------------------ | --------------------------------------------------- |
| TIterator CreateIterator()                 | 返回一个当前容器的多功能迭代器，指向首元素          |
| TConstIterator CreateConstIterator() const | 返回一个当前容器的const多功能迭代器，指向首元素     |
| RangedForIteratorType begin()              | 返回一个当前容器的Foreach迭代器，指向首元素         |
| RangedForConstIteratorType begin() const   | 返回一个当前容器的const Foreach迭代器，指向首元素   |
| RangedForIteratorType end  ()              | 返回一个当前容器的Foreach迭代器，指向尾后元素       |
| RangedForConstIteratorType end  () const   | 返回一个当前容器的const Foreach迭代器，指向尾后元素 |

#### 其它算法

| 方法                                              | 作用                                                         |
| ------------------------------------------------- | ------------------------------------------------------------ |
| void Sort()                                       | 使用<符号排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用 |
| void Sort(const PREDICATE_CLASS& Predicate)       | 按给定谓词排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用 |
| void StableSort()                                 | 使用<符号稳定排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用，对相同的元素不改变元素顺序 |
| void StableSort(const PREDICATE_CLASS& Predicate) | 按给定谓词稳定排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用，对相同的元素不改变元素顺序 |
| void HeapSort()                                   | 对数组进行堆排序，使用'<'排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用，对相同的元素不改变元素顺序 |
| void HeapSort(const PREDICATE_CLASS& Predicate)   | 对数组进行堆排序，按给定谓词排序，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用，对相同的元素不改变元素顺序 |

#### 转化为堆及操作

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| void Heapify()                                               | 使用<符号，将TArray排序为堆，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用 |
| void Heapify(const PREDICATE_CLASS& Predicate)               | 按给定谓词，将TArray排序为堆，若成员是指针类型，则自动解引用后再排序，智能指针不会取消引用 |
| SizeType HeapPush(const ElementType& InItem, const PREDICATE_CLASS& Predicate) | 按堆排序方式，传入对应谓词，插入元素，返回元素索引           |
| SizeType HeapPush(ElementType&& InItem, const PREDICATE_CLASS& Predicate) | 上一函数的右值版本                                           |
| SizeType HeapPush(const ElementType& InItem)                 | 按默认'<'堆排序方式，插入元素，返回元素索引                  |
| SizeType HeapPush(ElementType&& InItem)                      | 上一函数的右值版本                                           |
| void HeapPop(ElementType& OutItem, const PREDICATE_CLASS& Predicate, bool bAllowShrinking = true) | 返回并弹出堆顶元素，传入对应谓词，默认收缩容器               |
| void HeapPop(ElementType& OutItem, bool bAllowShrinking = true) | 返回并弹出堆顶元素，按默认'<'堆排序方式，默认收缩容器        |
| void VerifyHeap(const PREDICATE_CLASS& Predicate)            | 检查容器是否为堆                                             |
| void HeapPopDiscard(const PREDICATE_CLASS& Predicate, bool bAllowShrinking = true) | 弃置并弹出堆顶元素，传入对应谓词，默认收缩容器               |
| void HeapPopDiscard(bool bAllowShrinking = true)             | 弃置并弹出堆顶元素，按默认'<'堆排序方式，默认收缩容器        |
| ElementType& HeapTop()                                       | 返回堆顶元素                                                 |
| const ElementType& HeapTop() const                           | 上一函数const版本                                            |
| void HeapRemoveAt(SizeType Index, const PREDICATE_CLASS& Predicate, bool bAllowShrinking = true) | 删除给定索引位置的元素，传入对应谓词，默认收缩容器           |
| void HeapRemoveAt(SizeType Index, bool bAllowShrinking = true) | 删除给定索引位置的元素，按默认'<'堆排序方式，默认收缩容器    |


#### 冻结内存图

需要内存分配器，以及元素类型的支持

TAllocatorTraits<Allocator>::SupportsFreezeMemoryImage为真

	仅仅Allocator为TSparseArrayAllocator或TSetAllocator时SupportsFreezeMemoryImage才支持

当元素类型声明了StaticGetTypeLayout函数(通过特定的宏声明，见MemoryLayout.h)

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| void WriteMemoryImage(FMemoryImageWriter& Writer) const      | 将TArray写入FMemoryImageWriter，若TArray不支持则写入空TArray |
| void CopyUnfrozen(const FMemoryUnfreezeContent& Context, void* Dst) const |                                                              |
| static void AppendHash(const FPlatformTypeLayoutParameters& LayoutParams, FSHA1& Hasher) |                                                              |
| void ToString(const FPlatformTypeLayoutParameters& LayoutParams, FMemoryToStringContext& OutContext) const |                                                              |

### 迭代器

#### 多功能迭代器 TIndexedContainerIterator

| 方法                                                         | 作用                                                 |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| TIndexedContainerIterator(ContainerType& InContainer, SizeType StartIndex = 0) | 构造一个迭代器并指向指定索引                         |
| TIndexedContainerIterator& operator++()                      | 前置++，迭代器移动                                   |
| TIndexedContainerIterator operator++(int)                    | 后置++，迭代器移动                                   |
| TIndexedContainerIterator& operator--()                      | 前置--，迭代器移动                                   |
| TIndexedContainerIterator operator--(int)                    | 后置--，迭代器移动                                   |
| TIndexedContainerIterator& operator+=(SizeType Offset)       | 迭代器随机移动                                       |
| TIndexedContainerIterator operator+(SizeType Offset) const   | 迭代器随机移动                                       |
| TIndexedContainerIterator& operator-=(SizeType Offset)       | 迭代器随机移动                                       |
| TIndexedContainerIterator operator-(SizeType Offset) const   | 迭代器随机移动                                       |
| ElementType& operator* () const                              | 解引用，返回指向元素的引用                           |
| ElementType* operator->() const                              | 使迭代器可当作指向元素的指针使用                     |
| explicit operator bool() const                               | 显式类型转换为bool，迭代器指向有效则为真             |
| SizeType GetIndex() const                                    | 返回指向元素的索引                                   |
| void Reset()                                                 | 使迭代器指向首元素                                   |
| void SetToEnd()                                              | 使迭代器指向尾后元素                                 |
| void RemoveCurrent()                                         | 移除指向的元素                                       |
| friend bool operator==(const TIndexedContainerIterator& Lhs, const TIndexedContainerIterator& Rhs) | 判断两个迭代器是否相等                               |
| friend bool operator!=(const TIndexedContainerIterator& Lhs, const TIndexedContainerIterator& Rhs) | 判断两个迭代器是否不等                               |
| TIndexedContainerIterator<ContainerType, ElementType, SizeType> operator+(SizeType Offset, TIndexedContainerIterator<ContainerType, ElementType, SizeType> RHS) | 非成员函数，迭代器随机移动，但是使用方法为int+迭代器 |

#### 用于foreach的迭代器TCheckedPointerIterator

| 方法                                                         | 作用                       |
| ------------------------------------------------------------ | -------------------------- |
| TCheckedPointerIterator(const SizeType& InNum, ElementType* InPtr) | 构造一个迭代器             |
| ElementType& operator*() const                               | 解引用，返回指向元素的引用 |
| TCheckedPointerIterator& operator++()                        | 前置++，迭代器移动         |
| TCheckedPointerIterator& operator--()                        | 前置--，迭代器移动         |
| friend bool operator!=(const TCheckedPointerIterator& Lhs, const TCheckedPointerIterator& Rhs) | 检查两个迭代器是否相等     |

## TBitArray

template<typename Allocator>
class TBitArray

模板参数为内存分配器，需要内存连续

默认内存分配器为TInlineAllocator<4>

提供的方法较为晦涩，这里除了最基本的函数外，只记录我看到被使用的函数或迭代器

### 容器优势与内存布局

**容器优势**

对于bool元素的连续存储有很大的优化，容器操作中大量使用位运算来提高效率

针对UE4内部使用到该容器的地方，提供了一系列对应的高效存取与查找操作，或对应的迭代器操作

**内存布局：**

使用一个内联内存分配器，按位存储bool信息

### 公开成员与方法

#### 构造函数/析构函数

| 方法                                              | 作用                                |
| ------------------------------------------------- | ----------------------------------- |
| TBitArray()                                       | 默认构造                            |
| TBitArray(bool bValue, int32 InNumBits)           | 任意个给定值进行初始化              |
| TBitArray(const TBitArray& Copy)                  | 同类型另一个TBitArray进行初始化     |
| TBitArray(TBitArray&& Other)                      | 上一函数右值类型                    |
| TBitArray(const TBitArray<OtherAllocator> & Copy) | 不同类型的另一个TBitArray进行初始化 |

#### 操作符重载

| 方法                                                         | 作用                                  |
| ------------------------------------------------------------ | ------------------------------------- |
| TBitArray& operator=(const TBitArray& Copy)                  | 同类型另一个TBitArray进行赋值         |
| TBitArray& operator=(TBitArray&& Other)                      | 上一函数右值版本                      |
| TBitArray& operator=(const TBitArray< OtherAllocator >& Copy) | 不同类型另一个TBitArray进行赋值       |
| bool operator==(const TBitArray<Allocator>& Other) const     | 判断与另一任意类型TBitArray是否相等   |
| bool operator<(const TBitArray<Allocator>& Other) const      | 先比较长度，后比较大小(仅0，1比较)    |
| bool operator!=(const TBitArray<Allocator>& Other) const     | 判断与另一任意类型TBitArray是否不相等 |
| friend FArchive& operator<<(FArchive& Ar, TBitArray& BitArray) | 序列化到FArchive                      |

#### 查询

| 方法                                                         | 作用                                            |
| ------------------------------------------------------------ | ----------------------------------------------- |
| int32 FindAndSetFirstZeroBit(int32 ConservativeStartIndex = 0) | 查找数组中从给定位置开始第一个'0'位，返回其索引 |

#### 插入

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| int32 Add(const bool Value)                                  | 向最后添加一个元素                                           |
| int32 Add(const bool Value, int32 NumBitsToAdd)              | 向最后添加任意个相同元素                                     |
| int32 AddRange(const InWordType* ReadBits, int32 NumBitsToAdd, int32 ReadOffsetBits = 0) | 向后添加任意个元素，数据来源于给定指针的给定偏移处           |
| int32 AddRange(const TBitArray<OtherAllocator>& ReadBits, int32 NumBitsToAdd, int32 ReadOffsetBits = 0) | 向后添加任意个元素，数据从给定TBitArray的给定索引处开始      |
| int32 AddUninitialized(int32 NumBitsToAdd)                   | 添加任意个未初始化元素                                       |
| void Insert(bool Value, int32 Index)                         | 向给定索引处插入一个元素                                     |
| void Insert(bool Value, int32 Index, int32 NumBitsToAdd)     | 向给定索引处插入任意个元素                                   |
| void InsertRange(const InWordType* ReadBits, int32 Index, int32 NumBitsToAdd, int32 ReadOffsetBits = 0) | 向给定索引处插入任意元素，数据来源于给定指针的给定偏移处     |
| void InsertRange(const TBitArray<OtherAllocator>& ReadBits, int32 Index, int32 NumBitsToAdd, int32 ReadOffsetBits = 0) | 向给定索引处插入任意元素，数据从给定TBitArray的给定索引处开始 |
| void InsertUninitialized(int32 Index, int32 NumBitsToAdd)    | 向给定索引处插入任意未初始化元素                             |
| void Init(bool bValue, int32 InNumBits)                      | 使用任意数量给定值初始化容器                                 |
| void SetRange(int32 Index, int32 NumBitsToSet, bool Value)   | 在给定索引处，将连续任意元素设为给定值                       |

#### 删除

| 方法                                  | 作用                         |
| ------------------------------------- | ---------------------------- |
| void Empty(int32 ExpectedNumBits = 0) | 清除所有元素，并预设容器大小 |
| void Reset()                          | 移除所有元素，保留空间       |

#### 容器操作

| 方法                                      | 作用                                       |
| ----------------------------------------- | ------------------------------------------ |
| void CheckInvariants() const              | 检查容器不变量(检查容器是否合法)           |
| void Reserve(int32 Number)                | 保留内存，使仅其容纳所有元素且大于给定大小 |
| void SetNumUninitialized(int32 InNumBits) | 设置容器元素数，不初始化                   |

#### 友元

| 友元类                                 |      |
| -------------------------------------- | ---- |
| friend TScriptBitArray                 |      |
| friend class TConstSetBitIterator;     |      |
| friend class TConstDualSetBitIterator; |      |

### 迭代器

#### TConstSetBitIterator

template<typename Allocator>
class TConstSetBitIterator : public FRelativeBitReference

用于遍历TBitArray中所有的设置为true的元素

## TSparseArray

template<typename InElementType,typename Allocator>
class TSparseArray

模板参数分别为模板类型，分配器类型

默认使用TSparseArrayAllocator分配器

### 容器优势与内存布局

**容器优势**

常数时间的删除任意元素
常数时间的添加元素，不保证元素位置
高效的遍历元素，元素容器为连续存储，不必跳转地址

注意：由于容器元素不连续，需要使用迭代器来访问元素，operator[]访问的元素不可靠

**内存布局**

使用一个TArray记录所有元素(包含已初始化与未初始化的元素)

使用一个链表记录容器中所有空隙(未初始化的元素)，链表是一个union类型，存在于空隙所在的内存地址上，不占用额外空间

使用一个TBitArray记录TArray中的元素是否已被初始化

删除元素时直接添加空隙，添加元素时优先使用空隙

### 公开成员与方法

#### 构造函数/析构函数

| 方法                                     | 作用                       |
| ---------------------------------------- | -------------------------- |
| TSparseArray()                           | 默认构造                   |
| TSparseArray(const TSparseArray& InCopy) | 从另一TSparseArray拷贝构造 |
| TSparseArray(TSparseArray&& InCopy)      | 上一函数右值版本           |
| ~TSparseArray()                          | 析构函数                   |

#### 操作符重载

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| TSparseArray& operator=(const TSparseArray& InCopy)          | 清空当前容器，使用TSparseArray赋值，根据元素是否具有拷贝构造函数，确定是直接对内存赋值或者是对分别每一个元素使用拷贝构造函数 |
| TSparseArray& operator=(TSparseArray&& InCopy)               | 使用右值版本的TSparseArray赋值，根据容器是否支持移动赋值，确定是直接执行内容器内的元素赋值，或者调用普通的赋值方法(上一函数) |
| friend FArchive& operator<<(FArchive& Ar,TSparseArray& Array) | 友元函数，序列化操作                                         |
| friend void operator<<(FStructuredArchive::FSlot Slot, TSparseArray& InArray) | 友元函数，另一种结构的序列化操作                             |
| friend bool operator==(const TSparseArray& A,const TSparseArray& B) | 友元函数，检查两个TSparseArray是否有相同的元素和元素对应的索引 |
| friend bool operator!=(const TSparseArray& A,const TSparseArray& B) | 友元函数，上一函数取反                                       |
| ElementType& operator[](int32 Index)                         | 根据索引，获取元素                                           |
| const ElementType& operator[](int32 Index) const             | 上一函数的const版本                                          |
| TSparseArray& operator+=( const TSparseArray& OtherArray )   | 向当前容器中追加TSparseArray的元素                           |
| TSparseArray& operator+=( const TArray<ElementType>& OtherArray ) | 向当前容器中追加TArray的元素                                 |
| template <typename T, typename Allocator> void* operator new( size_t Size, TSparseArray<T, Allocator>& Array ) | 非成员函数，在容器的最后由new构造一个元素                    |
| template <typename T, typename Allocator> void* operator new( size_t Size, TSparseArray<T, Allocator>& Array, int32 Index ) | 非成员函数，在容器的指定索引处由new构造一个元素              |

#### 查询

| 方法                                                         | 作用                                         |
| ------------------------------------------------------------ | -------------------------------------------- |
| uint32 GetAllocatedSize( void ) const                        | 返回当前容器分配的所有内存大小               |
| bool IsCompact() const                                       | 返回容器是否是紧凑的(无中间空隙)             |
| int32 PointerToIndex(const ElementType* Ptr) const           | 根据元素的指针，获取对应的索引               |
| bool IsValidIndex(int32 Index) const                         | 检查索引处的元素，索引有效且元素存在则返回真 |
| bool IsAllocated(int32 Index) const { return AllocationFlags[Index]; } | 检查索引处是否存在元素，不检查索引是否有效   |
| int32 GetMaxIndex() const { return Data.Num(); }             | 获取容器最大索引                             |
| int32 Num() const { return Data.Num() - NumFreeIndices; }    | 获取容器内元素数量                           |
| void CheckAddress(const ElementType* Addr) const             | 检查元素地址是否存在于容器内                 |
| FElementOrFreeListLink& GetData(int32 Index)                 | 返回索引处对应的元素信息                     |
| const FElementOrFreeListLink& GetData(int32 Index) const     | 上一函数const版本                            |

#### 插入

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| FSparseArrayAllocationInfo AddUninitialized()                | 添加一个未初始化元素，返回该节点对应的信息，优先添加到容器的空隙中 |
| int32 Add(const ElementType& Element)                        | 添加一个元素，返回对应的索引                                 |
| int32 Add(ElementType&& Element)                             | 上一函数右值版本                                             |
| FSparseArrayAllocationInfo AddUninitializedAtLowestFreeIndex(int32& LowestFreeIndexSearchStart) | 若存在空隙则添加一个未初始化元素到给定位置开始最低位的空隙，修改传入引用为空隙的下一元素索引，不存在则添加到容器最后。返回节点对应的信息 |
| int32 AddAtLowestFreeIndex(const ElementType& Element, int32& LowestFreeIndexSearchStart) | 同上一函数，但使用给定值进行初始化，并返回其对应索引         |
| int32 EmplaceAtLowestFreeIndex(int32& LowestFreeIndexSearchStart, ArgsType&&... Args) | 同上一函数，但使用给定参数列表进行初始化                     |
| int32 Emplace(ArgsType&&... Args)                            | 使用给定参数列表向容器中添加并构造一个元素                   |
| FSparseArrayAllocationInfo InsertUninitialized(int32 Index)  | 从给定索引处添加一个未初始化元素，返回该节点对应的信息，要求给定索引处本来不存在元素(当索引大于容器大小，则对容器扩容) |
| void Insert(int32 Index,typename TTypeTraits<ElementType>::ConstInitType Element) | 在指定索引处插入一个元素，要求给定索引处本来不存在元素(当索引大于容器大小，则对容器扩容) |

#### 删除

| 方法                                                    | 作用                                                         |
| ------------------------------------------------------- | ------------------------------------------------------------ |
| void RemoveAtUninitialized(int32 Index,int32 Count = 1) | 删除从给定索引处连续任意个元素(不调用析构函数)，要求这些元素均已被分配(其中不能有空隙) |
| void RemoveAt(int32 Index,int32 Count = 1)              | 删除从给定索引处连续任意个元素(调用析构函数)，要求这些元素均已被分配(其中不能有空隙) |
| void Empty(int32 ExpectedNumElements = 0)               | 删除并析构所有元素，同时预分配容器空间                       |
| void Reset()                                            | 删除并析构所有元素，保留空间                                 |

#### 容器操作

| 方法                                    | 作用                                                         |
| --------------------------------------- | ------------------------------------------------------------ |
| void Reserve(int32 ExpectedNumElements) | 保留内存，使仅其容纳所有元素且大于给定大小                   |
| void Shrink()                           | 收缩容器，释放最后一个有效元素之后的所有内存，不改变中间空隙 |
| bool Compact()                          | 紧凑内存，将容器后方的元素移动到元素间的空隙中，填满空隙后，释放后方多余的内存，返回是否有元素被移动 |
| bool CompactStable()                    | 紧凑内存，只改变元素位置但不改变元素存储的顺序，释放多余的内存，返回是否有元素被移动 |
| void CountBytes(FArchive& Ar) const     | 通过存档跟踪容器的内存使用情况                               |

#### 获取迭代器

| 方法                                       | 作用                                            |
| ------------------------------------------ | ----------------------------------------------- |
| TIterator CreateIterator()                 | 创建一个普通迭代器                              |
| TConstIterator CreateConstIterator() const | 创建一个const版本的普通迭代器                   |
| TRangedForIterator      begin()            | 创建一个范围迭代器，指向容器第一个有效元素      |
| TRangedForConstIterator begin() const      | 创建一个const范围迭代器，指向容器第一个有效元素 |
| TRangedForIterator      end  ()            | 创建一个范围迭代器，指向容器尾后的元素          |
| TRangedForConstIterator end  () const      | 创建一个const范围迭代器，指向容器尾后的元素     |

#### 其它算法

| 方法                                              | 作用                                 |
| ------------------------------------------------- | ------------------------------------ |
| void Sort( const PREDICATE_CLASS& Predicate )     | 按给定谓词排序，空隙将全部被释放     |
| void Sort()                                       | 按'<'排序，空隙将全部被释放          |
| void StableSort(const PREDICATE_CLASS& Predicate) | 按给定谓词稳定排序，空隙将全部被释放 |
| void StableSort()                                 | 按'<'稳定排序，空隙将全部被释放      |

### 私有成员与方法

| 成员            | 作用                                                         |
| --------------- | ------------------------------------------------------------ |
| Data            | 存储数据元素本身的容器                                       |
| AllocationFlags | 存储Data容器状态的容器                                       |
| FirstFreeIndex  | 未初始化元素节点链表首元素索引，添加或使用元素，均在表头更改 |
| NumFreeIndices  | 未初始化元素节点数量                                         |

### 外部结构体

**FSparseArrayAllocationInfo**

记录TSparseArray内的指定索引处所对应的元素指针

void* operator new(size_t Size,const FSparseArrayAllocationInfo& Allocation)//在重载New

### 迭代器

如下迭代器均在类的内部定义，避免污染命名空间

#### TBaseIterator

template<bool bConst>
class TBaseIterator

提供遍历TSparseArray内所有有效元素的方法，根据TConstSetBitIterator迭代器来获取有效元素的索引，然后转换为TSparseArray的对应索引的元素进行输出

| 类型typedef                                                  | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| typedef TConstSetBitIterator<typename Allocator::BitArrayAllocator> BitArrayItType | 用于遍历AllocationFlags中值为true的所有元素的迭代器          |
| typedef typename TChooseClass<bConst,const TSparseArray,TSparseArray>::Result ArrayType | 根据传入的bool值，ArrayType将表示为const TSparseArray或TSparseArray |
| typedef typename TChooseClass<bConst,const ElementType,ElementType>::Result ItElementType | 根据传入的bool值，ItElementType将表示为const ElementType或ElementType |

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| TBaseIterator(ArrayType& InArray, const BitArrayItType& InBitArrayIt) | 构造函数，传入ArrayType类型的容器与BitArrayItType类型的迭代器 |
| TBaseIterator& operator++()                                  | 迭代器向后移动，指向下一个有效元素                           |
| int32 GetIndex() const                                       | 返回迭代器指向元素对应的索引                                 |
| friend bool operator==                                       | 判断两个迭代器是否指向相同容器的相同元素                     |
| friend bool operator!=                                       | 上一函数取反                                                 |
| operator bool() const                                        | 判断迭代器指向元素是否存在于容器中(迭代器是否有效)           |
| bool operator !() const                                      | 上一函数取反                                                 |
| ItElementType& operator*() const                             | 返回指向的元素引用                                           |
| ItElementType* operator->()                                  | 返回指向的元素指针                                           |

#### TIterator

class TIterator : public TBaseIterator<false>

继承自迭代器基类，实例化容器与元素类型为非const类型，简化了迭代器构造函数，提供了一个删除函数

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| TIterator(TSparseArray& InArray)                             | 构造函数，只用传入TSparseArray容器，TConstSetBitIterator迭代器使用AllocationFlags来初始化 |
| TIterator(TSparseArray& InArray, const typename TBaseIterator<false>::BitArrayItType& InBitArrayIt) | 构造函数，同时传入TSparseArray容器与TConstSetBitIterator迭代器 |
| void RemoveCurrent()                                         | 从容器中移除当前指向元素，此时迭代器的指向已不可使用，但任可以移动迭代器 |

#### TConstIterator

class TConstIterator : public TBaseIterator<true>

继承自迭代器基类，实例化容器与元素类型为const类型，简化了迭代器构造函数

提供两个构造函数，类似TIterator，但为const类型

#### TRangedForIterator

class TRangedForIterator : public TIterator

相比父类，不再支持简化的构造函数(便于直接使用Begin(),End()函数)，重载了bool operator!=以用来检查容器大小是否被更改

#### TRangedForConstIterator 

class TRangedForConstIterator : public TConstIterator

类似于TRangedForIterator，但是迭代器所指向的元素将为const类型

## TSet

template<typename InElementType, typename KeyFuncs, typename Allocator>
class TSet

模板参数分别为元素类型，元素到Key的映射方法，内存分配器

### 容器优势与内存布局

**容器优势**

实现了元素的常数级增删改查；元素插入无排序保证；可自定义是否允许重复元素，默认不允许；

**内存布局**

容器由一个哈希桶构成：使用一块空间保存每个桶节点的索引，每个桶节点内部使用链表存储与扩增元素。

其中链表节点为TSetElement类型，桶节点的索引为FSetElementId类型。

使用TSparseArray容器保存所有的链表节点，使用内联内存分配器保存桶索引。

注意：哈希值是由元素的Key来计算得到，默认情况Key等于元素本身，可由第二个模板参数重载。桶的选择由哈希值取模得到

### 公开成员与方法

#### 构造函数/析构函数

| 方法                                                         | 作用                     |
| ------------------------------------------------------------ | ------------------------ |
| TSet()                                                       | 默认构造                 |
| TSet(const TSet& Copy)                                       | 拷贝构造                 |
| TSet(TSet&& Other)                                           | 移动构造                 |
| TSet(const TSet<ElementType, KeyFuncs, OtherAllocator>& Other) | 从不同类型的TSet拷贝构造 |
| TSet(TSet<ElementType, KeyFuncs, OtherAllocator>&& Other)    | 上一函数右值版本         |
| TSet(const TArray<ElementType>& InArray)                     | 从TArray构造             |
| TSet(TArray<ElementType>&& InArray)                          | 上一函数右值版本         |
| TSet(std::initializer_list<ElementType> InitList)            | 初始化列表构造           |
| ~TSet()                                                      | 析构函数                 |

#### 操作符重载

| 方法                                                         | 作用                          |
| ------------------------------------------------------------ | ----------------------------- |
| TSet& operator=(const TSet& Copy)                            | 拷贝赋值                      |
| TSet& operator=(TSet&& Other)                                | 移动赋值                      |
| TSet& operator=(TSet<ElementType, KeyFuncs, OtherAllocator>& Other) | 不同类型TSet移动赋值          |
| TSet& operator=(TSet<ElementType, KeyFuncs, OtherAllocator>&& Other) | 不同类型TSet拷贝赋值          |
| TSet& operator=(std::initializer_list<ElementType> InitList) | 初始化列表赋值                |
| ElementType& operator[](FSetElementId Id)                    | 传入元素ID/下标，返回对应元素 |
| const ElementType& operator[](FSetElementId Id) const        | 上一函数const版本             |
| friend FArchive& operator<<(FArchive& Ar,TSet& Set)          | 序列化                        |
| friend void operator<<(FStructuredArchive::FSlot Slot, TSet& Set) | 序列化                        |

#### 查询

| 方法                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| uint32 GetAllocatedSize( void ) const                        | 返回当前容器分配的所有内存大小                               |
| void CountBytes(FArchive& Ar) const                          | 跟踪内存使用情况                                             |
| int32 Num() const                                            | 返回元素数量                                                 |
| int32 GetMaxIndex() const                                    | 获取最大元素索引                                             |
| bool IsValidId(FSetElementId Id) const                       | 检测索引是否有效                                             |
| FSetElementId FindId(KeyInitType Key) const                  | 查找第一个匹配Key的元素，返回索引                            |
| FSetElementId FindIdByHash(uint32 KeyHash, const ComparableKey& Key) const | 查找第一个匹配Key的元素，返回索引，提供Key的Hash来加快匹配   |
| ElementType* Find(KeyInitType Key)                           | 查找第一个匹配Key的元素，返回元素指针                        |
| const ElementType* Find(KeyInitType Key) const               | 上一函数const版本                                            |
| ElementType* FindByHash(uint32 KeyHash, const ComparableKey& Key) | 查找第一个匹配Key的元素，返回元素指针，提供Key的Hash来加快匹配 |
| const ElementType* FindByHash(uint32 KeyHash, const ComparableKey& Key) const | 上一函数const版本                                            |
| bool Contains(KeyInitType Key) const                         | 检测是否存在匹配Key的元素                                    |
| bool ContainsByHash(uint32 KeyHash, const ComparableKey& Key) const | 检测是否存在匹配Key的元素，提供Key的Hash来加快匹配           |
| void CheckAddress(const ElementType* Addr) const             | 检查元素地址是否存在于容器内                                 |

#### 插入

| 方法                                                         | 作用                                             |
| ------------------------------------------------------------ | ------------------------------------------------ |
| FSetElementId Add(const InElementType&  InElement, bool* bIsAlreadyInSetPtr = nullptr) | 插入一个元素，返回该元素是否被重复插入           |
| FSetElementId Add(      InElementType&& InElement, bool* bIsAlreadyInSetPtr = nullptr) | 上一函数移动版本                                 |
| FSetElementId AddByHash(uint32 KeyHash, const InElementType& InElement, bool* bIsAlreadyInSetPtr = nullptr) | 插入一个已知hash的元素，返回该元素是否被重复插入 |
| FSetElementId AddByHash(uint32 KeyHash,		 InElementType&& InElement, bool* bIsAlreadyInSetPtr = nullptr) | 上一函数移动版本                                 |
| Emplace(ArgsType&& Args, bool* bIsAlreadyInSetPtr = nullptr) | 同Add()                                          |
| FSetElementId EmplaceByHash(uint32 KeyHash, ArgsType&& Args, bool* bIsAlreadyInSetPtr = nullptr) | 同AddByHash()                                    |
| void Append(const TArray<ElementType, ArrayAllocator>& InElements) | 插入TArray内的所有元素                           |
| void Append(TArray<ElementType, ArrayAllocator>&& InElements) | 上一函数移动版本                                 |
| void Append(const TSet<ElementType, KeyFuncs, OtherAllocator>& OtherSet) | 插入另一个TArray内的所有元素                     |
| void Append(TSet<ElementType, KeyFuncs, OtherAllocator>&& OtherSet) | 上一函数移动版本                                 |
| void Append(std::initializer_list<ElementType> InitList)     | 插入初始化列表的所有元素                         |

#### 删除

| 方法                                                         | 作用                                           |
| ------------------------------------------------------------ | ---------------------------------------------- |
| void Remove(FSetElementId ElementId)                         | 删除给定索引处的元素                           |
| int32 Remove(KeyInitType Key)                                | 删除满足Key的所有元素                          |
| int32 RemoveByHash(uint32 KeyHash, const ComparableKey& Key) | 删除等于Key的所有元素，提供Key的Hash来加快匹配 |

#### 容器操作

| 方法                                                    | 作用                                     |
| ------------------------------------------------------- | ---------------------------------------- |
| void Empty(int32 ExpectedNumElements = 0)               | 清空容器所有元素，调整容器大小为给定大小 |
| void Reset()                                            | 之间清空桶索引，不析构元素               |
| void Shrink()                                           | 收缩容器                                 |
| void Compact()                                          | 收紧元素，收紧TSpaceArray并Rehash()      |
| void CompactStable()                                    | 收紧元素，不改变元素顺序，并Rehash()     |
| void Reserve(int32 Number)                              | 预分配容器大小                           |
| void Relax()                                            | 放缩哈希表，使每个桶平均一个元素         |
| void Dump(FOutputDevice& Ar)                            | 输出容器的描述信息                       |
| bool VerifyHashElementsKey(KeyInitType Key)             | 检验匹配Key的桶中所有元素的有效性        |
| void DumpHashElements(FOutputDevice& Ar)                | 序列化相关                               |
| bool LegacyCompareEqual(const TSet& A,const TSet& B)    | 检验两个TSet的TSparseArray是否相同       |
| bool LegacyCompareNotEqual(const TSet& A,const TSet& B) | 上一函数取反                             |
| TArray<ElementType> Array() const                       | 转化为TArray                             |

#### 获取迭代器

| 方法                                       | 作用                                            |
| ------------------------------------------ | ----------------------------------------------- |
| TIterator CreateIterator()                 | 返回用于一个遍历所有元素的迭代器                |
| TConstIterator CreateConstIterator() const | 上一函数const版本                               |
| TRangedForIterator      begin()            | 创建一个范围迭代器，指向容器第一个有效元素      |
| TRangedForConstIterator begin() const      | 创建一个const范围迭代器，指向容器第一个有效元素 |
| TRangedForIterator      end()              | 创建一个范围迭代器，指向容器尾后的元素          |
| TRangedForConstIterator end() const        | 创建一个const范围迭代器，指向容器尾后的元素     |

#### 其它算法

| 方法                                                         | 作用                                                        |
| ------------------------------------------------------------ | ----------------------------------------------------------- |
| void Sort( const PREDICATE_CLASS& Predicate )                | 按给定方法对元素排序                                        |
| void StableSort(const PREDICATE_CLASS& Predicate)            | 按给定方法对元素稳定排序                                    |
| TSet Intersect(const TSet& OtherSet) const                   | 返回容器的交集                                              |
| TSet Union(const TSet& OtherSet) const                       | 返回容器的并集                                              |
| TSet Difference(const TSet& OtherSet) const                  | 返回存在与当前容器但不存在与输入容器的元素，this - OtherSet |
| bool Includes(const TSet<ElementType,KeyFuncs,Allocator>& OtherSet) const | 返回输入容器是否是当前容器的子集                            |

### 私有成员

| 成员                                                         | 作用                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| typedef TSparseArray<SetElementType,typename Allocator::SparseArrayAllocator>     ElementArrayType;<br />ElementArrayType Elements | 用于存储TSetElement的稀疏数组<br />TSetElement中存储了元素、当前所在的哈希桶、桶中下一个元素的数组索引 |
| typedef typename Allocator::HashAllocator::template ForElementType<FSetElementId> HashType;<br />HashType Hash | 用于存储元素在Elements中索引的哈希桶                         |
| int32	 HashSize                                           | 用于记录哈希桶的大小                                         |

### 外部类

#### BaseKeyFuncs

template<typename ElementType,typename InKeyType,bool bInAllowDuplicateKeys = false>
struct BaseKeyFuncs

模板参数分别为元素类型，键类型，是否允许出现重复元素(若允许则效率稍高)

功能有两个：

1. 记录是否允许重复元素
2. 类型内存优化：使用模板元编程，根据元素、值的类型大小来类型重定义，使其成为它本来的类型或者它的引用类型

typedef InKeyType KeyType;
typedef typename TCallTraits<InKeyType>::ParamType KeyInitType;
typedef typename TCallTraits<ElementType>::ParamType ElementInitType;

继承自本类型的KeyFuncs需要提供一些方法来操作Key，但由于这些方法是静态函数，所以没有在当前类中进行声明，需要直接在子类中实现，具体哪些函数见DefaultKeyFuncs

##### DefaultKeyFuncs 

template<typename ElementType,bool bInAllowDuplicateKeys>
struct DefaultKeyFuncs : BaseKeyFuncs<ElementType,ElementType,bInAllowDuplicateKeys>

继承自BaseKeyFuncs，模板参数分别为元素类型，是否允许出现重复元素

相比父类，提供了默认的Key类型，Key的类型就是元素的类型

显而易见的是对于大类型，使用默认Key是不合适的

| 操作Key的方法                                         | 作用                                                         |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| static KeyInitType GetSetKey(ElementInitType Element) | 获取这个元素的Key                                            |
| static bool Matches(KeyInitType A, KeyInitType B)     | 比较两个Key是否相同                                          |
| static bool Matches(KeyInitType A, ComparableKey B)   | 比较两个Key是否相同，第二个Key不必与第一个类型相同，但要求可'==' |
| static uint32 GetKeyHash(KeyInitType Key)             | 获取Key对应的Hash值                                          |
| static uint32 GetKeyHash(ComparableKey Key)           | 获取Key对应的Hash值，传入的Key不必与KeyInitType类型相同，但要求可Hash |

#### FSetElementId

class FSetElementId

用于记录元素的ID，只拥有一个int Index成员来记录ID，对这个Index封装了一些方法

| 方法                                                         |                                     |
| ------------------------------------------------------------ | ----------------------------------- |
| FSetElementId()                                              | 默认构造                            |
| bool IsValidId() const                                       | 检验该Index是否被初始化(有效)       |
| int32 AsInteger() const                                      | 返回Index                           |
| FSetElementId(int32 InIndex)                                 | 构造函数，使用参数初始化Index       |
| static FSetElementId FromInteger(int32 Integer)              | 对Index赋值                         |
| static void ResetRange(FSetElementId* Range, int32 Count)    | 默认构造一个连续的FSetElementId数组 |
| friend bool operator==(const FSetElementId& A,const FSetElementId& B) | 判断两个Index是否相同               |
| operator int32()                                             | 强制类型转换                        |

#### TSetElementBase

template<typename InElementType, bool bTypeLayout>
class TSetElementBase

模板参数分别为元素类型，是否拥有类型布局(序列化)

用于存储TSet中的元素的基类，将元素及元素的位置信息封装到一个类中

| 方法                                | 作用                                                     |
| ----------------------------------- | -------------------------------------------------------- |
| TSetElementBase()                   | 默认构造函数                                             |
| TSetElementBase(InitType&& InValue) | 使用元素构造，参数是一个模板类型，所以传入左值或右值均可 |

| 成员                             | 作用                            |
| -------------------------------- | ------------------------------- |
| ElementType Value                | 存储的元素本身                  |
| mutable FSetElementId HashNextId | 同一哈希存储桶中下一个元素的 ID |
| mutable int32 HashIndex          | 元素当前所在的哈希桶            |

##### TSetElementBase重载

template<typename InElementType>
class TSetElementBase<InElementType, true>

该类模板拥有如上所示重载，与标准情况相比，类内成员均使用类型布局相关宏标记，以支持序列化

##### TSetElement

template <typename InElementType>
class TSetElement : public TSetElementBase<InElementType, THasTypeLayout<InElementType>::Value>

类模板参数为元素类型，并自动根据元素类型是否支持类型布局(序列化)，选择对应的基类重载

具体作用见基类，这里只是额外添加了几个函数

| 添加的方法                                                   | 作用                   |
| ------------------------------------------------------------ | ---------------------- |
| friend FArchive& operator<<(FArchive& Ar,TSetElement& Element) | 序列化相关             |
| friend void operator<<(FStructuredArchive::FSlot Slot, TSetElement& Element) | 序列化相关             |
| bool operator==(const TSetElement& Other) const              | 判断存储的元素是否相同 |
| bool operator!=(const TSetElement& Other) const              | 判断存储的元素是否不同 |

### 迭代器

#### 迭代器基类TBaseIterator

template<bool bConst, bool bRangedFor = false>
	class TBaseIterator

模板参数为：是否为const迭代器，是否为范围迭代器

用于遍历TSparseArray内的每一个元素，是对TSparseArray的迭代器的封装

| 方法                                            | 作用                                               |
| ----------------------------------------------- | -------------------------------------------------- |
| TBaseIterator(const ElementItType& InElementIt) | 传入TSparseArray的迭代器，构造一个当前类型的迭代器 |
| TBaseIterator& operator++()                     | 迭代器向后移动，指向下一个有效元素                 |
| explicit operator bool() const                  | 迭代器是否有效                                     |
| bool operator !() const                         | 上一函数取反                                       |
| FSetElementId GetId() const                     | 返回迭代器指向元素的ID                             |
| ItElementType* operator->() const               | 返回指向的元素指针                                 |
| ItElementType& operator*() const                | 返回指向的元素引用                                 |
| bool operator==                                 | 判断两个迭代器是否指向相同容器的相同元素           |
| bool operator!=                                 | 上一函数取反                                       |

#### TConstIterator 

class TConstIterator : public TBaseIterator<true>

| TConstIterator(const TSet& InSet) | 构造函数，返回容器的首元素迭代器 |
| --------------------------------- | -------------------------------- |

##### TIterator

class TIterator : public TBaseIterator<false>

| 方法                   | 作用                             |
| ---------------------- | -------------------------------- |
| TIterator(TSet& InSet) | 构造函数，返回容器的首元素迭代器 |
| void RemoveCurrent()   | 移除迭代器当前指向元素           |

##### TRangedForConstIterator

using TRangedForConstIterator = TBaseIterator<true, true>;

##### TRangedForIterator

using TRangedForIterator      = TBaseIterator<false, true>;

#### Key迭代器基类TBaseKeyIterator

template<bool bConst>
	class TBaseKeyIterator

模板参数为是否是const类型

用于遍历满足给定Key的所有元素

| 方法                                               | 作用                                            |
| -------------------------------------------------- | ----------------------------------------------- |
| TBaseKeyIterator(SetType& InSet,KeyInitType InKey) | 构造函数，传入TSet，Key，返回容器的首元素迭代器 |
| TBaseKeyIterator& operator++()                     | 指向下一个满足条件的元素                        |
| operator bool() const                              | 检验迭代器是否有效                              |
| bool operator !() const                            | 上一函数取反                                    |
| ItElementType* operator->() const                  | 返回指向的元素指针                              |
| ItElementType& operator*() const                   | 返回指向的元素引用                              |

##### TConstKeyIterator

class TConstKeyIterator : public TBaseKeyIterator<true>

对基类的const封装

##### TKeyIterator

class TKeyIterator : public TBaseKeyIterator<false>

对基类的简单封装，添加了删除当前指向元素的操作

## TMapBase

template <typename KeyType, typename ValueType, typename SetAllocator, typename KeyFuncs>
class TMapBase

模板参数分别为：键类型，值类型，TSet的内存分配器，TSet的元素与键的映射关系

看懂TSet后TMap非常简单

### 容器优势与内存布局

TMapBase的本质是一个TSet容器，只是将键与值组合成一个TPair元素，并将该元素作为TSet的元素存储。同时提供了一些函数用来使内部的TSet容器更加具备Map的特点。

### 公开成员与方法

提供的方法基本上是对TSet方法的封装，这些方法不再重复记录，这里介绍几个特有的变化

添加键值对方法(Add或Emplace相关方法)：传入键与值、仅传入键并使用默认值、传入TPair。均返回值的引用来便于修改

查找相关方法(Find)：根据值查找键、根据键查找值

FindOrAdd相关方法：根据键查找值，若未找到则添加一个键值对

迭代器相比于TSet不可直接访问，需要访问其中的Key、Value函数

没有封装sort函数

| 方法                                                         | 作用                         |
| ------------------------------------------------------------ | ---------------------------- |
| int32 GetKeys(TArray<KeyType, Allocator>& OutKeys) const     | 将所有唯一键转换成TArray输出 |
| int32 GetKeys(TSet<KeyType, Allocator>& OutKeys) const       | 将所有键转换成TSet输出       |
| void GenerateKeyArray(TArray<KeyType, Allocator>& OutArray) const | 将所有键转换成TArray输出     |
| void GenerateValueArray(TArray<ValueType, Allocator>& OutArray) const | 将所有值转换成TArray输出     |

### TSortableMapBase

相比父类，额外封装了Sort方法，可实现对键或对值排序

### TMap

相比父类，重载了operator[]函数，使键值对读取更加方便

### TMultiMap

相比父类，可一次返回所有匹配Key的元素，可仅删除一个匹配Key的元素，可查找键与值均相同的元素

### 外部类

#### TKeyInitializer

template <typename KeyInitType, typename ValueInitType>
class TPairInitializer

模板参数为键类型与值类型

仅提供一个构造函数与一个类型转换函数，作用是将输入的键与值转换为TPair类型。

另一个重载如下，作用是将输入的键与默认值转换为TPair类型：

template <typename KeyInitType>
class TKeyInitializer

#### TDefaultMapKeyFuncs 

template<typename KeyType, typename ValueType, bool bInAllowDuplicateKeys>
struct TDefaultMapKeyFuncs : BaseKeyFuncs<TPair<KeyType, ValueType>, KeyType, bInAllowDuplicateKeys>

继承自Set.h中的BaseKeyFuncs类，模板参数分别为：键类型、值类型、是否允许重复

与父类的差异在于将键类型与值类型组合成一个TPair类型，然后将TPair传递给父类作为一个值类型

定义了KeyFuncs所要求的5个函数：

| 操作Key的方法                                         | 作用                                                         |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| static KeyInitType GetSetKey(ElementInitType Element) | 获取这个元素的Key                                            |
| static bool Matches(KeyInitType A, KeyInitType B)     | 比较两个Key是否相同                                          |
| static bool Matches(KeyInitType A, ComparableKey B)   | 比较两个Key是否相同，第二个Key不必与第一个类型相同，但要求可'==' |
| static uint32 GetKeyHash(KeyInitType Key)             | 获取Key对应的Hash值                                          |
| static uint32 GetKeyHash(ComparableKey Key)           | 获取Key对应的Hash值，传入的Key不必与KeyInitType类型相同，但要求可Hash |

###### TDefaultMapHashableKeyFuncs

template<typename KeyType, typename ValueType, bool bInAllowDuplicateKeys>
struct TDefaultMapHashableKeyFuncs : TDefaultMapKeyFuncs<KeyType, ValueType, bInAllowDuplicateKeys>

相比父类，唯一的作用是检查Key是否是可Hash的

# 基础的链式存储容器

## TQueue

### 容器优势

根据模板参数的不同分为单生产者单消费者队列、多生产者单消费者队列。

单生产者单消费者队列：最简单的队列。

多生产者单消费者队列：非常高效的线程安全队列。

### 内存布局

常规的链式存储，插入节点时new，取出节点时delete。

### 访问方法

仅支持向队尾插入元素、从队首查询或取出元素、清空队列

## List

**TList：**最简单的链表节点，由元素与指向下一元素的指针构成，未提供任何操作

**TLinkedListIterator** 拥有单项遍历方法、前后节点查询修改方法的双向链表，未提供其它操作

**TDoubleLinkedList** 支持多种遍历查询修改操作的双向链表。包含链表头尾节点指针、链表大小属性。

# 某些领域专用的容器

### ArrayView

保存的数组的某一处地址，以及从这个地址开始之后的长度。对于一个大型数组，但只对其中一部分进行操作时比较适用。

### HashTable 

为其它容器提供Hash索引的辅助容器

**TStaticHashTable**：静态分配的hash表，为其它数据结构实现索引查找。要求Value不重复(例如递增)，实现任意一个hash健都有唯一一个Value对应，再通过Value作为下标或索引查找另一个容器的元素。通过这种非常简化的流程实现了高效且基本的Hash表查找功能。存在Hash冲突时采用链式存储。

**FHashTable**：功能同TStaticHashTable，但是Hash表本身为动态分配、动态扩容。使用FMemory::Memset与FMemory::Memcpy等方法实现内存分配。

**THashTable**：功能同FHashTable，但是使用自定义的内存分配器。

**UnrealString** 本质就是一个TArray<TChar>，但是封装了很多方便的字符操作方法。


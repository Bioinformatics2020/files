# 读写文件的选项(Flags)

读文件选项：

```cc
FILEREAD_None //空状态，用于函数默认参数
FILEREAD_NoFail //若文件打开失败，不报Fatal
FILEREAD_Silent //若文件打开失败，不报Warning
FILEREAD_AllowWrite //标记为可读且可写
```

写文件选项：

```cc
FILEWRITE_None //空状态，用于函数默认参数
FILEWRITE_NoFail //若文件打开失败，不报Fatal级别错误
FILEWRITE_NoReplaceExisting //目前未发现这个标志在任何地方使用到
FILEWRITE_EvenIfReadOnly //取消文件的只读，并写入文件
FILEWRITE_Append //追加模式
FILEWRITE_AllowRead //标记为可写且可读
FILEWRITE_Silent //关闭文件异常状态的log，如Seek失败、close失败、write失败、FlushBuffer失败
```

以某些名称命名的文件是不可创建的。可以使用FFileHelper::IsFilenameValidForSaving函数来检查文件名称是否有效参考：[命名文件、路径和命名空间 - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/fileio/naming-a-file?redirectedfrom=MSDN)。

# 读写的文件类型

## 文本文件

### UE字符编码

支持的文件字符编码：

​	ASCII、UTF-8、UTF-16。其中ASCII是UTF-8的子集。特别注意UE4并不支持ANSI编码，在UE4中ANSI指的是ASCII

读文本文件时：

​	会自动判断文件为UTF-8或UTF-16，并将其转换为TChar类型。ASCII为UTF-8的子集，不必考虑

写文本文件的编码选项：

```cc
EEncodingOptions::AutoDetect //在ANSI与UTF16编码之间自动选择
EEncodingOptions::ForceAnsi //注意！不是使用ANSI编码而是ASCII编码，遇到无法使用ASCII编码的字符，输出'?'，ANSI编码不存在BOM标志。
EEncodingOptions::ForceUnicode //使用UTF-16编码，注意不是USC-2(定长2字节)编码，带BOM标志
EEncodingOptions::ForceUTF8 //使用UTF-8编码，带BOM标志
EEncodingOptions::ForceUTF8WithoutBOM //使用UTF-8编码，不带BOM标志
```

### 通用文本文件操作

```cc
FFileHelper::SaveStringToFile//保存FString到文件
FFileHelper::LoadFileToString //打开文件，并解析成FString
FFileHelper::BufferToString //解析uint8* 到FString
//等等
```

### Config文件加载

数据加载方法：

1. 参考通用文本文件操作方法来读取配置文件
2. 解析读取到的数据，保存到GConfig类中

数据加载时机：

1. 在游戏初始化时加载基本的Config文件(BaseEngine.ini、DefaultInput.ini等)的数据到GConfig类中
2. 对于自定义的Config文件，在使用(如GConfig->GetInt)时，会动态的同步加载。

优点：

1. 一般配置文件体积较小，加载迅速
2. 文本文件存储，语法简单，便于编辑
3. 文件读取之后就常驻内存，读写方便

## 二进制文件

### 游戏数据保存与读取


```cc
UGameplayStatics::LoadGameFromSlot//加载GameSave类的数据，核心分成了两步：加载文件到内存、解析内存中的数据并反序列化为对应的USaveGame类
UGameplayStatics::SaveGameToSlot //序列化USaveGame类，并保存到本地

UGameplayStatics::SaveDataToSlot //直接保存TArray<uint8>数据到本地，参考FFileHelper::SaveArrayToFile
UGameplayStatics::LoadDataFromSlot //从本地读取TArray<uint8>，参考FFileHelper::LoadFileToArray
```

USaveGame是一个便于反序列化与序列化的类，UE在GameplayStatics中额外封装了便于使用的API。当某些数据需要本地化存储以及读取时，使用这个类会很方便。

直接使用TArray<uint8>保存数据的优势在于简单且自由，UGameplayStatics::LoadDataFromSlot的优点在于数据读取与存储的格式更加统一。FFileHelper::LoadFileToArray的优点在于简单自由
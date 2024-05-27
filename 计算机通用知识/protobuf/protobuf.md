### 基本介绍：

Protocol Buffers（简称protobuf）是Google开发的一种高效、自动化序列化结构数据的机制，它能够将结构化的数据序列化成字节流，同时也能够反序列化字节流还原成数据结构。protobuf设计初衷是为了替代XML、JSON等数据交换格式，以实现更小、更快、更简单的数据交换协议。下面是protobuf的一些基本特点和组成部分：

官网：[Protocol Buffer Basics: C++ | Protocol Buffers Documentation (protobuf.dev)](https://protobuf.dev/getting-started/cpptutorial/)

github：[protocolbuffers/protobuf: Protocol Buffers - Google's data interchange format (github.com)](https://github.com/protocolbuffers/protobuf/tree/main)

### 主要特点：

1. **高效性**：相比XML和JSON，protobuf使用变长编码、提供字段标识、省略默认值等方法，序列化后的数据体积更小，解析速度更快，特别适合对性能有严格要求的网络通信场景。
2. **跨语言**：protobuf提供了C++、Java、Python等多种语言的API，保证了不同语言之间数据的无缝传输。
3. **跨平台**：自动帮助我们完成大端编码与小端编码的转换。
4. **兼容性**：protobuf设计时考虑了向前和向后兼容性，允许数据结构在某一端添加字段后与另一端依然兼容。
5. **易于使用的工具**：Google提供了protoc编译器，可以将.proto文件编译成各种语言的源代码，简化了开发者的工作。

### 使用流程：

1. **定义数据结构**：在.proto文件中定义你的数据结构。
2. **编译生成代码**：使用protoc编译器，根据.proto文件生成指定语言的源代码。
3. **导入依赖库**：libprotobuf.lib、libprotobuf-lite.lib、libprotoc.lib。
4. **编写业务逻辑**：在你的应用程序中，使用生成的类来构造、序列化、反序列化数据对象。
5. **数据交换**：序列化后的数据可以通过网络或其他方式传输，接收方再进行反序列化恢复原始数据结构。

总之，protobuf是一种高性能、跨平台的数据序列化框架，特别适用于需要频繁进行数据交换的应用场景，尤其是分布式系统、微服务架构中的通信。

注：通过依赖库的方式不利于学习protobuf的原理与特殊API，建议仔细翻阅官方文档或直接下载源码。

### 定义数据与生成代码：

定义数据：参考当前目录的其它文件。

基础用法：`protoc --cpp_out=./ addressbook.proto`

查看帮助：`protoc -h`

### 业务逻辑编写：

**序列化**

```c++
#include "my_message.pb.h" // 包含生成的头文件

// 创建消息实例
tutorial::Person person;
person.set_name("Alice");
person.set_id(123);
person.set_email("alice@example.com");

// 序列化到字符串
std::string serialized;
if (!person.SerializeToString(&serialized)) {
    // 错误处理：序列化失败
}
//将字符串保存或发送到其它平台

// 或者序列化到输出流
std::ofstream output("myfile.dat", std::ios::binary);
if (!person.SerializeToOstream(&output)) {
    // 错误处理：写入文件失败
}
```

**反序列化**

```c++
#include "my_message.pb.h" // 包含生成的头文件

// 创建消息实例
tutorial::Person parsed_person;

// 从字符串反序列化到对象
if (!parsed_person.ParseFromString(serialized)) {
    // 错误处理：解析失败
}

// 或者从输入流反序列化
std::ifstream input("myfile.dat", std::ios::binary);
if (!parsed_person.ParseFromIstream(&input)) {
    // 错误处理：读取文件或解析失败
}

// 使用数据
parsed_person.name();
parsed_person.id();
parsed_person.email();
```

### 源码解析：

**序列化源码调用流程**

```c++
//调用基类函数，序列化并添加到到字符串尾部。
//类似的函数还有多个，基类中提供多种序列化接口，并转化为统一的输出类型，最终调用到子类的序列化函数。
MessageLite::AppendToString(std::string* output)
    //调整字符串大小，找到开始添加数据的位置
--->MessageLite::AppendPartialToString(std::string* output)
		//调用子类方法，统计序列化之后的数据大小
    --->Message::ByteSizeLong()
    	//修改output容器大小
    --->absl::strings_internal::STLStringResizeUninitializedAmortized()
    	//找到容器的原本的数据尾部，从这里开始添加
    --->uint8_t* start=io::mutable_string_data(output) + old_size
    	//将容器转化为地址起点与剩余容器长度(类似数组形式)，并调用序列化到数组
    --->SerializeToArrayImpl(const MessageLite& msg, uint8_t* target, int size)
    		//将地址与长度封装成io::EpsCopyOutputStream便于指针自动移动
    	--->io::EpsCopyOutputStream out(target, size)
    		//在子类完成数据序列化，并写入到输出流
    	--->AddressBook::_InternalSerialize(
    			::uint8_t* target,
    			::google::protobuf::io::EpsCopyOutputStream* stream) 
```

**内存布局**

以Person类为例

```c++
//Person大小 80
//注意实际大小会大许多，这里只是使用指针导致无法统计指向的对象的大小
sizeof(Person) = 80;

//静态数据 
//类大小 304
static const ::google::protobuf::internal::TcParseTable<
      3, 5, 2,
      33, 2>
      _table_;

//处于proto文件中的第几条消息(没有地方用到) 数据大小 4
static constexpr int kIndexInFileMessages = 1;
//类默认实例指针 数据大小 8
static constexpr const void* _raw_default_instance_

//动态数据
//内部类Impl_包含所有动态数据 结构体大小 64
struct Impl_ {
    //大小 4
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    //大小 4
    mutable ::google::protobuf::internal::CachedSize _cached_size_;

    //PhoneNumber数组的类 大小 24
    ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber > phones_;
    //字符串类 大小 8
    ::google::protobuf::internal::ArenaStringPtr name_;
    //字符串类 大小 8
    ::google::protobuf::internal::ArenaStringPtr email_;
    //时间戳类指针 大小 8
    ::google::protobuf::Timestamp* last_updated_;
    //int32类型 大小 4
    ::int32_t id_;
    //向上对齐 4
}

//基类(虚表8，InternalMetadata 8) 大小 16
::google::protobuf::Message
```


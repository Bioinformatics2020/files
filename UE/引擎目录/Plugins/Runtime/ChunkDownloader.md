## 简介

它是热更新流程中，用于Pak资源下载的工具，依靠这款工具以及对应的官方文档，我们可以了解到热更新的基本功能点。请注意它只是完整的热更新系统中很基础的一部分，如果是大型系统，你可能需要较多的扩展。

## 主要技术点或功能点

1. 基于本地Manifest的本地Pak文件管理
2. 本地Manifest与指定的网络Manifest版本管理比对
3. HTTP文件下载、重新请求、下载进度管理
4. Pak缓存中、下载中、已挂载等状态管理
5. 异步资源Mount
6. 以上各种状态提供了大量回调事件用于监听

## 基础使用方法

### 开发配置

配置方法官方参考

1. [设置ChunkDownloader插件 | 虚幻引擎4.27文档 (unrealengine.com)](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Patching/ChunkDownloader/PluginSetup/)
2. [托管ChunkDownloader的清单和资产 | 虚幻引擎4.27文档 (unrealengine.com)](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Patching/ChunkDownloader/LocalHost/)

服务器管理方法

1. 对每个版本的热更新包创建一个文件夹，对应一个单独的Manifest文件
2. 文件夹名称、ContentBuildId名称、Manifest的BuildId均需要对应，便于资源下载
3. Pak更新时，Manifest对应的版本编号也需要更新
4. 服务器可以有多个，资源下载失败时ChunkDownloader支持切换服务器再次尝试

资源管理注意事项

1. 我们需要一套系统来管理资源什么时候更新，期望更新到什么版本
2. 一定要仔细处理主资产分类、便于资源卸载与再加载
3. 常规的打包方法每次都是全量打包，需要自行判断更新了哪些内容并写一个Manifest文件，并将更新的资源上传到服务器

### 运行时流程

首次加载流程

1. 创建ChunkDownloader：GetOrCreate()
2. 初始化：Initialize()
3. 加载本地缓存的配置文件：LoadCachedBuild()
4. 挂载本地的Pak：MountChunks()
5. 加载资源

更新Pak流程

1. 确定需要更新到的版本
2. 更新配置文件：UpdateBuild()，注意更新的版本编号要不同于旧版本
3. 卸载对期望更新的Pak资源引用的模块并GC
4. 下载并挂载新Pak：MountChunks()
5. 重新加载Pak文件对应的资源

## 关键函数

### public

- static getter

  ```c++
  static TSharedPtr<FChunkDownloader> Get();//通过FChunkDownloaderModule模块获取ChunkDownloader
  static TSharedRef<FChunkDownloader> GetChecked();//同上，但触发断言检查
  static TSharedRef<FChunkDownloader> GetOrCreate();//通过FChunkDownloaderModule模块获取ChunkDownloader，若ChunkDownloader为空则创建一个
  static void Shutdown();//通过FChunkDownloaderModule卸载ChunkDownloader
  ```

- 模块初始化与卸载

  ```c++
  //初始化ChunkDownloader，传入平台名称、最大并行下载数量
  //1. 创建Save/PersistentDownloadDir/PakCache下载目录
  //2. 解析Content/EmbeddedPaks/EmbeddedManifest.txt，保存在EmbeddedPaks Map中
  //3. 解析Save/PersistentDownloadDir/PakCache/LocalManifest.txt
  //4. 将LocalManifest中正确记录的文件保存到PakFiles Map中
  //5. 删除未被LocalManifest记录但是存在于本地的文件
  //6. 更新LocalManifest.txt文件
  //7. 初始化PlatformName、TargetDownloadsInFlight(4.27版本这个值的使用很可能存在bug)、
  //		CacheFolder、EmbeddedFolder
  void Initialize(const FString& PlatformName, int32 TargetDownloadsInFlight);
  ```
  
- 更新Manifest文件或更新Pak文件

  ```C++
  //加载本地的Manifest文件
  bool LoadCachedBuild(const FString& DeploymentName);
  
  //若版本更新则加载远程的Manifest文件
  void UpdateBuild(const FString& DeploymentName, const FString& ContentBuildId, const FCallback& Callback);
  
  //根据本地Chunk信息，下载Pak文件到本地，下载流程如下
  //1. 添加下载任务到下载请求队列
  //2. 对每个任务创建一个FDownload类来管理资源下载
  //3. 在FDownload调用PlatformStreamDownload开启最终的HTTP请求以及文件写入到本地
  void DownloadChunks(const TArray<int32>& ChunkIds, const FCallback& Callback, int32 Priority = 0);
  void DownloadChunk(int32 ChunkId, const FCallback& Callback, int32 Priority = 0);
  
  //下载Pak文件并挂载
  //1. 下载流程同上
  //2. 创建一个异步任务FAsyncTask<FPakMountWork>，异步启动Mount操作
  //3. 调用FCoreDelegates::OnMountPak.Execute()完成Mount
  void MountChunks(const TArray<int32>& ChunkIds, const FCallback& Callback);
  void MountChunk(int32 ChunkId, const FCallback& Callback);
  ```

- 文件统计

  ```c++
  //开始统计下载进度，全部下载完成时触发回调
  void BeginLoadingMode(const FCallback& Callback);
  
  //查看最新的下载进度
  const FStats& GetLoadingStats() const;
  
  //查看单个Chunk状态
  EChunkStatus GetChunkStatus(int32 ChunkId) const;
  //将Chunk状态枚举信息转换为字符串
  static const TCHAR* ChunkStatusToString(EChunkStatus Status);
  ```

### private

- 解析Manifest文件

  ```c++
  //Manifest文件格式：
  //$属性名称 = 值 注意等号两边存在空格
  //	$NUM_ENTRIES 表示pak文件数，当值与实际文件数不匹配时报错。
  //pak信息格式：文件名称(字符串)  \t  文件大小(整数，单位字节)  \t  文件版本(字符串)  \t  ChunkId  \t  文件相对路径
  
  //解析Manifest文件，传入文件路径，返回属性信息与文件信息。解析流程如下
  //1. 加载文本文件
  //2. 逐行解析文件，解析属性信息与pak文件信息
  //3. 行数检查
  static TArray<FPakFileEntry> ParseManifest(const FString& ManifestPath, TMap<FString,FString>* Properties = nullptr);
  
  //重新保存本地的Manifest文件
  void SaveLocalManifest(bool bForce);
  ```

- 资源下载与加载

  ```c++
  //设置资源的下载路径，DeploymentName用于查找部署的服务器地址，ContentBuildId用于查找服务器内的资源地址
  void SetContentBuildId(const FString& DeploymentName, const FString& NewContentBuildId);
  
  //加载传入的PakFiles。流程如下
  //1. 阻塞等待原有的资源Mount流程
  //2. 强制GC，便于后面的资源卸载
  //3. 根据传入的新PakFiles更新原有的Chunks与PakFiles。
  //4. Chunks更新流程是添加新的Chunk，将新的PakFile与Chunk绑定，若旧Chunk与新Chunk存在差异且旧Chunk被挂载，则卸载旧Chunk
  //5. PakFiles更新流程是添加新PakFile，移除并卸载新包未包含的旧PakFile
  //6. 更新LocalManifest.txt文件
  void LoadManifest(const TArray<FPakFileEntry>& PakFiles);
  ```


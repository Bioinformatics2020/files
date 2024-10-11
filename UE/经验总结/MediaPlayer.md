# 视频播放函数调用流程

1. MediaPlayer模块：
   
   封装多种不同的打开文件的方法如UMediaPlayer::OpenSource、UMediaPlayer::OpenFile，创建不同类型的MediaSource。最终调用PlayerFacade->Open。

2. FMediaPlayerFacade模块：
   
   - 根据MediaSource所属类型，查找对应类型的IMediaPlayerFactory。
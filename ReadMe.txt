Video Codec SDK 8.1
   * Windows: N卡 Driver version 390.77  or higher
   * Linux:   N卡 Driver version 390.25 or higher

Note: 
     @1Windows 为VS x64工程,由于我的机器没有32位的opencv所以不能使用32位的工程，需要的同事可以自己配置32位的opencv即可开发
	 
	 @2支持纯c++接口(跨平台接口) Qt的qml视频播放接口(跨平台接口) Qt的widgets视频播放接口(跨平台接口)
	 
	 @3支持软解和硬解
	 
	 @4回调函数为解码后的视频格式和视频帧地址
	 
	 @5附带各个平台的Sample示例;纯c++接口与opencv结合显示(主要为了方便为算法提供支持),windows为VS工程，linux为Makefile工程(需要cuda-8.0及以上)；与Qt相关的为跨平台工程
	 
	 @6需要依赖的ffmpeg库在工程的3rd目录可根据对应平台选择
	 
	 @7package目录为编译好的各平台的sdk
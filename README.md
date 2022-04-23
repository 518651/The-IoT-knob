**The-IoT-knob**

项目目录:
Circuit Project为User Desktop端配置进程

main为Iot S3项目

----------
2022/4/6 23:09:40 

----------
virson：TS.01
上传User Desktop端项目,已完成UI代码编写,下一步将移植现有TCP库/AUTO UPDATE 对接S3 TCP客户端

TCP通讯请参考:
https://github.com/jchristn/SuperSimpleTcp

请注意,S3代码为项目缓存,含有大量错误代码,现在尽量不要移植！


----------
2022/4/13 19:30:53 

----------
对Circuit Project加装TCP通讯及API生成函数,可执行对低交互
主线项目正在执行LVGL的移植,暂时有BUG,无法完成烧录后的监听.

下一进度:Circuit Project完成自升级模块开发
主线项目:对接LVGL的全面UI编写,业务代码及UI代码达到最低交互程度

----------
2022/4/18 10:10:24 

----------
新增类:AutoUpdata自动更新函数


----------
2022/4/22 3:03:12 
----------
新增LVGL\TCP模块(调试的时候又把环境调傻了,没有机会再测试,顾留到下一次)


----------
2022/4/23 19:47:40 

----------
花了点时间重新构建本项目代码结构,**放弃对MAIN文件代码支持,改为ES-KRNB**

此次新增:
	
	[WIFI]
		支持扫描AP信元	[完成]
		桌面端程序添加WIFI [正在执行]
		蓝牙添加WIFI [正在执行]
		断点重启后读取WIFI Config文件重新连接WIFI [完成]

	[SCREEN]
		LVGL项目移植 [完成--未测试]
		屏幕UI [完成--未测试]
	
	[TCP]
		TCP服务端 [完成--未测试]
		异步传输[正在执行]

	[蓝牙通讯]
		对接桌面端功能 [开/关机] [未完成]
		对接桌面端功能 [快捷键]  [未完成]

![](https://github.com/518651/The-IoT-knob/blob/main/photo/code.png)

	功能均正常编译

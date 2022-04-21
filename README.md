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
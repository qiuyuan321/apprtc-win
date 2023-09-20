# apprtc-win

webrtc apprtc window app; qt  
本项目封装webrtc的peerclient为动态库，减少webrtc的依赖。后端服务器采用apprtc，基于qt框架的视频一对一通话 demo

### 编译方法
1、安装Qt5 + vs2019   
2、首先解压【third/opencv343.zip】到【third】文件夹   
3、修改【prepareWinBuild.bat】，将Qt5_ROOT改为Qt的对应安装目录  
4、双击运行【prepareWinBuild.bat】，将生成对应的工程文件【win_build】  
5、打开【win_build/apprtc-win.sln】，进行 编译即可  
6、自动打包qt运行环境方法，运行【win_build/Release/_qtenv.bat】将自动拷贝qt相关文件


### 软件运行截图
![]()
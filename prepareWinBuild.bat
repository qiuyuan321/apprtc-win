@echo off
set Qt5_ROOT=C:\dev_tools\Qt5.13.2\5.13.2\msvc2017_64
set build_dir=win_build
echo ====== Build env in [%build_dir%] with QT[%Qt5_ROOT%]
md %build_dir%
cd %build_dir%
md Debug
md Release

set targetDir=Debug
echo ====== Copy files to [%targetDir%]...
xcopy ..\third\opencv343\dll\opencv_world343d.dll                         %targetDir%\ /Y
xcopy ..\third\webrtc\lib\peerconnection_client_shared.dll                %targetDir%\ /Y
echo %Qt5_ROOT%\bin\windeployqt.exe apprtc-win.exe >                      %targetDir%\_qtenv.bat

set targetDir=Release
echo ====== Copy files to [%targetDir%]...
xcopy ..\third\opencv343\dll\opencv_world343.dll                          %targetDir%\ /Y
xcopy ..\third\webrtc\lib\peerconnection_client_shared.dll                %targetDir%\ /Y
echo %Qt5_ROOT%\bin\windeployqt.exe apprtc-win.exe >                      %targetDir%\_qtenv.bat


echo ====== Running cmake...
cmake .. -DCMAKE_INSTALL_PREFIX=install -DCMAKE_CONFIGURATION_TYPES="Debug;Release"  -DQt5_DIR=%Qt5_ROOT%\lib\cmake\Qt5 -G "Visual Studio 16 2019"
cd ..
echo ============================================
echo Completed! 
pause
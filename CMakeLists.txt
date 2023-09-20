cmake_minimum_required (VERSION 3.5.1)
project(apprtc-win)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED Core Network Gui Widgets WebSockets)

set(OpenCV_DIR ${PROJECT_SOURCE_DIR}/third/opencv343)
set(webrtc_DIR ${PROJECT_SOURCE_DIR}/third/webrtc)
set(extra_install  ${OpenCV_DIR}/dll/opencv_world343.dll 
               ${fftw3_DIR}/dll/libfftw3-3.dll 
		       ${fftw3_DIR}/dll/libfftw3f-3.dll )
set(CMAKE_CXX_FLAGS_RELEASE "/MD")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Od")


include_directories(${OpenCV_DIR}/include 
					${webrtc_DIR}/include 
					apprtc-win/
					)
file(GLOB_RECURSE apprtc_SOURCES apprtc-win/*.cpp)
file(GLOB_RECURSE apprtc_HEADERS apprtc-win/*.h)
file(GLOB_RECURSE ui_FORMS apprtc-win/*.ui) 
QT5_WRAP_UI(ui_FORMS_HEADERS ${ui_FORMS})
set(apprtc-win_QRC apprtc-win/apprtcwin.qrc)

set(ALL_Sources 
    ${apprtc_SOURCES} ${apprtc_HEADERS}
	${ui_FORMS} ${ui_FORMS_HEADERS})
   
qt5_add_resources(apprtc-win_QRC_SOURCES ${apprtc-win_QRC})

source_group(apprtc-win FILES ${apprtc_SOURCES} ${apprtc_HEADERS})

add_executable(apprtc-win # WIN32
    ${ALL_Sources} ${apprtc-win_QRC_SOURCES})

target_link_libraries(apprtc-win PRIVATE 
    debug ${OpenCV_DIR}/lib/opencv_world343d.lib
    optimized ${OpenCV_DIR}/lib/opencv_world343.lib
    ${webrtc_DIR}/lib/peerconnection_client_shared.dll.lib
	#setupapi.lib
    Qt5::Core
	Qt5::Widgets
    Qt5::Gui
	Qt5::WebSockets
    )



#install(FILES ${extra_install} src/startup.bmp DESTINATION ${CMAKE_INSTALL_PREFIX})
#install(TARGETS OverLimitMonitor)

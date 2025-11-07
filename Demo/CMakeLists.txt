# CMakeList.txt: cloud_injector 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

if(NOT DEFINED OSG_PATH)
    message(FATAL_ERROR "OSG_PATH is not defined. Please pass -DOSG_PATH=your_osg_path to cmake.")
endif()

# 通过命令行参数传递 CMAKE_BUILD_INCLUDE_DIR
if(NOT DEFINED CMAKE_BUILD_INCLUDE_DIR)
    message(FATAL_ERROR "CMAKE_BUILD_INCLUDE_DIR is not defined. Please pass -DCMAKE_BUILD_INCLUDE_DIR=your_include_dir to cmake.")
endif()

if(NOT DEFINED SYZ_SDK_VERSION)
    message(FATAL_ERROR "SYZ_SDK_VERSION is not defined. Please pass -DSYZ_SDK_VERSION= sdk branch name.")
endif()

SET(CMAKE_OSG_DIR ${OSG_PATH})

set(CMAKE_CXX_STANDARD 17) #poco 需要c++ 17支持

set(CMAKE_CURRENT_SOURCE_DIR .)
# 主目录文件
file(GLOB MAIN_SOURCES "*.cpp" "*.h")
source_group("src" FILES ${MAIN_SOURCES})
add_executable (datacore_demo ${MAIN_SOURCES})

target_include_directories(datacore_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../DataCore 
${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/include
${CMAKE_OSG_DIR}/osg/include
${CMAKE_OSG_DIR}/osgearth/include
)

target_compile_definitions(datacore_demo PRIVATE _CONSOLE ENABLE_OSG_HEADERS)
	
if(WIN32)
target_link_libraries(datacore_demo PUBLIC
	../../install/lib/windows/DataCore
	${CMAKE_OSG_DIR}/osg/lib/syz.lib
	${CMAKE_OSG_DIR}/osg/lib/syzViewer.lib
	${CMAKE_OSG_DIR}/osg/lib/syzWidget.lib
	${CMAKE_OSG_DIR}/osg/lib/syzGA.lib
	${CMAKE_OSG_DIR}/osg/lib/syzUtil.lib
	${CMAKE_OSG_DIR}/osg/lib/syzDB.lib
	${CMAKE_OSG_DIR}/osg/lib/OpenThreads.lib
	${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/lib/Release/sceneCore.lib
	)
elseif(UNIX)
#target_link_libraries(datacore_demo PUBLIC
#	../../install/lib/linux/DataCore
#	pthread
#	)
endif()


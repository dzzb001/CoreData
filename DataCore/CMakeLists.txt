cmake_minimum_required(VERSION 3.16)

project(DataCore)

# 设置安装前缀
set(CMAKE_INSTALL_PREFIX "/opt/dataCore" CACHE PATH "Installation directory")

# 包含标准安装目录定义
include(GNUInstallDirs)

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
SET(CMAKE_CURRENT_SOURCE_DIR ./)
SET(DATACORE_3RDPARTY_PATH "${CMAKE_SOURCE_DIR}/../Dependencies" CACHE STRING "Dependency root path")


# 添加可执行文件并指定特定目录下的源文件
file(GLOB SOURCES "./*.cpp" "./*.h")  # 匹配指定目录下的所有 .cpp 文件
file(GLOB GLOBSetting_SOURCES "./GlobalSetting/*.cpp" "./GlobalSetting/*.h")
source_group("GlobalSetting" FILES ${GLOBSetting_SOURCES})

# 添加工具类
file(GLOB Tool_SOURCES "./Tool/*.cpp" "./Tool/*.h")
source_group("Tool" FILES ${Tool_SOURCES})

if(WIN32)
	# 设置默认的构建类型
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
	endif()
endif()

IF(USE_WASM_OPTIONS)
	add_library(DataCore STATIC ${SOURCES} ${GLOBSetting_SOURCES} ${Tool_SOURCES})
ELSE()
	add_library(DataCore SHARED ${SOURCES} ${GLOBSetting_SOURCES} ${Tool_SOURCES})
ENDIF()

IF(USE_WASM_OPTIONS)
#CMAKE_OSG_DIR}/include 包含osg、osgeath、osgverse
target_include_directories(DataCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/GlobalSetting ${CMAKE_CURRENT_SOURCE_DIR}/Tool ${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/include
${CMAKE_OSG_DIR}/include
)
ELSE()
target_include_directories(DataCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/GlobalSetting ${CMAKE_CURRENT_SOURCE_DIR}/Tool ${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/include
${CMAKE_OSG_DIR}/osg/include
${CMAKE_OSG_DIR}/osgearth/include
)
ENDIF()

target_compile_definitions(DataCore PRIVATE _CONSOLE ENABLE_OSG_HEADERS BUILD_SDK_VERSE)

# 检查操作系统类型，设置特定的编译选项
if (WIN32)
    # Windows 平台特定设置
    # 可以在这里添加 Windows 平台特定的编译选项、链接库等
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GL")
	# 设置堆栈保留 16MB，提交 1MB 
    target_link_options(DataCore PRIVATE "/STACK:16777216")
    target_link_options(DataCore PRIVATE "/HEAP:16777216")
	# 链接库文件到目标
	target_link_libraries(DataCore PUBLIC
		${CMAKE_OSG_DIR}/osg/lib/syz.lib
		${CMAKE_OSG_DIR}/osg/lib/syzViewer.lib
		${CMAKE_OSG_DIR}/osg/lib/syzWidget.lib
		${CMAKE_OSG_DIR}/osg/lib/syzGA.lib
		${CMAKE_OSG_DIR}/osg/lib/syzUtil.lib
		${CMAKE_OSG_DIR}/osg/lib/syzDB.lib
		${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/lib/Release/sceneCore.lib
	)
elseif (UNIX)
    # Linux 平台特定设置
    # 可以在这里添加 Linux 平台特定的编译选项、链接库等
	#set(CMAKE_CXX_FLAGS "-g -MMD -std=c++11")  #注意调试的话,此行代码一定要添加!-fno-rtti
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
	
	IF(USE_WASM_OPTIONS)	
		# https://emscripten.org/docs/porting/simd.html
		SET(DATACORE_SIMD_FEATURES sse sse2 sse3 avx)
		MESSAGE(STATUS "[osgVerse] WebAssembly Porting. Features: ${DATACORE_SIMD_FEATURES}")
			
		# 设置编译目标为WASM
		set(CMAKE_SYSTEM_NAME Emscripten)
		set(CMAKE_C_COMPILER emcc)
		set(CMAKE_CXX_COMPILER em++)
		target_compile_options(DataCore PRIVATE -O2)
		target_link_options(DataCore PRIVATE -s WASM=1 -s STANDALONE_WASM)
		target_compile_definitions(DataCore PRIVATE USE_WASM)
	ENDIF()

	IF(DATACORE_SIMD_FEATURES)
		# Find SIMD instruction supporting using the following statement:
		#   IF("sse3" IN_LIST DATACORE_SIMD_FEATURES) ...
		IF(USE_WASM_OPTIONS)
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msimd128")
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msimd128")
		ENDIF()
	ENDIF()
	
	add_compile_options(-fPIC)
	add_compile_options(-pthread)
	add_compile_options(--Wl,-rpath=./)
	
	IF(USE_WASM_OPTIONS)
		target_link_libraries(DataCore PUBLIC 
			${CMAKE_BUILD_INCLUDE_DIR}/${SYZ_SDK_VERSION}/BuilderSDK/lib/Release/libSceneCore.a 
			${DOSG_PATH}/lib/libosgVerseWrappers.a
			${DOSG_PATH}/lib/libosgVerseAnimation.a
			${DOSG_PATH}/lib/libosgVerseReaderWriter.a
			${DOSG_PATH}/lib/libosgViewer.a
			${DOSG_PATH}/lib/libosgWidget.a
			${DOSG_PATH}/lib/libosgGA.a
			${DOSG_PATH}/lib/libosgDB.a
			${DOSG_PATH}/lib/libosg.a
			${DOSG_PATH}/lib/libosgUtil.a
			${DOSG_PATH}/lib/libOpenThreads.a
			${DOSG_PATH}/lib/libosgText.a
			${DOSG_PATH}/lib/libosgVerseAnimation.a
			${DOSG_PATH}/lib/libosgVerseModeling.a
			${DOSG_PATH}/lib/libCommonData.a
			)
	ELSE()
		# 链接库文件到目标
		target_link_libraries(DataCore PUBLIC 
			${CMAKE_THRD_LIB_DIR}/libSceneCore.so 
			${CMAKE_THRD_LIB_DIR}/libosgVerseWrappers.so
			${CMAKE_THRD_LIB_DIR}/libosgVerseAnimation.so
			${CMAKE_THRD_LIB_DIR}/libosgVerseReaderWriter.so
			${CMAKE_THRD_LIB_DIR}/libosgViewer.so
			${CMAKE_THRD_LIB_DIR}/libosgWidget.so
			${CMAKE_THRD_LIB_DIR}/libosgGA.so
			${CMAKE_THRD_LIB_DIR}/libosgDB.so
			${CMAKE_THRD_LIB_DIR}/libosg.so
			${CMAKE_THRD_LIB_DIR}/libosgUtil.so
			${CMAKE_THRD_LIB_DIR}/libOpenThreads.so
			${CMAKE_THRD_LIB_DIR}/libosgText.so
			${CMAKE_THRD_LIB_DIR}/libosgVerseAnimation.a
			${CMAKE_THRD_LIB_DIR}/libosgVerseModeling.a
			${CMAKE_THRD_LIB_DIR}/libCommonData.a
		)
	ENDIF()
ENDIF()

set_property(TARGET DataCore PROPERTY CXX_STANDARD 17)

# 安装配置
install(FILES DataCoreInterface.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
		
install(TARGETS DataCore
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)
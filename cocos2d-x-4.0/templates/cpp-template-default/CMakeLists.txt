#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.12)

set(APP_NAME game_test)
project(${APP_NAME})

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

# ============================================================
# 引擎只编译一次
# ============================================================
if(NOT DEFINED BUILD_ENGINE_DONE)
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)
    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# ============================================================
# 自动扫描所有 Classes* 模块目录（必须含 AppDelegate.h）
# ============================================================
file(GLOB APP_MODULE_CANDIDATES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Classes*")

set(APP_MODULE_DIRS)
foreach(DIR ${APP_MODULE_CANDIDATES})
    if(IS_DIRECTORY "${DIR}" AND EXISTS "${DIR}/AppDelegate.h")
        list(APPEND APP_MODULE_DIRS "${DIR}")
    endif()
endforeach()

list(SORT APP_MODULE_DIRS)

if(NOT APP_MODULE_DIRS)
    message(FATAL_ERROR "没有找到任何 Classes* 模块目录（必须包含 AppDelegate.h）")
endif()

message(STATUS "===== 自动发现的模块 =====")
foreach(DIR ${APP_MODULE_DIRS})
    message(STATUS "  - ${DIR}")
endforeach()
message(STATUS "===========================")

# ============================================================
# 同步脚本定位
# ============================================================
set(SYNC_SCRIPT "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts/sync_folder.py")
    set(SYNC_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/sync_folder.py")
elseif(EXISTS "${COCOS2DX_ROOT_PATH}/cmake/scripts/sync_folder.py")
    set(SYNC_SCRIPT "${COCOS2DX_ROOT_PATH}/cmake/scripts/sync_folder.py")
endif()

if(SYNC_SCRIPT)
    message(STATUS "SYNC_SCRIPT = ${SYNC_SCRIPT}")
else()
    message(STATUS "SYNC_SCRIPT 未找到，将使用 copy_directory 全量拷贝")
endif()

# ============================================================
# 平台入口（共用）
# ============================================================
set(PLATFORM_SOURCE)
set(PLATFORM_HEADER)
set(APP_UI_RES)

if(ANDROID)
    list(APPEND PLATFORM_SOURCE proj.android/app/jni/hellocpp/main.cpp)
elseif(LINUX)
    list(APPEND PLATFORM_SOURCE proj.linux/main.cpp)
elseif(WINDOWS)
    list(APPEND PLATFORM_HEADER proj.win32/main.h proj.win32/resource.h)
    list(APPEND PLATFORM_SOURCE proj.win32/main.cpp proj.win32/game.rc)
elseif(APPLE)
    if(IOS)
        list(APPEND PLATFORM_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h)
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets)
        list(APPEND PLATFORM_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES})
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist)
        list(APPEND PLATFORM_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES})
    endif()
endif()

# ============================================================
# Android：只编第一个模块
# ============================================================
if(ANDROID)
    list(GET APP_MODULE_DIRS 0 FIRST_DIR)
    get_filename_component(FIRST_DIR_NAME ${FIRST_DIR} NAME)

    file(GLOB_RECURSE FIRST_DIR_FILES CONFIGURE_DEPENDS
        "${FIRST_DIR}/*.cpp" "${FIRST_DIR}/*.cc" "${FIRST_DIR}/*.c"
        "${FIRST_DIR}/*.h"   "${FIRST_DIR}/*.hpp")

    set(APP_NAME MyGame)

    add_library(${APP_NAME} SHARED
        ${PLATFORM_HEADER} ${PLATFORM_SOURCE}
        ${FIRST_DIR_FILES})

    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
    target_link_libraries(${APP_NAME} cocos2d)
    target_include_directories(${APP_NAME}
            PRIVATE ${FIRST_DIR}
            PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/)

    setup_cocos_app_config(${APP_NAME})
    return()
endif()

# ============================================================
# 多模块循环
# ============================================================
foreach(MOD_DIR ${APP_MODULE_DIRS})
    get_filename_component(MOD_DIR_NAME ${MOD_DIR} NAME)
    get_filename_component(MOD_DIR_ABS "${MOD_DIR}" ABSOLUTE)

    if(MOD_DIR_NAME STREQUAL "Classes")
        set(MOD_NAME "game_test")
        set(RES_SUFFIX "")
    else()
        string(REGEX REPLACE "^Classes_" "" MOD_SUFFIX "${MOD_DIR_NAME}")
        set(MOD_NAME "game_test_${MOD_SUFFIX}")
        string(REGEX REPLACE "^Classes_" "" RES_SUFFIX "${MOD_DIR_NAME}")
    endif()

    file(GLOB_RECURSE MOD_FILES CONFIGURE_DEPENDS
        "${MOD_DIR_ABS}/*.cpp"
        "${MOD_DIR_ABS}/*.cc"
        "${MOD_DIR_ABS}/*.cxx"
        "${MOD_DIR_ABS}/*.c"
        "${MOD_DIR_ABS}/*.h"
        "${MOD_DIR_ABS}/*.hpp"
        "${MOD_DIR_ABS}/*.hxx"
        "${MOD_DIR_ABS}/*.inl"
        )

    if(NOT MOD_FILES)
        message(WARNING "模块 ${MOD_NAME}: ${MOD_DIR} 里没有找到任何源文件，跳过")
        continue()
    endif()

    add_executable(${MOD_NAME}
        ${PLATFORM_SOURCE}
        ${PLATFORM_HEADER}
        ${MOD_FILES}
        )

    source_group(TREE "${MOD_DIR_ABS}" PREFIX "Source Files" FILES ${MOD_FILES})
    if(PLATFORM_SOURCE)
        source_group("Source Files" FILES ${PLATFORM_SOURCE})
    endif()
    if(PLATFORM_HEADER)
        source_group("Header Files" FILES ${PLATFORM_HEADER})
    endif()

    target_link_libraries(${MOD_NAME} cocos2d)
    target_include_directories(${MOD_NAME}
            PRIVATE ${MOD_DIR}
            PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
    )

    # 内部会建 SYNC_RESOURCE-${MOD_NAME}，挂 Utils
    setup_cocos_app_config(${MOD_NAME})

    if(APPLE)
        set_target_properties(${MOD_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
        if(MACOSX)
            set_xcode_property(${MOD_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
        elseif(IOS)
            set_xcode_property(${MOD_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
            set_xcode_property(${MOD_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        endif()
    elseif(WINDOWS)
        cocos_copy_target_dll(${MOD_NAME})
    endif()

    # ============================================================
    # 资源同步：挂到 Cocos 自带的 SYNC_RESOURCE-${MOD_NAME}
    #   ★ 不判断目录是否存在，命令永远挂上
    #   运行时脚本自己判断源目录在不在
    # ============================================================
    if(RES_SUFFIX STREQUAL "")
        set(MOD_RES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
    else()
        set(MOD_RES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Resources_${RES_SUFFIX}")
    endif()

    set(MOD_RES_DST "${CMAKE_BINARY_DIR}/bin/${MOD_NAME}/$<CONFIG>/Resources")

    if(TARGET SYNC_RESOURCE-${MOD_NAME})
        if(SYNC_SCRIPT)
            add_custom_command(TARGET SYNC_RESOURCE-${MOD_NAME} POST_BUILD
                COMMAND ${PYTHON_COMMAND} ARGS
                    "${SYNC_SCRIPT}"
                    -s "${MOD_RES_SRC}"
                    -d "${MOD_RES_DST}"
                    -m "$<CONFIG>"
                COMMENT "Sync ${MOD_NAME} resources (${RES_SUFFIX})"
                VERBATIM
            )
        else()
            add_custom_command(TARGET SYNC_RESOURCE-${MOD_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${MOD_RES_DST}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${MOD_RES_SRC}" "${MOD_RES_DST}"
                COMMENT "Copy ${MOD_NAME} resources (${RES_SUFFIX})"
                VERBATIM
            )
        endif()

        if(EXISTS "${MOD_RES_SRC}")
            message(STATUS "模块 ${MOD_NAME}: 资源后缀='${RES_SUFFIX}'，资源源=${MOD_RES_SRC}")
        else()
            message(STATUS "模块 ${MOD_NAME}: 资源目录 ${MOD_RES_SRC} 暂不存在（运行时创建后会在下次构建自动同步）")
        endif()
    else()
        message(WARNING "模块 ${MOD_NAME}: 没有 SYNC_RESOURCE-${MOD_NAME} target")
    endif()
endforeach()
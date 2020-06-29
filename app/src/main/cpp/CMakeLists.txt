# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


# 输出信息到，高版本cmake只有WARNING及以上才会输出了
message(WARNING "yy cmake输出测试")
message(WARNING "yy cmake输出测试2")
message(WARNING CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR})
message(WARNING PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR})

##需要引入的opencv头文件路径，不然编译时cpp中include的时候找不到
#include_directories(opencv_inc)  #TODO 默认是cmake所在路径？？？
include_directories(E:/MyGit/NDK/OpenCV/app/src/main/cpp/opencv_inc)
#添加引入第三方opencv so不然 opencv header找不到对应实现方法
add_library(opencv_java3 SHARED IMPORTED)  #TODO add_library参数含义
# 指名第三方opencv_java3 so所在的路径
set_target_properties( opencv_java3
        PROPERTIES IMPORTED_LOCATION
        E:/MyGit/NDK/OpenCV/app/libs/${ANDROID_ABI}/libopencv_java3.so )#默认是在cmake所在路径
#set_target_properties( opencv_java3
#        PROPERTIES IMPORTED_LOCATION
#        E:/MyGit/NDK/OpenCV/app/libs/armeabi-v7a/libopencv_java3.so )#默认是在cmake所在路径
#set_target_properties( opencv_java3
#        PROPERTIES IMPORTED_LOCATION64
#        E:/MyGit/NDK/OpenCV/app/libs/arm64-v8a/libopencv_java3.so )#TODO 64位要用这个，怎么一次性导入？？？
message(WARNING header: E:/MyGit/NDK/OpenCV/app/libs/${ANDROID_ABI}/libopencv_java3.so)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#set_target_properties( opencv_java3
#        PROPERTIES IMPORTED_LOCATION
#        ../../../libs/armeabi-v7a/libopencv_java3.so )#默认是在cmake所在路径

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                        #加入该opencv依赖库
                        opencv_java3
                        #加入android bitmap依赖库
                        jnigraphics
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
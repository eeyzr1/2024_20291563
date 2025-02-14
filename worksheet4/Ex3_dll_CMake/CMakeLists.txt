# calc/CMakeLists.txt

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# 添加静态库（maths）
add_library(maths adder.cpp)

# 可选：强制 CMake 生成静态库或动态库
# add_library(maths STATIC adder.cpp)
# add_library(maths SHARED adder.cpp)

# 如果不是动态库，则定义 `maths_STATIC` 宏，供 `adder.h` 使用
if (NOT BUILD_SHARED_LIBS)
    add_definitions(-Dmaths_STATIC)
endif()

# 添加可执行文件（calc）
add_executable(calc calc.cpp)

# 链接静态库到 `calc`
target_link_libraries(calc maths)

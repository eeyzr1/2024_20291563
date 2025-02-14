cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# 构建动态或静态库
add_library(maths SHARED adder.cpp)

# 添加 `calc` 可执行文件
add_executable(calc calc.cpp)

# 让 `calc` 链接 `maths`
target_link_libraries(calc maths)

cmake_minimum_required(VERSION 3.23)
project(TomlExam)

# 设置项目开启C++17版本要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 打开允许创建文件夹的开关
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 调用cmake的一些环境配置
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolkit.cmake)
# 引入公用的cmake文件
include(${CMAKE_CURRENT_SOURCE_DIR}/CommonSource/CommonSource.cmake)

# 设置项目输出路径
SET_PROJECT_OUTPUT(${CMAKE_SOURCE_DIR}/publish)

# option 
option(EXAMPLE_WRITE_ENABLED        "WRITE EXAMPLE PROJECT ENABLED" ON)

# option 
option(EXAMPLE_READ_ENABLED         "READ EXAMPLE PROJECT ENABLED" ON)

# WRITE EXAMPLE
if (EXAMPLE_WRITE_ENABLED)
        add_subdirectory("example_write")
endif()

# READ EXAMPLE 
if (EXAMPLE_READ_ENABLED)
        add_subdirectory("example_read")
endif()


# 2. 把工程加到文件夹中
# set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "CMakeTargets")
# 3. 给 cmake 自动创建的工程重新命名, 默认名为 "CMakePredefinedTargets" 
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
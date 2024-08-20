cmake_minimum_required(VERSION 3.23)

project(exam_read)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定源码目录
set(PROJECT_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 有这些文件需要纳入编译: 不推荐这样用， 应该一个一个文件的手动加入到这里
file(GLOB
SRC_FILES 
${PROJECT_SRC_ROOT}/*.*
)

# 当前项目的源文件分组
source_group("read" FILES ${SRC_FILES})

# 当前项目有这些源文件需要纳入编译
set(PROJECT_SRC_FILES ${SRC_FILES})

# 有这些来自 DMCommon 文件夹下的源码文件需要纳入编译, 这里填写文件夹的名称
set(DM_COMMON_NAME
toml
)

# 头文件搜索路径
set(THIRD_MODULE_INCLUDE_ROOT_LIST ${PROJECT_SRC_ROOT})

# 遍历文件夹下的源码文件
foreach(EACH_FOLDER ${DM_COMMON_NAME})
    # 将其他需要纳入编译的源码文件加入 PROJECT_SRC_FILES 中
    list(APPEND PROJECT_SRC_FILES ${${EACH_FOLDER}_SRC_FILES})
    # 有这些头文件在这里搜索
    list(APPEND THIRD_MODULE_INCLUDE_ROOT_LIST ${${EACH_FOLDER}_SRC_ROOT} ${${EACH_FOLDER}_INTERFACE_ROOT})
    # 设置源文件分组
    # message("${${EACH_FOLDER}_FILTER_NAME}")
    source_group("${${EACH_FOLDER}_FILTER_NAME}" FILES ${${EACH_FOLDER}_SRC_FILES})
endforeach()

# 创建可执行程序项目
add_executable(${PROJECT_NAME} ${PROJECT_SRC_FILES})

# 有这么多的头文件路径需要设置
SET_PROJECT_INCLUDE_ROOT(TARGET_NAME ${PROJECT_NAME} VISIBLE_FLAG PRIVATE INCLUDE_ROOT ${THIRD_MODULE_INCLUDE_ROOT_LIST})

# 设置管理员权限
USE_ADMINISTRATOR(${PROJECT_NAME})

# 设置关闭控制台
CLOSE_RELEASE_CONSOLE()

# 设置UTF8编码
SRC_FILES_ENCODE_UTF8(${PROJECT_NAME})

# 拷贝 example.toml到输出文件夹
CP_FILES(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/exam_file/example.toml TARGET_ROOT ${PBD} ${PBR})



# 当前文件分组的名称
set(MODULE_NAME "toml")

# 源文件所在路径， 这里存放的是不需要导出给外部使用的接口文件
set(${MODULE_NAME}_SRC_ROOT 		${CMAKE_CURRENT_LIST_DIR}/src)
# 接口头文件所在路径， 如果是动态库导出的接口，可以将文件放在这里
set(${MODULE_NAME}_INTERFACE_ROOT 	${CMAKE_CURRENT_LIST_DIR}/interface)

# 设置需要加入编译的源码文件
file( GLOB
${MODULE_NAME}_SRC_FILES 
${${MODULE_NAME}_SRC_ROOT}/*.*
${${MODULE_NAME}_INTERFACE_ROOT}/*.cpp
${${MODULE_NAME}_INTERFACE_ROOT}/toml
)

# 接口文件有这些
file(GLOB ${MODULE_NAME}_INTERFACE_FILES ${${MODULE_NAME}_INTERFACE_ROOT}/*.*)

list(APPEND ${MODULE_NAME}_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME}.cmake)

# 设置源码文件分组的名称
set(${MODULE_NAME}_FILTER_NAME ${MODULE_NAME})
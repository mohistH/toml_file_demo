# 有这么多的cmake文件
file(GLOB_RECURSE
DM_COMMON_CMAKE_FILES
${CMAKE_CURRENT_LIST_DIR}/src/*.cmake
)

# 引入cmake文件
foreach(EACH_CMAKE ${DM_COMMON_CMAKE_FILES})
    include(${EACH_CMAKE})
endforeach()

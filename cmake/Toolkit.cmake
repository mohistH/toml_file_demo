# ------------------------------------------------------------
# 拷贝文件
macro(CP_FILES )	
	set(options)
	set(oneValueArgs )
	# SRC_FILES - 要拷贝哪些文件
	# TARGET_ROOT - 拷贝到哪里
	set(multiValueArgs SRC_FILES TARGET_ROOT )
	# 解析参数
	cmake_parse_arguments(CF "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	# 有这么多文件需要拷贝
    foreach(WHICH_FILE ${CF_SRC_FILES})
		# 将文件拷贝到这么多的文件夹
		foreach(WHICH_ROOT ${CF_TARGET_ROOT})
			# 将文件WHICH_FILE拷贝到WHICH_ROOT目录
			execute_process( COMMAND ${CMAKE_COMMAND} -E copy ${WHICH_FILE} ${WHICH_ROOT})
		endforeach()
    endforeach()
endmacro()


# ------------------------------------------------------------
# 创建文件夹
macro(MKDIR)
	set(options)
	set(oneValueArgs )
	# TARGET_ROOT - 有哪些路径需要创建
	set(multiValueArgs TARGET_ROOT )
	# 解析参数
	cmake_parse_arguments(MK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	# 需要创建这么多的的文件夹
	foreach(WHICH_ROOT ${MK_TARGET_ROOT})
		# 创建文件夹落地
		execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${WHICH_ROOT})
	endforeach()
endmacro()


# ------------------------------------------------------------
# 拷贝文件夹
macro(CP_ROOT)
	set(options)
	set(oneValueArgs )
	# SRC_ROOT - 要拷贝哪些文件夹
	# TARGET_ROOT - 拷贝到哪里
	set(multiValueArgs SRC_ROOT TARGET_ROOT )
	# 解析参数
	cmake_parse_arguments(CR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	# 有这么多需要拷贝
    foreach(WHICH_SRC_ROOT ${CR_SRC_ROOT})
		# 将文件拷贝到这么多的文件夹
		foreach(WHICH_TARGET_ROOT ${CR_TARGET_ROOT})
			# 将文件 WHICH_SRC_ROOT 拷贝到 WHICH_TARGET_ROOT 目录
			execute_process( COMMAND ${CMAKE_COMMAND} -E copy_directory ${WHICH_SRC_ROOT} ${WHICH_TARGET_ROOT})
		endforeach()
    endforeach()
endmacro()


#
# 设置项目输出
# 
macro(SET_PROJECT_OUTPUT OUTPUT_PREFIX)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
        set(compiler clang)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
        set(compiler gcc)
        
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(compiler intel)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if(MSVC_VERSION EQUAL 1400)
            set(compiler vc80)
        elseif(MSVC_VERSION EQUAL 1500)
            set(compiler vc90)
        elseif(MSVC_VERSION EQUAL 1600)
            set(compiler vc100)
        elseif(MSVC_VERSION EQUAL 1700)
            set(compiler vc110)
        elseif(MSVC_VERSION EQUAL 1800)
            set(compiler vc120)
        elseif(MSVC_VERSION EQUAL 1900)
            set(compiler vc140)
        elseif(MSVC_VERSION EQUAL 1910)
            set(compiler vc141)
        elseif(MSVC_VERSION EQUAL 1920)
            set(compiler vc142)
        elseif(MSVC_VERSION EQUAL 1930)
            set(compiler vc143)
        else()
            set(compiler vcXX)
        endif()
        
    endif()

    # 64位
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(platform x64)
    else()
        set(platform x86)
    endif()

    # 输出前缀
    set(PUBLISH_ROOT ${OUTPUT_PREFIX})
    set(PUBLISH_PREFIX ${PUBLISH_ROOT}/${compiler}/${platform})

    # 设置 可执行程序输出目录
    set(PBD 			${PUBLISH_PREFIX}/bin/Debug)
    set(PBR 		    ${PUBLISH_PREFIX}/bin/Release)

    # 设置库文件输出目录
    set(PLD 			${PUBLISH_PREFIX}/lib/Debug)
    set(PLR 	    	${PUBLISH_PREFIX}/lib/Release)
    # 头文件所在路径
    set(PIR             ${PUBLISH_PREFIX}/include)


    # 指定可执行程序输出目录
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PBD})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PBR})

    # 指定 库文件输出目录
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${PLD})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE 	${PLR})

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PLD})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PLR})

    

    # 输出cmake文件夹
    set(PC ${PUBLISH_PREFIX}/cmake)

    # 有这么多文件夹需要创建
    set(TO_MK_ROOT 
    ${PIR}
    ${PLD}
    ${PLR}
    ${PBD}
    ${PBR}
    ${PC}
    )

    MKDIR(TARGET_ROOT ${TO_MK_ROOT})
endmacro()


# 拷贝 dll到输出目录
# RELEASE_ROOT - RELEASE的输出路径
# DEBUG_ROOT - debug的输出路径
macro(COPY_EXEC_TO_OUTPUT_ROOT WHICH_TARGET DEBUG_ROOT RELEASE_ROOT)
	if (EXISTS ${pbd})
		# 获取DEBUG属性
		get_property(debug_bin_file TARGET ${WHICH_TARGET} PROPERTY IMPORTED_LOCATION_DEBUG)
		# 拷贝DLL到输出目录
		copy_file_macro(${debug_bin_file} ${DEBUG_ROOT})
	endif()

	if (EXISTS ${pbr})
		# 获取 RELEASE 属性
		get_property(release_bin_file TARGET ${WHICH_TARGET} PROPERTY IMPORTED_LOCATION_RELEASE)
		# 拷贝DLL到输出目录
		copy_file_macro(${release_bin_file} ${RELEASE_ROOT})
	endif()
endmacro( )

# 
# 检查文件是否存在
#
function(CHECK_EXISTS PARAM)
    if (NOT EXISTS ${PARAM})
        message(FATAL_ERROR "${PARAM}不存在，请检查")
    endif()
endfunction()

# 
# 设置Qt的一些组件
#
macro(SET_QT_PROJECT_PROPERTIES )
    set(options)
    set(oneValueArgs TARGET_NAME )

    # REQUIRED_LIBS_QUALIFIED - 有这么多的Qt库需要链接
    set(multiValueArgs REQUIRED_LIBS_QUALIFIED)
    # 解析参数
    cmake_parse_arguments(SQPPRLQ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # CMAKE包查找
    find_package(Qt${QT_MAJOR_VERSION} COMPONENTS ${SQPPRLQ_REQUIRED_LIBS_QUALIFIED} REQUIRED)
endmacro()

# 
# 设置查找项目对应的库文件
#
macro(FIND_EXT_LIB_PACKAGES)
    set(options)
    set(oneValueArgs )
    set(multiValueArgs LIB_CMAKE_NAMES)
    # 解析参数
    cmake_parse_arguments(FELP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    
    foreach(EACH_NAME ${FELP_LIB_CMAKE_NAMES})
        find_package(${EACH_NAME} REQUIRED)
    endforeach(EACH_NAME ${FELP_LIB_CMAKE_NAMES})
endmacro(FIND_EXT_LIB_PACKAGES)

#
# 设置项目的头文件包含路径
#
function(SET_PROJECT_INCLUDE_ROOT )

    set(options)
    # VISIBLE_FLAG - PRIVATE PUBLIC 
    # TARGET_NAME - 哪个项目
    set(oneValueArgs TARGET_NAME VISIBLE_FLAG)

    # INCLUDE_ROOT - 有这么多的头文件路径搜索需要设置
    set(multiValueArgs INCLUDE_ROOT)
    # 解析参数
    cmake_parse_arguments(SPIR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    foreach(EACH_ROOT ${SPIR_INCLUDE_ROOT})
        target_include_directories(${SPIR_TARGET_NAME} ${SPIR_VISIBLE_FLAG} ${EACH_ROOT})
    endforeach()
endfunction()


#
# 设置项目链接库文件
#
function(SET_PROJECT_LINK_LIBS )

    set(options)
    # VISIBLE_FLAG - PRIVATE PUBLIC 
    # TARGET_NAME - 哪个项目
    set(oneValueArgs TARGET_NAME VISIBLE_FLAG)

    # LIB_NAMES - 有这么多的头文件路径搜索需要设置
    set(multiValueArgs LIB_NAMES)
    # 解析参数
    cmake_parse_arguments(SPLL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    target_link_libraries(${SPLL_TARGET_NAME} ${SPLL_VISIBLE_FLAG} ${SPLL_LIB_NAMES})

endfunction()

#
# 设置项目管理员权限
#
macro(USE_ADMINISTRATOR WHICH_TARGET)
    # 如果是windows，设置管理员权限。注意使用if需要配合endif使用
    if (MSVC)
        set_target_properties(${WHICH_TARGET} PROPERTIES LINK_FLAGS "/level='requireAdministrator'" )
    endif()
endmacro()

#
# 关闭控制台
#
macro(CLOSE_RELEASE_CONSOLE)
    # 关闭控制台
    if (MSVC)
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endmacro()

#
# 设置MSVC启用UTF8编码
#
function(SRC_FILES_ENCODE_UTF8 WHICH_TARGET)
    if(MSVC)
        target_compile_options( ${WHICH_TARGET} 
        PRIVATE /GS
        PRIVATE /source-charset:utf-8 /execution-charset:gb2312 
        )
    endif()
endfunction()



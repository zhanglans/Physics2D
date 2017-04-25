cmake_minimum_required(VERSION 2.8)

# œ‘ æ±‡“Î—°œÓ
SET(CMAKE_VERBOSE_MAKEFILE ON)

project (Physics2D)


set (Test_VERSION_MAJOR 2)
set (Test_VERSION_MINOR 0)


set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /Zm1000 /EHsc /GR /MP")
# Flags used by the compiler during debug builds.
set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
# Flags used by the compiler during version builds.
set(CMAKE_CXX_FLAGS_VERSION "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
# Flags used by the compiler during release minsize builds.
set(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /O1 /Ob1 /D NDEBUG")
# Flags used by the compiler during release builds (/MD /Ob1 /Oi /Ot /Oy /Gs will produce slightly less optimized but smaller files).
set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /D NDEBUG")
# Flags used by the compiler during Release with Debug Info builds.
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /D NDEBUG")
# Flags used by the compiler during all build types.
set(CMAKE_C_FLAGS "/DWIN32 /D_WINDOWS /W3 /Zm1000")
# Flags used by the compiler during debug builds.
set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MDd /Zi  /Ob0 /Od /RTC1")
# Flags used by the compiler during version builds.
set(CMAKE_C_FLAGS_VERSION "/D_DEBUG /MDd /Zi  /Ob0 /Od /RTC1")
# Flags used by the compiler during release minsize builds.
set(CMAKE_C_FLAGS_MINSIZEREL "/MD /O1 /Ob1 /D NDEBUG")
# Flags used by the compiler during release builds (/MD /Ob1 /Oi /Ot /Oy /Gs will produce slightly less optimized but smaller files).
set(CMAKE_C_FLAGS_RELEASE "/MD /O2 /Ob2 /D NDEBUG")
# Flags used by the compiler during Release with Debug Info builds.
set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /D NDEBUG")


set(P2D_ROOT_PATH "${PROJECT_SOURCE_DIR}")

set(P2D_SOURCE_DIR "${P2D_ROOT_PATH}/src")

set(P2D_INCLUDE_DIRS
	${P2D_SOURCE_DIR}
	${P2D_SOURCE_DIR}/Utils
	${P2D_SOURCE_DIR}/Shape
	${P2D_SOURCE_DIR}/Collide
	${P2D_SOURCE_DIR}/Dynamics)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${P2D_ROOT_PATH}/lib/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${P2D_ROOT_PATH}/lib/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${P2D_ROOT_PATH}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${P2D_ROOT_PATH}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${P2D_ROOT_PATH}/bin")

if (NOT NDEBUG)
	set(P2D_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")
else()
	set(P2D_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release")
endif()

set(EXTERNAL_LIBS_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})




macro(group_files_by_dir src_files)
        set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(group_files ${${src_files}})
			#message(${group_files})
            string(REGEX REPLACE ${cur_dir}/\(.*\) \\1 sgbd_fpath ${group_files})
			#message(${sgbd_fpath})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
			#$message(${sgbd_group_name})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "\\" "/" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "Common")
            endif(sgbd_nogroup)
	
			source_group(${sgbd_group_name} FILES ${group_files})
        endforeach(group_files)
endmacro(group_files_by_dir)


# This seems to break Xcode projects so definitely don't enable on Apple builds
set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

#option (USE_MYMATH "Use MyMath lib" ON)

#configure_file (
#	"${PROJECT_SOURCE_DIR}/TestConfig.h.a"
#	"${PROJECT_BINARY_DIR}/TestConfig.h"
#)

add_subdirectory("src")
add_subdirectory("Physics2DTest")





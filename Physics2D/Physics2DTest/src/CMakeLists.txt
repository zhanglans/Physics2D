
	
include_directories(${P2D_INCLUDE_DIRS})

link_directories(${P2D_LIB_DIR} ${EXTERNAL_LIBS_DIR})

link_libraries(Physics2D.lib glut32.lib)

file(GLOB_RECURSE P2D_TEST_HEADER_FILES *.h *.inl)
file(GLOB_RECURSE P2D_TEST_SOURCE_FILES *.cpp *.cc *.c *.cxx)

set(ALL_FILES ${P2D_TEST_HEADER_FILES} ${P2D_TEST_SOURCE_FILES})
group_files_by_dir(ALL_FILES)

add_executable(Physics2DTest ${P2D_TEST_HEADER_FILES} ${P2D_TEST_SOURCE_FILES})

add_dependencies(Physics2DTest Physics2D)

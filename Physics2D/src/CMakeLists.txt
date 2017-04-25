message(STATUS "Configuring src")

add_subdirectory("Utils")
add_subdirectory("Shape")
add_subdirectory("Collide")
add_subdirectory("Dynamics")

include_directories(${P2D_INCLUDE_DIRS})

file(GLOB_RECURSE HEADER_FILES *.h *.inl)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.cc *.c *.cxx)


set(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})
group_files_by_dir(ALL_FILES)

add_library(Physics2D STATIC ${HEADER_FILES} ${SOURCE_FILES})

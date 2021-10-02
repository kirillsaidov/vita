cmake_minimum_required(VERSION 3.0)

# project name init
project(vita
		VERSION 1.0
		DESCRIPTION "Vita - a custom C library for memory management, string manipulation and more."
		LANGUAGES C)

# setting default build type
set(default_buid_type "Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# finding all headers and sources 
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/inc/)

# building library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

#message("Installing the library to lib/...")
#install(TARGETS ${PROJECT_NAME}
#		ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})





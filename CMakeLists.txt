cmake_minimum_required(VERSION 3.0)

# project name init
project(vita
		VERSION 1.0
		DESCRIPTION "Vita - a custom C library for memory management, string manipulation and more."
		LANGUAGES C)

# setting default build type
set(default_buid_type "Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../lib")

# finding all headers and sources 
file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

# include directories
include_directories(include)

# create lib destination folder
#file(MAKE_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# building library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

message("Installing...")
install(TARGETS ${PROJECT_NAME}
		ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})





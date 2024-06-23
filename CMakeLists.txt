cmake_minimum_required(VERSION 3.8)

# project name init
project(
    vita
    VERSION 0.5.0
    DESCRIPTION "Vita - a custom C library for memory management, string manipulation and more."
    LANGUAGES C
)

# setting default build type
set(CMAKE_C_STANDARD 11)
set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "-Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")
# set(DEFAULT_BUILD_TYPE "Release")
# set(CMAKE_C_FLAGS "-DNDEBUG -Wall -Wpedantic -Wextra -Wreturn-type -Wswitch -Wunused -Werror -O2")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
endif()

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





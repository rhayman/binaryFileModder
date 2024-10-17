cmake_minimum_required(VERSION 3.22)
project(BinFileModder VERSION 0.0.1 DESCRIPTION "modifies a binary file by adding or removing columns or rows" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# generate the compile_commands.json file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include( FetchContent )

message(STATUS "Fetching argparse...")
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)
message(STATUS "Fetching indicators...")
FetchContent_Declare(
  indicators
  GIT_REPOSITORY https://github.com/p-ranav/indicators.git
)
FetchContent_MakeAvailable(indicators)

set( SOURCES src/main.cpp )
add_executable( ${PROJECT_NAME} ${SOURCES} )
target_link_libraries( ${PROJECT_NAME}
 PUBLIC
 argparse
 indicators
)

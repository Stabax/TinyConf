cmake_minimum_required(VERSION 2.6)

project (tinyconf_tests)

#CORE FILES
file(GLOB_RECURSE
     source_files
     ./*
)
include_directories(${CMAKE_SOURCE_DIR})

#Global Parameter
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#Build sequence
add_executable(tinyconf_tests ${source_files})
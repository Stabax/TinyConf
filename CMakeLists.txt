cmake_minimum_required(VERSION 3.2)

project(TinyConf
        VERSION 1.0.1
        LANGUAGES CXX)


# Full Unit Test binary

file(GLOB FullTestSrc "./tests/test.cpp")

add_executable(${PROJECT_NAME}_fulltest ${FullTestSrc})

set_target_properties(${PROJECT_NAME}_fulltest PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Speed Test binary

file(GLOB FullTestSrc "./tests/speed.cpp")

add_executable(${PROJECT_NAME}_speedtest ${FullTestSrc})

set_target_properties(${PROJECT_NAME}_speedtest PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Continuous integration

if (CMAKE_BUILD_TYPE STREQUAL "Coverage" AND CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME ${PROJECT_NAME}_coverage
        EXECUTABLE ${PROJECT_NAME}_test
        DEPENDENCIES ${PROJECT_NAME}_test)
endif()

# Include

include_directories(include)
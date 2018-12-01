##======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============##
##
## Purpose:     IVS Project 1 - Solution CMakeFile
##
## $NoKeywords: $ivs_project_1 $CMakeLists.txt
## $Authors:    Filip Vaverka <ivaverka@fit.vutbr.cz>
##              David Grochol <igrochol@fit.vutbr.cz>
## $Date:       $2017-01-04
##============================================================================##

cmake_minimum_required(VERSION 2.8.2)
project(ivs_proj_1)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage.cmake)
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

include(GoogleTest.cmake)

# Test targets
enable_testing()

find_library(BLACK_BOX_LIBS black_box_lib REQUIRED PATHS libs NO_DEFAULT_PATH)
include_directories("libs")

add_executable(black_box_test black_box_tests.cpp)
target_link_libraries(black_box_test ${BLACK_BOX_LIBS} gtest_main)
GTEST_ADD_TESTS(black_box_test "" black_box_tests.cpp)

add_executable(white_box_test white_box_tests.cpp white_box_code.cpp)
target_link_libraries(white_box_test gtest_main)
GTEST_ADD_TESTS(white_box_test "" white_box_tests.cpp)
if(CMAKE_COMPILER_IS_GNUCXX)
    SETUP_TARGET_FOR_COVERAGE(white_box_test_coverage white_box_test white_box_test_coverage)
endif()

add_executable(tdd_test tdd_code.cpp tdd_tests.cpp)
target_link_libraries(tdd_test gtest_main)
GTEST_ADD_TESTS(tdd_test "" tdd_tests.cpp)
if(CMAKE_COMPILER_IS_GNUCXX)
    SETUP_TARGET_FOR_COVERAGE(tdd_test_coverage tdd_test tdd_test_coverage)
endif()

if(CMAKE_VERSION VERSION_GREATER 3.2.0)
    add_custom_target(pack COMMAND
        ${CMAKE_COMMAND} -E tar "cfv" "xlogin00.zip" --format=zip
            "${CMAKE_CURRENT_SOURCE_DIR}/black_box_tests.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/white_box_tests.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/tdd_code.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/tdd_code.cpp")
endif()

#add_test(NAME ivs_proj_1_black_box_test COMMAND ivs_proj_1_black_box --gtest_filter=example_test1.*)

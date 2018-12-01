##======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============##
##
## Purpose:     IVS Project 1 - CMakeFile (except from FindGTest.cmake)
##
## $NoKeywords: $ivs_project_1 $CMakeLists.txt
## $Authors:    Filip Vaverka <ivaverka@fit.vutbr.cz>
##              David Grochol <igrochol@fit.vutbr.cz>
## $Date:       $2017-01-04
##============================================================================##

function(GTEST_ADD_TESTS executable extra_args)
    if(NOT ARGN)
        message(FATAL_ERROR "Missing ARGN: Read the documentation for GTEST_ADD_TESTS")
    endif()
    if(ARGN STREQUAL "AUTO")
        # obtain sources used for building that executable
        get_property(ARGN TARGET ${executable} PROPERTY SOURCES)
    endif()
    set(gtest_case_name_regex ".*\\( *([A-Za-z_0-9]+) *, *([A-Za-z_0-9]+) *\\).*")
    set(gtest_test_type_regex "(TYPED_TEST|TEST_?[FP]?)")
    foreach(source ${ARGN})
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${source})
        file(READ "${source}" contents)
        string(REGEX MATCHALL "${gtest_test_type_regex} *\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
        foreach(hit ${found_tests})
          string(REGEX MATCH "${gtest_test_type_regex}" test_type ${hit})

          # Parameterized tests have a different signature for the filter
          if("x${test_type}" STREQUAL "xTEST_P")
            string(REGEX REPLACE ${gtest_case_name_regex}  "*/\\1.\\2/*" test_name ${hit})
          elseif("x${test_type}" STREQUAL "xTEST_F" OR "x${test_type}" STREQUAL "xTEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1.\\2" test_name ${hit})
          elseif("x${test_type}" STREQUAL "xTYPED_TEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1/*.\\2" test_name ${hit})
          else()
            message(WARNING "Could not parse GTest ${hit} for adding to CTest.")
            continue()
          endif()
          add_test(NAME ${test_name} COMMAND ${executable} --gtest_filter=${test_name} ${extra_args})
        endforeach()
    endforeach()
endfunction()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/googletest-master)
    configure_file(CMakeLists.txt.in googletest/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest)
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest)
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-master 
                 ${CMAKE_BINARY_DIR}/googletest-build)

if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

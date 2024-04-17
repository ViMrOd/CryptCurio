# @file CMakeLists.txt
#
# CMake file for big number library test

file(GLOB TESTS "*.sh")
foreach(TEST ${TESTS})

    get_filename_component(TESTNAME "${TEST}" NAME_WLE)

    add_test(NAME ${TESTNAME}
         COMMAND ${CMAKE_SOURCE_DIR}/testsuite/${TESTNAME}.sh
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

endforeach()


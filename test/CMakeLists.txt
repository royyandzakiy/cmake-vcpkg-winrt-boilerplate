add_executable(
    unit_tests
    test_calc.cpp
    test_run_calc.cpp
)
target_link_libraries(unit_tests PRIVATE 
    run_calc 
    GTest::gtest 
    GTest::gtest_main
    # GTest::gmock 
    # GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)

# Python System Tests (if Python is available)
if(ENABLE_PYTHON_TESTS AND Python3_Interpreter_FOUND)
    # Add a test that will be discovered by CTest
    add_test(
        NAME python_system_tests
        COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/test/python -v
    )
    
    # Set a property to mark this as a system test
    set_tests_properties(python_system_tests PROPERTIES
        LABELS "system"
        TIMEOUT 60
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    message(STATUS "Python system tests enabled")
else()
    message(STATUS "Python system tests disabled")
endif()
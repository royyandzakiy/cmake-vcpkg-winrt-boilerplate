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
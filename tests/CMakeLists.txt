# NutVIC/tests

add_executable(cpu_tests
	add_tests.cpp
)


target_sources(cpu_tests PRIVATE

)

target_link_libraries(cpu_tests GTest::gtest_main)

set_property(TARGET cpu_tests PROPERTY CXX_STANDARD 26)

include(GoogleTest)
gtest_discover_tests(cpu_tests)

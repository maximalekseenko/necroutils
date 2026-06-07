include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(logger_tests logger_tests.cpp)
target_link_libraries(logger_tests PRIVATE necroutils GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(logger_tests)

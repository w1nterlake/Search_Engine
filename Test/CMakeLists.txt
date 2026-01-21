include(FetchContent)

enable_testing()


set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  URL "https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip"
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test.cpp
    # headers можно не перечислять, но пусть будут
    test.h
    test_Search.h
)

target_include_directories(unit_tests
    PRIVATE
        ${PROJECT_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/Test
)

target_link_libraries(unit_tests
    PRIVATE
        search_engine_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)

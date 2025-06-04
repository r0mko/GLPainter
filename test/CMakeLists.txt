include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(GLPainterTests sample_test.cpp)

target_link_libraries(GLPainterTests PRIVATE GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(GLPainterTests)

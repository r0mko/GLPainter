include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)

add_executable(GLPainterTests
    sample_test.cpp
    text_buffer_test.cpp
)

target_link_libraries(GLPainterTests PRIVATE
    GTest::gtest_main
    TextViewCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(GLPainterTests PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/TextView/lib
)

include(GoogleTest)

gtest_discover_tests(GLPainterTests)

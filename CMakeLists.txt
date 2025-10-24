cmake_minimum_required(VERSION 3.16)
project(rv32i_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O1")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_subdirectory(third_party/loguru)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.cpp
)

add_executable(rv32i main.cpp ${SOURCES})

target_include_directories(rv32i PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/ELFIO
)

# target_link_libraries(rv32i PRIVATE loguru)

find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/tests/source/*.cpp
)

add_executable(tests tests/main.cpp ${SOURCES} ${TEST_SOURCES})

target_compile_features(tests PUBLIC cxx_std_20)

target_include_directories(tests PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests
    GTest::gtest 
    GTest::gtest_main
)

enable_testing()
add_test(NAME UnitTests COMMAND tests)



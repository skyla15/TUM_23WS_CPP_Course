

# reference : https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
# https://modoocode.com/332#page-heading-7
 

# specifying the minimum version
cmake_minimum_required(VERSION 3.14)

# add combnatorics as a library to hw02
add_library(hw02 combinatorics.cpp)

# add current directory to be included in hw02
target_include_directories(hw02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(runhw02 run.cpp)

target_link_libraries(runhw02 PRIVATE hw02)

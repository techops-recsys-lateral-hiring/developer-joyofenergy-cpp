cmake_minimum_required(VERSION 3.17)
project(developer_joyofenergy_cpp_beast)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(rest)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE rest)

enable_testing()
add_subdirectory(test)
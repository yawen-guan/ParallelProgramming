project(add)
cmake_minimum_required (VERSION 2.8)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -mavx -mavx2 -O0")

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_executable(add "${PROJECT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(add lib)




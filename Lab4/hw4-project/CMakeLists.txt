project(vecMul)
cmake_minimum_required (VERSION 2.8)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(vecMul_Foster "${PROJECT_SOURCE_DIR}/vecMul_Foster.cpp")

add_executable(vecMul_Normal "${PROJECT_SOURCE_DIR}/vecMul_Normal.cpp")







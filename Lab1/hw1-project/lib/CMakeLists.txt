set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -mavx -mavx2 -O0")

add_library(lib dataMaker.cpp addNormal.cpp addUsingAVX.cpp addUsingThreads.cpp)


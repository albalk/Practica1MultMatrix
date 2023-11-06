cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(serverMultMatrix LANGUAGES CXX)
add_executable(serverMultMatrix utils.h utils.cpp serverMultMatrix.cpp multmatrix.cpp multmatrix.h multMatrix_imp.h operaciones.h)
target_link_libraries(serverMultMatrix pthread)

project(clientMultMatrix LANGUAGES CXX)
add_executable(clientMultMatrix utils.h utils.cpp clientMultMatrix.cpp multMatrix_stub.h operaciones.h)
target_link_libraries(clientMultMatrix pthread)

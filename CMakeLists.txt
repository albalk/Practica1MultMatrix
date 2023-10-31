cmake_minimum_required (VERSION 2.8.11)

project (multMatrix)

add_executable (multMatrix main_multMatrix.cpp multmatrix.cpp multmatrix.h)


project(serverMultMatrix LANGUAGES CXX)
add_executable(serverMultMatrix utils.h utils.cpp serverMultMatrix.cpp multmatrix.h multMatrix_imp.h operaciones.h)
target_link_libraries(serverMultMatrix pthread)

project(clientMultMatrix LANGUAGES CXX)
add_executable(clientMultMatrix utils.h utils.cpp clientMultMatrix.cpp multMatrix_stub.h operaciones.h)
target_link_libraries(clientMultMatrix pthread)

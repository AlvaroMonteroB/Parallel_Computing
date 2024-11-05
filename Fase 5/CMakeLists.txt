cmake_minimum_required(VERSION 3.29)
project(Fase_5 C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)
add_compile_options(${MPI_COMPILE_FLAGS})


add_executable(Fase_5 main.c)
target_link_libraries(Fase_5 ${MPI_LIBRARIES} m)

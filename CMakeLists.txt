cmake_minimum_required(VERSION 3.10)
project(MiMotorJuego)

set(CMAKE_CXX_STANDARD 17)

# Buscar SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(MiMotorJuego ${SOURCES})

# Enlazar la biblioteca SDL2
target_link_libraries(MiMotorJuego ${SDL2_LIBRARIES})
cmake_minimum_required(VERSION 3.16)
project(cppweb)

set(CMAKE_CXX_STANDARD 17)

# Incluir Crow desde su ubicación en el sistema
include_directories(/usr/local/src/crow/include)

# Archivo fuente principal
add_executable(cppweb src/main.cpp)

# Linkear la librería SSL (necesaria para Crow)
target_link_libraries(cppweb ssl crypto pthread)

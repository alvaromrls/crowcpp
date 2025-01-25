# Requerimientos mínimos de CMake
cmake_minimum_required(VERSION 3.16)

# Nombre del proyecto
project(cppweb)

# Configuración del estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir directorios de Crow y otras dependencias
include_directories(/usr/local/src/crow/include)
include_directories(include)

# Configurar el directorio de salida para los binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Encuentra OpenSSL para asegurar que OpenSSL::Crypto esté disponible
find_package(OpenSSL REQUIRED)

# Encuentra cURL
find_package(CURL REQUIRED)
# --------------------------------
# Aplicación principal
# --------------------------------
# Archivos fuente principales
add_executable(cppweb src/main.cpp src/server.cpp src/hash256.cpp src/HexUtils.cpp)

# Linkear librerías necesarias
target_link_libraries(cppweb ssl crypto pthread)

# --------------------------------
# Tests unitarios
# --------------------------------
# Habilitar testing
enable_testing()

# Asegúrate de que Google Test está disponible
find_package(GTest REQUIRED)

# Añadir ejecutable para los tests
add_executable(unit_tests tests/main_test.cpp src/server.cpp src/hash256.cpp src/HexUtils.cpp)
add_executable(hash256_tests tests/hash256_tests.cpp src/hash256.cpp src/HexUtils.cpp)

# Vincular Google Test y pthread al ejecutable de tests
target_link_libraries(unit_tests PRIVATE gtest gtest_main pthread curl ssl crypto )
target_link_libraries(hash256_tests PRIVATE gtest gtest_main ssl crypto pthread)

# Registrar los tests para que puedan ejecutarse con `ctest`
add_test(NAME RunTests COMMAND unit_tests)
add_test(NAME HASH_256_TESTS COMMAND hash256_tests)

# Opcional: Mostrar mensajes adicionales para facilitar la depuración
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

cmake_minimum_required(VERSION 3.5.0)
project(helloworld VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
# SET(CMAKE_EXE_LINKER_FLAGS  "/FORCE")

include(CTest)
enable_testing()

include_directories(
  "C:\\vcpkg\\installed\\x64-windows\\include"
)
link_directories(
  "C:\\vcpkg\\installed\\x64-windows\\lib"
)

file(GLOB_RECURSE sources src/*.cpp src/*.cc src/*.h src/*.hpp)

add_executable(main ${sources})

find_package(Crow CONFIG REQUIRED)
target_link_libraries(main Crow::Crow)

find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(main cppzmq cppzmq-static)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(main rapidjson)

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(main libpqxx::pqxx)

find_package(unofficial-restbed CONFIG REQUIRED)
target_link_libraries(main unofficial::restbed::restbed)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

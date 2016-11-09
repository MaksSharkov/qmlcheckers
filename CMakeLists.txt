cmake_minimum_required (VERSION 3.0.1)
project(qmlcheckers)

set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)

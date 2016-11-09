cmake_minimum_required (VERSION 3.0.1)

project(server)

find_package(Qt5Core)
find_package(Qt5WebSockets)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5WebSockets_INCLUDE_DIRS})

file(GLOB_RECURSE SERVER_SRC ${PROJECT_SOURCE_DIR}/src *.cpp)
file(GLOB_RECURSE SERVER_HEADERS ${PROJECT_SOURCE_DIR}/include *.h)

add_executable(${PROJECT_NAME} ${SERVER_SRC} ${SERVER_HEADERS})
target_link_libraries(${PROJECT_NAME}
        ${Qt5Core_LIBRARIES} ${Qt5WebSockets_LIBRARIES} shared)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

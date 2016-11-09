cmake_minimum_required (VERSION 3.1.0)

project(shared)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5WebSockets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5WebSockets_INCLUDE_DIRS})

file(GLOB_RECURSE SHARED_SRC ${PROJECT_SOURCE_DIR} *.cpp)

add_library(${PROJECT_NAME} STATIC ${SHARED_SRC})
target_link_libraries(${PROJECT_NAME} ${Qt5Core_LIBRARIES} ${Qt5WebSockets_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)


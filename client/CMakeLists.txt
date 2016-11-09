cmake_minimum_required (VERSION 3.0.1)

project(client)

find_package(Qt5Qml)
find_package(Qt5Quick)
include_directories(${Qt5Qml_INCLUDE_DIRS} ${Qt5Quick_INCLUDE_DIRS})

file(GLOB_RECURSE CLIENT_SRC ${PROJECT_SOURCE_DIR} *.cpp)
QT5_ADD_RESOURCES(CLIENT_RESOURCE_FILE "resources.qrc")

add_executable(${PROJECT_NAME} ${CLIENT_SRC} ${CLIENT_RESOURCE_FILE})
target_link_libraries(${PROJECT_NAME}
	${Qt5Qml_LIBRARIES} ${Qt5Quick_LIBRARIES} shared)

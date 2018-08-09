cmake_minimum_required(VERSION 3.12)
project(vrcreader)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

set(SRC
        src/main.cpp
        src/mainwindow.cpp
#        src/mainwindow.ui
        src/vrcplayer.cpp
        src/vrcreader.cpp)
set(HEADERS
        include/mainwindow.h
        include/vrcplayer.h
        include/vrcreader.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui)

cmake_minimum_required(VERSION 3.20)

project(FramelessWindow)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64/lib/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

option(BUILD_DEMO "Build the demo" ON)

#add_library(FramelessWindow INTERFACE)
#
#target_include_directories(
#        FramelessWindow INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>")
#target_link_libraries(FramelessWindow INTERFACE
#        Qt${QT_VERSION_MAJOR}::Core
#        Qt${QT_VERSION_MAJOR}::Gui
#        Qt${QT_VERSION_MAJOR}::Widgets)

if(BUILD_DEMO)
    add_subdirectory(example)
endif()
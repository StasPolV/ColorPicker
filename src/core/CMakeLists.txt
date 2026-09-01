cmake_minimum_required(VERSION 3.28)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)
qt_standard_project_setup()

qt_add_executable(core ScreenGrabber.h ScreenGrabber.cpp)

target_link_libraries(core PRIVATE Qt6::Core Qt6::Gui)

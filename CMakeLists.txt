cmake_minimum_required(VERSION 3.24) #wxWidgets needs this version for find_package

project(JIKKEN
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "Jikken software tool for Japan MQP"
)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(src)
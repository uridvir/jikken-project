add_library(jikken-camera OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraController.cpp
)

target_include_directories(jikken-camera PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
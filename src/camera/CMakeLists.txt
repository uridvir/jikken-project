add_library(jikken-camera OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraController.cpp
)

target_link_libraries(jikken-camera PUBLIC jikken-cameramenu)

target_include_directories(jikken-camera PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
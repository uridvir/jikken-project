add_library(jikken-camera OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraSerial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraVideoStream.cpp
)

target_link_libraries(jikken-camera PUBLIC 
    jikken-cameramenu
    serialib
    ${OpenCV_LIBS}
    jikken-properties
)

target_include_directories(jikken-camera PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_subdirectory(gui)
add_subdirectory(camera)
add_subdirectory(cameramenu)
add_subdirectory(bubble)
add_subdirectory(properties)

add_executable(jikken WIN32 jikken.cpp) #WIN32 tells CMake to invoke WinMain instead of main, is ignored when not on Windows
target_link_libraries(jikken
    ${wxWidgets_LIBRARIES}
    ${OpenCV_LIBS}
    serialib
    jikken-gui
    jikken-camera
    wxImagePanel
    jikken-properties
    bubble
)

add_executable(video_test video_test.cpp)
target_link_libraries(video_test
    ${OpenCV_LIBS}
)
add_executable(jikken WIN32 jikken.cpp) #WIN32 tells CMake to invoke WinMain instead of main, is ignored when not on Windows
target_link_libraries(jikken
    ${wxWidgets_LIBRARIES}
    ${OpenCV_LIBS}
    serialib
)
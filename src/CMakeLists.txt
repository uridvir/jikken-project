add_executable(HelloWorldApp WIN32 HelloWorldApp.cpp) #WIN32 is ignored when not on Windows
target_link_libraries(HelloWorldApp
    ${wxWidgets_LIBRARIES}
    #${OpenCV_LIBS}
)
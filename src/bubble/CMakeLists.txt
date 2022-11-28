add_library(video-cleaner-lib OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoCleaner.cpp
)
target_include_directories(video-cleaner-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(bubble-counter-lib OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/BubbleCounter.cpp
)
target_include_directories(bubble-counter-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(video-clean video-clean.cpp)
target_link_libraries(video-clean
    ${OpenCV_LIBS}
    video-cleaner-lib
)

add_executable(bubble-count bubble-count.cpp)
target_link_libraries(bubble-count
    ${OpenCV_LIBS}
    bubble-counter-lib
)
add_library(jikken-cameramenu INTERFACE)

target_link_libraries(jikken-cameramenu INTERFACE jikken-camera)

target_include_directories(jikken-cameramenu INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
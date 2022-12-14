add_library(jikken-gui OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/JikkenFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DownloadPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertiesFrame.cpp
)

target_link_libraries(jikken-gui PUBLIC
    ${wxWidgets_LIBRARIES}
    jikken-camera
    jikken-properties
    bubble
)

target_include_directories(jikken-gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
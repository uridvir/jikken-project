add_library(jikken-gui OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/JikkenFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VideoPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsPanel.cpp
)
target_include_directories(jikken-gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
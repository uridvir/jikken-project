add_library(jikken-properties OBJECT
    # ${CMAKE_CURRENT_SOURCE_DIR}/JikkenGlobals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JikkenPropertiesHolder.cpp
)

target_include_directories(jikken-properties PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
cmake_minimum_required(VERSION 3.24) #wxWidgets needs this version for find_package

project(JIKKEN
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "Jikken software tool for Japan MQP"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
include(platform_settings) #Platform-specific settings

# -----------------
# PREBUILT PACKAGES
# -----------------

find_package(wxWidgets REQUIRED COMPONENTS net core base) #Added wx directory to WXWIN env var
include(${wxWidgets_USE_FILE})

find_package(OpenCV REQUIRED) #Platform-specific install directory
include_directories(${OpenCV_INCLUDE_DIRS})

# --------------------------
# PACKAGES BUILT FROM SOURCE
# --------------------------

include(FetchContent)

FetchContent_Declare(
    serialib
    GIT_REPOSITORY  https://github.com/imabot2/serialib.git
    GIT_TAG         57e817cebec10dede3cb70006886269f9c7809cc #latest version as of 11/2/2022
)
FetchContent_Declare(
    wxImagePanel
    GIT_REPOSITORY  https://github.com/tomay3000/wxImagePanel.git
    GIT_TAG         34006f34e592f5578ec4758aeec2aaaba595af4c #latest version as of 11/29/2022
)
FetchContent_MakeAvailable(
    serialib
    wxImagePanel
)

FetchContent_GetProperties(serialib)
include_directories(${serialib_SOURCE_DIR}/lib)
add_library(serialib OBJECT ${serialib_SOURCE_DIR}/lib/serialib.cpp)

FetchContent_GetProperties(wxImagePanel)
include_directories(${wximagepanel_SOURCE_DIR}) #Dependency name must be lowercase here
add_library(wxImagePanel OBJECT ${wximagepanel_SOURCE_DIR}/imagepanel.cpp)
# target_link_libraries(wxImagePanel wxWidgets)

add_subdirectory(src)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
install(TARGETS
    jikken
    # video-clean
    # bubble-count
CONFIGURATIONS Release #Only deploy in "Release" mode
RUNTIME DESTINATION bin)
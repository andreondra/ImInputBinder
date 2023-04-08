cmake_minimum_required(VERSION 3.2)
project(ImInputBinder)
set(CMAKE_CXX_STANDARD 17)
option(BUILD_EXAMPLE "Include dependencies to run the example." OFF)

##########################################################
# Prepare hello_imgui and add example.
##########################################################
if(BUILD_EXAMPLE)
    include(FetchContent)
    FetchContent_Declare(
            hello_imgui
            GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
    )
    FetchContent_MakeAvailable(hello_imgui)
    list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
    include(hello_imgui_add_app)

    hello_imgui_add_app(ImInputBinderExample main.cpp ImInputBinder.cpp)
endif()

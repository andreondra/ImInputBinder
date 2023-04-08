cmake_minimum_required(VERSION 3.25)
project(ImInputBinder)
set(CMAKE_CXX_STANDARD 17)

##########################################################
# Prepare hello_imgui during configure time.
##########################################################
include(FetchContent)
FetchContent_Declare(
        hello_imgui
        GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
)
FetchContent_MakeAvailable(hello_imgui)
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include(hello_imgui_add_app)

##########################################################
# Build ImInputBinder example.
##########################################################
hello_imgui_add_app(ImInputBinderExample main.cpp ImInputBinder.cpp)

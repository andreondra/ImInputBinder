# ImInputBinder
Input binding widget for Dear ImGui library.

## How to build and run the example
### Linux
In the project directory execute:
```shell
mkdir build
cd build
cmake -DBUILD_EXAMPLE=ON ..
make
./ImInputBinderExample
```

## How to use
### Prerequisites
- C++11 (C++17 to run the example),
- `imgui.h` available the in include path.

### 1) Get the sources
You can add the widget to your project directly by downloading ZIP of the repo and adding ImInputBinder.h to your include path
and ImInputBinder.cpp to your build.

Or, if you use CMake, you can download the sources using this CMake snippet:
```cmake
include(FetchContent)
FetchContent_Declare(
        ImInputBinder
        GIT_REPOSITORY https://github.com/andreondra/ImInputBinder
)
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include_directories(${ImInputBinder_SOURCE_DIR})
file(GLOB ImInputBinder_sources ${ImInputBinder_SOURCE_DIR}/ImInputBinder.cpp)

# Do not forget to add ${ImInputBinder_sources} to your build. For example like this:
add_executable(your_app mySource1.cpp mySource2.cpp mySourceX.cpp ${ImInputBinder_sources})
```
### 2) Add to code
#### Setup
You have to create an instance of ImInputBinder:
```c++
ImInputBinder imInputBinder;
```

Then you add actions. The action is a representation of a reaction to the specified user input. It consists of:
- `name_id`: a unique name,
- `key`: a key (or a button) which will trigger callbacks,
- `pressCallback` and `releaseCallback`: callbacks for press and release of the specified key/button,

```c++
imInputBinder.addAction({
    .name_id ="Example Up Arrow",
    .key = ImGuiKey_UpArrow,
    .pressCallback = [](){ std::cout << "Example Up Arrow press callback." << std::endl; },
    .releaseCallback = [](){ std::cout << "Example Up Arrow release callback." << std::endl; },
});
```

You can also add multiple actions at a same time using addActions:
```c++
imInputBinder.addActions({
    {
        .name_id = "Test A key press",
        .key = ImGuiKey_A,
        .pressCallback = [](){ std::cout << "A key pressed!" << std::endl; }
    },
    {
            .name_id = "Test B key release",
            .key = ImGuiKey_B,
            .releaseCallback = [](){ std::cout << "B key released!" << std::endl; }
    },
});
```

Then you can load existing bindings from a file. You can specify a file name, stream in a binary mode, or nothing and
a default file will be used.
```c++
imInputBinder.loadBindings("myfile.iib");
```

#### Running
After the setup is complete, you have the call `imInputBinder.update()` periodically to check for keys pressed.

To show a configuration dialog, call `imInputBinder.renderEditorWindow()` to render the dialog in its own window, or
`imInputBinder.renderEditor()` to render only contents (you have to provide the ImGui window).

#### Saving the state
To save the state, you can call `imInputBinder.saveBindings()` with same parameters as `loadBindings()`.

## License
This project is licensed under the MIT license.

© 2023, Ondrej Golasowski
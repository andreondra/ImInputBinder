#include <iostream>
#include "hello_imgui/hello_imgui.h"
#include "ImInputBinder.h"

ImInputBinder imInputBinder;

void run() {
    imInputBinder.renderEditorWindow();
    imInputBinder.update();
}

int main(int , char *[])
{

    imInputBinder.addAction({
        .name_id ="Example Up Arrow",
        .key = ImGuiKey_UpArrow,
        .pressCallback = [](){ std::cout << "Example Up Arrow press callback." << std::endl; },
        .releaseCallback = [](){ std::cout << "Example Up Arrow release callback." << std::endl; },
    });

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

    imInputBinder.loadBindings();

    HelloImGui::Run(
            &run,
            "ImInputBinder example",
            false,
            false,
            {1280, 720}
    );

    imInputBinder.saveBindings();

    return 0;
}

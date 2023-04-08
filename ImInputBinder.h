#ifndef IMINPUTBINDER_IMINPUTBINDER_H
#define IMINPUTBINDER_IMINPUTBINDER_H

#include <string>
#include <map>
#include <functional>
#include <fstream>
#include "imgui.h"

/**
 * ImGui input binding helper.
 * */
class ImInputBinder {

public:
    /**
     * A single action representation.
     *
     * At least one of press or release callbacks must be defined.
     * */
    struct action_t {
        /// Action name which serves also as ID. Must be unique and not empty.
        std::string                 name_id;
        /// Key (button) that triggers the action.
        ImGuiKey                    key                 = ImGuiKey_None;
        /// Function to call on press.
        std::function<void(void)>   pressCallback;
        /// Function to call on release.
        std::function<void(void)>   releaseCallback;
        /// Whether to call pressCallback multiple times on key (button) hold.
        bool                        repeatPress         = false;
    };

    ImInputBinder()            = default;
    ~ImInputBinder() noexcept  = default;

    /**
     * Remove all bindings.
     * */
    void reset() noexcept;

    /**
     * Add an action binding.
     *
     * @param action Action to add.
     * @param overwriteDuplicates Whether to overwrite action with same ID.
     *
     * @return True if successful.
     * */
    bool addAction(action_t action, bool overwriteDuplicates = true);

    /**
     * Add multiple actions.
     * @see addAction
     */
    bool addActions(std::initializer_list<action_t> actions, bool overwriteDuplicates = true);

    /**
     * Remove an action binding.
     *
     * @param name_id Name of action to remove.
     * @return True if action was found and removed.
     * */
    bool removeAction(const std::string & name_id);

    /*
     * Call this on every new frame to check for key presses and releases.
     * This function triggers callbacks.
     * */
    void update();

    /**
     * Render editor contents.
     * Use this function if you provide your own window you want to render into.
     * */
    void renderEditor();

    /**
     * Render editor window with contents.
     * Provides its own window. Use renderEditor() to render only contents.
     * */
    void renderEditorWindow();

    /**
     * Load keybindings from file with specified name.
     * */
    bool loadBindings(const std::string & fileName = "ImInputBinderBindings.iib");

    /**
     * Load keybindings from provided stream.
     *
     * @param ifs Stream to load from. Must be opened in a binary mode!
     * */
    bool loadBindings(std::ifstream & ifs);

    /**
     * Save keybindings to file with specified name.
     * */
    bool saveBindings(const std::string & fileName = "ImInputBinderBindings.iib");

    /**
     * Save keybindings to stream provided.
     *
     * @param ifs Stream to save to. Must be opened in a binary mode!
     * */
    bool saveBindings(std::ofstream & ofs);

private:
    std::map<std::string, action_t> m_actions;

    /// Action which has rebinding active. 0 for no active rebinding.
    int m_activeRebindingID = 0;
};

#endif //IMINPUTBINDER_IMINPUTBINDER_H

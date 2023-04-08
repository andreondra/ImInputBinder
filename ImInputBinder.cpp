#include "ImInputBinder.h"

void ImInputBinder::reset() noexcept {
    m_actions.clear();
}

bool ImInputBinder::addAction(action_t action, bool overwriteDuplicates) {

    if(action.name_id.empty())
        throw std::invalid_argument("Action name_id cannot be empty!");
    if(!action.releaseCallback && !action.pressCallback)
        throw std::invalid_argument("Action has no effect: no callback defined!");

    if(!overwriteDuplicates && m_actions.count(action.name_id) != 0)
        return false;

    m_actions[action.name_id] = std::move(action);
    return true;
}

bool ImInputBinder::addActions(std::initializer_list<action_t> actions, bool overwriteDuplicates) {

    for(auto & action : actions) {
        if(!addAction(action, overwriteDuplicates)) return false;
    }

    return true;
}

bool ImInputBinder::removeAction(const std::string & name_id) {

    if(m_actions.count(name_id) > 0) {
        m_actions.erase(name_id);
        return true;
    } else {
        return false;
    }
}

void ImInputBinder::update() {

    for(auto & action : m_actions) {
        if(
            ImGui::IsKeyPressed(action.second.key, action.second.repeatPress) &&
            action.second.pressCallback
        ) {
            action.second.pressCallback();
        }

        if(
            ImGui::IsKeyReleased(action.second.key) &&
            action.second.releaseCallback
        ) {
            action.second.releaseCallback();
        }
    }
}

void ImInputBinder::renderEditor() {

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("split", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Input name");
        ImGui::TableSetupColumn("Assigned button");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        // Start from 1, because 0 is reserved for no rebinding.
        int actionIndex = 1;
        for(auto & action : m_actions) {

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", action.second.name_id.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", ImGui::GetKeyName(action.second.key));
            ImGui::TableNextColumn();

            if(m_activeRebindingID == actionIndex) {

                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 0.6f, 0.6f));
                if (ImGui::Button("Cancel")) {
                    m_activeRebindingID = 0;
                }
                ImGui::PopStyleColor();

                if (ImGui::IsKeyDown(ImGuiKey_MouseLeft) || ImGui::IsKeyDown(ImGuiKey_Escape)) {
                    m_activeRebindingID = 0;
                } else {
                    for (ImGuiKey key = ImGuiKey_KeysData_OFFSET; key < ImGuiKey_COUNT; key = (ImGuiKey) (key + 1)) {
                        if (ImGui::IsKeyDown(key)) {
                            action.second.key = key;
                            m_activeRebindingID = 0;
                        }
                    }
                }
            } else {

                ImGui::PushID(std::string("RE" + std::to_string(actionIndex)).c_str());
                if(ImGui::Button("Rebind...")){
                    m_activeRebindingID = actionIndex;
                }
                ImGui::PopID();
                ImGui::SameLine();

                ImGui::PushID(std::string("DE" + std::to_string(actionIndex)).c_str());
                if(action.second.key != ImGuiKey_None && ImGui::Button("Unassign")){
                    action.second.key = ImGuiKey_None;
                }
                ImGui::PopID();
            }

            actionIndex++;
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}

void ImInputBinder::renderEditorWindow() {

    ImGui::Begin("Input bindings");
    renderEditor();
    ImGui::End();
}

bool ImInputBinder::saveBindings(const std::string & fileName) {

    std::ofstream file(fileName, std::ios::out | std::ios::binary);
    if(!file)
        return false;

    return saveBindings(file);
}

bool ImInputBinder::saveBindings(std::ofstream & ofs) {

    const char *header = "IIB";
    char version = 1;

    ofs.write(header, strlen(header));
    ofs.write(&version, 1);
    if(!ofs) return false;

    std::string stringID;
    for(auto & action : m_actions) {

        ofs.write(action.second.name_id.data(), action.second.name_id.size());
        ofs.put('\0');

        try {
            stringID = std::to_string(action.second.key);
        } catch (...) {
            return false;
        }

        ofs.write(stringID.data(), stringID.size());
        ofs.put(';');

        if(!ofs) return false;
    }

    return true;
}

bool ImInputBinder::loadBindings(const std::string & fileName) {

    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    if(!file)
        return false;

    return loadBindings(file);
}

bool ImInputBinder::loadBindings(std::ifstream &ifs) {

    char header[3];
    char version;

    ifs.read(header, 3);
    if(!ifs || strncmp(header, "IIB", 3) != 0) {
        return false;
    }

    ifs.read(&version, 1);
    if(!ifs || version != 1) {
        return false;
    }

    std::string id, key;
    ImGuiKey parsedKey;

    while(ifs) {

        std::getline(ifs, id, '\0');
        std::getline(ifs, key, ';');

        if(!ifs) break;

        try {
            parsedKey = static_cast<ImGuiKey>(std::stoi(key));
        } catch (...) {
            return false;
        }

        if(m_actions.count(id) > 0) {
            m_actions[id].key = parsedKey;
        }
    }

    if(ifs.eof())
        return true;
    else
        return false;
}

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include "../Debugging/Logging.h"

void ImGuiHandler::DrawLogsWindow() {

    //static ExampleAppLog log;
    bool open = true;
    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Log", &open);
    std::vector<std::string> log_Entries = Logging::Logger::GetInstance().getLogList();
    for (size_t i = 0; i < log_Entries.size(); ++i) {
        ImGui::Text(log_Entries[i].c_str());
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    //log.Draw("Example: Log", &open);
}
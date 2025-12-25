#include "AiPanel.h"
#include "Imgui/imgui.h"
#include <string>
#include <vector>
#include <mutex>

AiPanel::AiPanel()
{

}

AiPanel::~AiPanel()
{

}

struct ChatMessage {
    const char* role;
    std::string content;
    ImVec4 color;
};

std::vector<ChatMessage> g_chatMessages;
char g_inputBuffer[2048] = "";
bool g_isAIThinking = false;
std::mutex g_msgMutex;

void SimulateAIReply(const std::string& userInput) {
    g_isAIThinking = true;

    _sleep(2000);
    std::string aiReply = userInput;
    {
        std::lock_guard<std::mutex> lock(g_msgMutex);
        g_chatMessages.push_back({
            "assistant",
            aiReply,
            ImVec4(0.2f, 0.6f, 1.0f, 1.0f)
            });
    }

    g_isAIThinking = false;
}

void AiPanel::update()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("AiPanel");

    ImVec2 chatRegionSize = ImVec2(0.0f, -ImGui::GetFrameHeightWithSpacing() * 3);

    ImGui::BeginChild("ChatRegion", chatRegionSize, true);

    {
        std::lock_guard<std::mutex> lock(g_msgMutex);
        for (const auto& msg : g_chatMessages) {

            ImGui::TextColored(msg.color, "%s:", msg.role == std::string("user") ? "you" : "AI");

            ImGui::SameLine();

            ImGui::TextWrapped("%s", msg.content.c_str());

            ImGui::Spacing();
        }
    }


    if (g_isAIThinking) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "AI is thinking");
    }

    if (g_chatMessages.size() > 0 || g_isAIThinking) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::InputText(
        "##Input",
        g_inputBuffer,
        IM_ARRAYSIZE(g_inputBuffer),
        ImGuiInputTextFlags_EnterReturnsTrue
    );

    ImGui::SameLine();
    bool sendClicked = ImGui::Button("send", ImVec2(100, 0.0f));

    bool inputValid = (g_inputBuffer[0] != '\0') && !g_isAIThinking;
    if ((sendClicked || ImGui::IsItemActivated()) && inputValid) {

        std::string userInput = g_inputBuffer;

        {
            std::lock_guard<std::mutex> lock(g_msgMutex);
            g_chatMessages.push_back({
                "user",
                userInput,
                ImVec4(0.2f, 0.8f, 0.2f, 1.0f)
                });
        }


        memset(g_inputBuffer, 0, sizeof(g_inputBuffer));

        SimulateAIReply(userInput);
    }

    ImGui::End();

    ImGui::PopStyleVar();
}

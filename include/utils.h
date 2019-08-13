#ifndef __UTILS_H__
#define __UTILS_H__

#include <windows.h>
#include <shlobj.h>
#include <string>

#include "imgui.h"
#include "imgui-SFML.h"
#include "nfd.h" 

namespace glspv {

class Utils {
public:
  enum TextInputType {
    kTextInput_Text = 0,
    kTextInput_OpenFile,
    kTextInput_PickFolder
  };
  static bool TextInput(std::string& path, const char* title, const char* hint, TextInputType type, const char* defaultPath, const char* filterList, bool is_big = true) {
    bool return_value = false;
    if (is_big) {
      ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
      ImGui::Text(title);
      ImGui::PopFont();
    }
    if (type != kTextInput_Text) {
      std::string button_id = "...##";
      button_id.append(title);
      if (ImGui::Button(button_id.c_str(), ImVec2(30.0f, 30.0f))) {
        nfdchar_t* outPath = NULL;
        nfdresult_t result;
        switch (type) {
        case kTextInput_OpenFile:
          result = NFD_OpenDialog(filterList, defaultPath, &outPath);
          break;
        case kTextInput_PickFolder:
          result = NFD_PickFolder(defaultPath, &outPath);
          break;
        }

        if (result == NFD_OKAY) {
          puts("Success!");
          puts(outPath);
          path = outPath;
          free(outPath);
          return_value = true;
        }
        else if (result == NFD_CANCEL) {
          puts("User pressed cancel.");
        }
        else {
          printf("Error: %s\n", NFD_GetError());
        }
      }
      ImGui::SameLine();
      ImGui::PushItemWidth(-1);
    }
    else {
      ImGui::PushItemWidth(400);
    }
    std::string input_id = "##";
    input_id.append(title);

    ImGui::InputTextWithHint(input_id.c_str(), hint, (char*)path.data(), 255);
    return return_value;
  }

  static void SetGreenButtonColor() {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(20, 102, 91));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(34, 137, 110));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(66, 188, 127));;
  }

  static void SetRedButtonColor() {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(135, 42, 56));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(216, 56, 67));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(255, 104, 102));;
  }

  static void ResetButtonColor() {
    ImGui::PopStyleColor(3);
  }

  static void SetStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.FramePadding = ImVec2(5, 5);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;

    style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
  }

  static void AddFonts() {
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)
    // IO.Fonts->AddFontDefault(); // this will load default font as well
    io.Fonts->AddFontFromFileTTF("../../resources/fonts/NotoSansKR-Regular.otf", 18.f);
    io.Fonts->AddFontFromFileTTF("../../resources/fonts/NotoSansKR-Bold.otf", 28.f);

    ImGui::SFML::UpdateFontTexture(); // important call: updates font texture
  }

  static std::string GetMyDocumentsFolder() {
    static CHAR my_documents[MAX_PATH];
    static HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

    return std::string(my_documents);
  }

};
}

#endif //!__UTILS_H__

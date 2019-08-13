#include "app.h"

#include <cstdio>
#include <string>
#include <memory>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>

#include "nfd.h" 
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include "imgui.h"
#include "imgui-SFML.h"


namespace {
struct {
  std::unique_ptr<sf::RenderWindow> renderWindow;
  sf::Clock deltaClock;
  std::string vulkanSdk;
  std::string defaultVulkanSdk = "C:\\VulkanSDK";
  std::string inputFile;
  std::string myDocuments;
  std::string outputPath;
  std::string defaultPath;
  std::string outputName;
  HWND handle;
  LONG_PTR originalsfmlcallback = 0x0;
} AppData;
};
enum TextInputType {
  kTextInput_Text = 0,
  kTextInput_OpenFile,
  kTextInput_PickFolder
};

#pragma region StaticFuncs

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
  io.Fonts->AddFontFromFileTTF("../../resources/fonts/NotoSansKR-Regular.otf", 16.f);
  io.Fonts->AddFontFromFileTTF("../../resources/fonts/NotoSansKR-Bold.otf", 28.f);

  ImGui::SFML::UpdateFontTexture(); // important call: updates font texture
}

static bool TextInput(std::string& path, const char* title, const char* hint, TextInputType type, const char* defaultPath, const char* filterList) {
  bool return_value = false;
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
  ImGui::Text(title);
  ImGui::PopFont();
  std::string button_id = "...##";
  button_id.append(title);
  if (type != kTextInput_Text) {
    if (ImGui::Button(button_id.c_str(), ImVec2(30.0f, 26.0f))) {
      nfdchar_t* outPath = NULL;
      nfdresult_t result;
      switch (type)
      {
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
  }
  ImGui::PushItemWidth(-1);
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

static void CompileScript(const std::string& vulkanSdk, const std::string& input, const std::string& ouput) {
  // additional information
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  std::string command = vulkanSdk + " " + input + " -o " + ouput;

  // start the program up
  CreateProcess(NULL,   // the path
    const_cast<LPSTR>(command.c_str()),        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
  );
  // Close process and thread handles. 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

static void SetRedButtonColor() {
  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(135, 42, 56));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(216, 56, 67));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(255, 104, 102));;
}

static void ResetButtonColor() {
  ImGui::PopStyleColor(3);
}

static std::string GetMyDocumentsFolder() {
  CHAR my_documents[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

  return std::string(my_documents);
}

static LRESULT CALLBACK mycallback(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_DROPFILES)
  {
    HDROP hdrop = reinterpret_cast<HDROP>(wParam);
    POINT p;
    p.x = 0;
    p.y = 0;
    if (DragQueryPoint(hdrop, &p))
      std::printf("Point is %d, %d\n", p.x, p.y);
    else
      puts("Failed to get point");

    const UINT filescount = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < filescount; ++i)
    {
      const UINT bufsize = DragQueryFile(hdrop, i, NULL, 0);
      std::string str;
      str.resize(bufsize + 1);
      if (DragQueryFile(hdrop, i, const_cast<LPSTR>(str.c_str()), bufsize + 1))
      {
        std::string stdstr;
        sf::Utf8::fromWide(str.begin(), str.end(), std::back_inserter(stdstr));
        puts(stdstr.c_str());
        //std::cout << stdstr << std::endl;
      }
    }
    DragFinish(hdrop);
    puts("-------------");
    //std::cout << "-------------" << std::endl;
  }//if WM_DROPFILES
  return CallWindowProcW(reinterpret_cast<WNDPROC>(AppData.originalsfmlcallback), handle, message, wParam, lParam);
}

#pragma endregion

void glspv::App::init(const glspv::App::WindowProperties& properties) {
  AppData.renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(480, 640), "ImGui + SFML = <3", sf::Style::Close);
  AppData.renderWindow->setFramerateLimit(60);
  ImGui::SFML::Init(*AppData.renderWindow);

  SetStyle();
  AddFonts();

  AppData.defaultPath = AppData.myDocuments = GetMyDocumentsFolder();
  AppData.handle = AppData.renderWindow->getSystemHandle();

  DragAcceptFiles(AppData.handle, TRUE);
  AppData.originalsfmlcallback = SetWindowLongPtrW(AppData.handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(mycallback));
}

void glspv::App::run() {
  while (AppData.renderWindow->isOpen()) {
    sf::Event event;
    while (AppData.renderWindow->pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        AppData.renderWindow->close();
      }
    }
    ImGui::SFML::Update(*AppData.renderWindow, AppData.deltaClock.restart());

    int32_t window_flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
    ImGui::SetNextWindowSize(ImVec2(480.f, 640.f));
    ImGui::Begin("Main window", 0, window_flags);
    if (TextInput(AppData.vulkanSdk, "Vulkan SDK Path", "Path to Vulkan SDK", kTextInput_PickFolder, AppData.defaultVulkanSdk.c_str(), NULL)) {
      AppData.defaultVulkanSdk = AppData.vulkanSdk;
    }
    if (TextInput(AppData.inputFile, "Input", "Path to shader", kTextInput_OpenFile, AppData.defaultPath.c_str(), NULL)) {
      std::filesystem::path path(AppData.inputFile);
      AppData.outputPath = AppData.defaultPath = path.parent_path().string();
      AppData.outputName = path.stem().string() + "-" + path.extension().string().erase(0, 1) + ".spv";
      printf("Parent path: %s\n", path.parent_path().string().c_str());
      printf("File name: %s\n", path.stem().string().c_str());
      printf("File extension: %s\n", path.extension().string().c_str());
    }

    TextInput(AppData.outputPath, "Output Path", "", kTextInput_PickFolder, AppData.defaultPath.c_str(), NULL);
    TextInput(AppData.outputName, "Output Name", "", kTextInput_Text, NULL, NULL);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    SetGreenButtonColor();
    float width = (ImGui::GetContentRegionAvailWidth() * 0.5f) - 5.0f;
    if (ImGui::Button("Compile", ImVec2(width, 0))) {
      CompileScript(AppData.vulkanSdk + "\\Bin32\\glslc.exe", AppData.inputFile, AppData.outputPath + "\\" + AppData.outputName);
    }
    ResetButtonColor();
    SetRedButtonColor();
    ImGui::SameLine(0.0f, 10.0f);
    if (ImGui::Button("Clear", ImVec2(width, 0))) {
      // TODO
    }
    ResetButtonColor();
    ImGui::PopFont();
    ImGui::End();

    //ImGui::ShowTestWindow();


    AppData.renderWindow->clear();
    ImGui::SFML::Render(*AppData.renderWindow);
    AppData.renderWindow->display();
  }
}

void glspv::App::shutdown() {
  ImGui::SFML::Shutdown();
}

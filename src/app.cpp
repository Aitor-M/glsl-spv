#include "app.h"

#include <cstdio>
#include <string>
#include <memory>
#include <windows.h>
#include <shellapi.h>
#include <fstream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "INIReader.h"

#include "shader_list.h"
#include "utils.h"

namespace {

struct {
  std::unique_ptr<sf::RenderWindow> renderWindow;
  sf::Clock deltaClock;
  std::string vulkanSdk;
  std::string defaultVulkanSdk = "C:\\VulkanSDK";
  glspv::ShaderList datalist;
  ImVec2 windowSize;
  std::string title;
  bool fullscreen;

  HWND handle;
  LONG_PTR originalsfmlcallback = 0x0;
} AppData;
};

#pragma region StaticFuncs

static LRESULT CALLBACK mycallback(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
  if (message == WM_DROPFILES) {
    HDROP hdrop = reinterpret_cast<HDROP>(wParam);
    POINT p;
    p.x = 0;
    p.y = 0;
#ifdef  _DEBUG
    if (DragQueryPoint(hdrop, &p))
      std::printf("Point is %d, %d\n", p.x, p.y);
    else
      puts("Failed to get point");
#endif //  _DEBUG


    const UINT filescount = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < filescount; ++i) {
      const UINT bufsize = DragQueryFile(hdrop, i, NULL, 0);
      std::string str;
      str.resize(bufsize + 1);
      if (DragQueryFile(hdrop, i, const_cast<LPSTR>(str.c_str()), bufsize + 1)) {
        std::string stdstr;
        // end - 1 because we don't want to copy the \0, right?
        sf::Utf8::fromWide(str.begin(), str.end() - 1, std::back_inserter(stdstr));
#ifdef  _DEBUG
        puts(stdstr.c_str());
#endif
        glspv::ShaderData new_data(glspv::Utils::GetMyDocumentsFolder());
        new_data.format(stdstr);
        AppData.datalist.add(new_data);
      }
    }
    DragFinish(hdrop);
#ifdef  _DEBUG
    puts("-------------");
#endif
    //std::cout << "-------------" << std::endl;
  }//if WM_DROPFILES
  return CallWindowProcW(reinterpret_cast<WNDPROC>(AppData.originalsfmlcallback), handle, message, wParam, lParam);
}

static void ClearConsole() {
  COORD topLeft = { 0, 0 };
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;

  GetConsoleScreenBufferInfo(console, &screen);
  FillConsoleOutputCharacterA(
    console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
  );
  FillConsoleOutputAttribute(
    console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
    screen.dwSize.X * screen.dwSize.Y, topLeft, &written
  );
  SetConsoleCursorPosition(console, topLeft);
}

static void SaveIni() {
  std::string default_ini =
    "[Window]\n"\
    "Width=" + std::to_string((int)AppData.windowSize.x) + "\n"\
    "Height=" + std::to_string((int)AppData.windowSize.y) + "\n"\
    "[Vulkan]\n"\
    "Path=" + AppData.vulkanSdk + "\n";
  std::ofstream out("settings.ini");
  out << default_ini;
  out.close();
}

#pragma endregion

void glspv::App::init() {
  AppData.windowSize = ImVec2(560, 640);
  AppData.title = "GLSL-SPV";
  INIReader ini("settings.ini");
  if (!ini.ParseError()) {
    AppData.windowSize.x = ini.GetReal("Window", "Width", AppData.windowSize.x);
    AppData.windowSize.y = ini.GetReal("Window", "Height", AppData.windowSize.y);
    AppData.vulkanSdk = ini.Get("Vulkan", "Path", "");
  } else {
    SaveIni();
  }

  AppData.renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode((unsigned int)AppData.windowSize.x, (unsigned int)AppData.windowSize.y), AppData.title, sf::Style::Close);
  AppData.renderWindow->setFramerateLimit(60);
  ImGui::SFML::Init(*AppData.renderWindow);

  Utils::SetStyle();
  Utils::AddFonts();

  AppData.handle = AppData.renderWindow->getSystemHandle();

  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = NULL;

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
    ImGui::SetNextWindowSize(AppData.renderWindow->getSize());
    ImGui::Begin("Main window", 0, window_flags);


    Utils::TextInput(AppData.vulkanSdk, "Vulkan SDK Path", "C:\\VulkanSDK\\x.x.xxx.x", Utils::kTextInput_PickFolder, AppData.defaultVulkanSdk.c_str(), NULL);
    Utils::SetGreenButtonColor();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    if (ImGui::Button("Add Shader", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
      ShaderData new_data(Utils::GetMyDocumentsFolder());
      AppData.datalist.add(new_data);
    }
    ImGui::PopFont();
    Utils::ResetButtonColor();

    ImVec2 availableRegion = ImGui::GetContentRegionAvail();
    ImVec2 child_size = ImVec2(availableRegion.x, availableRegion.y - 40.0f);

    ImGui::BeginChild("Child1", child_size, false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);
    AppData.datalist.draw();
    ImGui::EndChild();

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    Utils::SetGreenButtonColor();
    float width = (ImGui::GetContentRegionAvailWidth() * 0.5f) - 5.0f;
    if (ImGui::Button("Compile", ImVec2(width, 0))) {
      ClearConsole();
      AppData.datalist.compile(AppData.vulkanSdk + "\\Bin32\\glslc.exe");
    }
    Utils::ResetButtonColor();
    Utils::SetRedButtonColor();
    ImGui::SameLine(0.0f, 10.0f);
    if (ImGui::Button("Clear", ImVec2(width, 0))) {
      AppData.datalist.clear();
    }
    Utils::ResetButtonColor();
    ImGui::PopFont();
    ImGui::End();

    AppData.renderWindow->clear();
    ImGui::SFML::Render(*AppData.renderWindow);
    AppData.renderWindow->display();
  }
}

void glspv::App::shutdown() {
  SaveIni();
  ImGui::SFML::Shutdown();
  AppData.renderWindow.reset();
}

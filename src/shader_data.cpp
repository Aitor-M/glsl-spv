#include "shader_data.h"

#include <filesystem>

#include "imgui.h"
#include "utils.h"

glspv::ShaderData::ShaderData(const std::string& defaultPaths) {
  inputFileDefaultPath_ = outputDefaultPath_ = defaultPaths;
}

void glspv::ShaderData::format(const std::string& inputFilePath) {
  std::filesystem::path path(inputFilePath);
#if _DEBUG
  printf("Parent path: %s\n", path.parent_path().string().c_str());
  printf("File name: %s\n", path.stem().string().c_str());
  printf("File extension: %s\n", path.extension().string().c_str());
#endif
  originalName_ = path.stem().string() + path.extension().string();
  inputFilePath_ = inputFilePath;
  outputPath_ = path.parent_path().string();
  outputName_ = path.stem().string() + "-" + path.extension().string().erase(0, 1) + ".spv";
}

void glspv::ShaderData::draw(uint32_t index) {
  std::string title = std::to_string(index) + " - " + originalName_;
  if (!ImGui::CollapsingHeader(title.c_str()))
    return;
  if (Utils::TextInput(inputFilePath_, std::string(title + "Input").c_str(), "Path to shader", Utils::kTextInput_OpenFile, inputFileDefaultPath_.c_str(), NULL, false)) {
    format(inputFilePath_);
  }

  Utils::TextInput(outputPath_, std::string(title + "OutputPath").c_str(), "Output path", Utils::kTextInput_PickFolder, outputDefaultPath_.c_str(), NULL, false);
  Utils::TextInput(outputName_, std::string(title + "OutputName").c_str(), "Output name", Utils::kTextInput_Text, NULL, NULL, false);
  ImGui::SameLine();
  Utils::SetRedButtonColor();
  if (ImGui::Button(std::string("Remove##" + title).c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
    shouldBeRemoved_ = true;
  }
  Utils::ResetButtonColor();
}

bool glspv::ShaderData::shouldBeRemoved() const {
  return shouldBeRemoved_;
}

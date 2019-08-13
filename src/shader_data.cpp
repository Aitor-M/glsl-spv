#include "shader_data.h"

#include <filesystem>

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
  inputFilePath_ = inputFilePath;
  outputPath_ = path.parent_path().string();
  outputName_ = path.stem().string() + "-" + path.extension().string().erase(0, 1) + ".spv";
}
#ifndef __SHADER_DATA_H__
#define __SHADER_DATA_H__

#include <string>

namespace glspv {
class ShaderData {
public:
  std::string inputFilePath_;
  std::string outputPath_;
  std::string outputName_;

  ShaderData(const std::string& defaultPaths);
  ~ShaderData() = default;
  ShaderData(const ShaderData&) = default;
  ShaderData& operator=(const ShaderData&) = default;

  // Sets inputFilePath with the provided string
  // outputPath will equal the same folder as inputFilePath's parent directory
  // outputName will be inputFilePath's fileName + "-" + inputFilePath's extension + ".spv"
  void format(const std::string& inputFilePath);
  void draw(uint32_t index);
  bool shouldBeRemoved() const;

private:
  std::string originalName_;
  std::string inputFileDefaultPath_;
  std::string outputDefaultPath_;
  bool shouldBeRemoved_ = false;
};
}

#endif //!__SHADER_DATA_H__

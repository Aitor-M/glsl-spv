#ifndef __SHADER_LIST_H__
#define __SHADER_LIST_H__

#include <list>

#include "shader_data.h"

namespace glspv {
class ShaderList {
public:
  ShaderList() = default;
  ~ShaderList() = default;
  void add(const ShaderData& data);
  void removeAt(size_t index);
  void draw();
  void clear();
  void compile(const std::string& vulkanSdk);

private:
  std::list<ShaderData> list_;

  ShaderList(const ShaderList&) = default;
  ShaderList& operator=(const ShaderList&) = default;
};
}


#endif //!__SHADER_LIST_H__
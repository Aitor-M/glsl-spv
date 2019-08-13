#include "shader_list.h"

void glspv::ShaderList::add(const ShaderData& data) {
  list_.push_back(data);
}

void glspv::ShaderList::removeAt(size_t index) {
  auto listi = list_.begin();
  std::advance(listi, index);
  list_.erase(listi);
}

std::list<glspv::ShaderData>& glspv::ShaderList::get(size_t index) {
  return list_;
}

const std::list<glspv::ShaderData>& glspv::ShaderList::get(size_t index) const {
  return list_;
}
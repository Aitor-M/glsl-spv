#include "app.h"


int main() {
  //glspv::App::init({ 480, 640, "GLSL-SPV" });
  glspv::App::init({ 1280, 720, "GLSL-SPV" });
  glspv::App::run();
  glspv::App::shutdown();
  return 0;
}

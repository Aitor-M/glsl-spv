#ifndef __APP_H__
#define __APP_H__ 1

#include <string>

namespace glspv {
class App {
public:
  struct WindowProperties {
    uint32_t width;
    uint32_t height;
    std::string title;
  };
  static void init(const WindowProperties& properties);
  static void run();
  static void shutdown();
};
}
#endif //!__APP_H__
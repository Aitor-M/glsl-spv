#ifndef __APP_H__
#define __APP_H__ 1

#include <string>

namespace glspv {
class App {
public:
  static void init();
  static void run();
  static void shutdown();
};
}
#endif //!__APP_H__
#include "utils.hpp"

#if defined(_WIN32)
  #include <windows.h>
#elif defined(__APPLE__)
  #include <cstdlib>
  #include <TargetConditionals.h>
#elif defined(__linux__)
  #include <cstdlib>
#else
  #error "Unsupported platform"
#endif

#include <string>

namespace utils {

void open_url(const std::string& url) {
#if defined(_WIN32)
  ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__)
  std::string cmd = "open " + url;
  std::system(cmd.c_str());
#elif defined(__linux__)
  std::string cmd = "xdg-open " + url;
  std::system(cmd.c_str());
#endif
}

} // namespace utils.
// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file app.hpp
/// @brief Class that handles the application.

# pragma once

#include <string>
#include <vector>
#include <chrono>

#include "app_window.h"
#include "comms/serial_manager.hpp"

namespace slint_gui_app::application {

class App {
 public:
  App();

  ~App();

  int Run();

 private:
  slint::ComponentHandle<AppWindow> ui_ = AppWindow::create();
  comms::SerialManager serial_manager_;
  slint::Timer serial_read_timer_{};

  void Setup();
};

} // namespace slint_gui_app::application
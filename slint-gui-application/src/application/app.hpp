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

/// @brief The Application class.
class App {
 public:

  /// @brief Construct an App object.
  App();

  /// @brief Destroy the App object.
  ~App();

  /// @brief Run the application.
  /// @return The application exit code.
  int Run();

 private:

  slint::ComponentHandle<AppWindow> ui_ = AppWindow::create(); ///< The application UI component.
  comms::SerialManager serial_manager_; ///< The serial manager for handling serial communication.
  slint::Timer serial_read_timer_{}; ///< Timer for periodically reading from the serial port.

  /// @brief Set up the application (UI, serial communication, etc.).
  void Setup();
};

} // namespace slint_gui_app::application
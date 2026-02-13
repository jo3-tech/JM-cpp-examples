// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file main.cpp
/// @brief Main file containing the main function (application entry point).

#include "app.hpp"

slint_gui_app::application::App app{}; ///< The application instance.

/// @brief The main application entry point.
int main(int argc, char **argv) {
  return app.Run();
}

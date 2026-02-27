// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file main.cpp
/// @brief Main application entry point that initialises the program and starts the top‑level application control flow.

#include "app.hpp"

slint_gui_app::application::App app{}; ///< The application instance.

/// @brief The main application entry point that starts the application.
int main(int argc, char **argv) {
  return app.Run();
}

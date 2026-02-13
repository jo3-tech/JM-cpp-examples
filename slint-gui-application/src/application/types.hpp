// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file types.hpp
/// @brief Types for the application.

#pragma once

#include <cstdint>

namespace slint_gui_app::application {

/// @brief Enum of D-Pad messages to write to the serial port.
enum class DPadMessage : uint8_t {
  kUp = 'u', // 117
  kDown = 'd', // 100
  Left = 'l', // 108
  Right = 'r' // 114
};

} // namespace slint_gui_app::application
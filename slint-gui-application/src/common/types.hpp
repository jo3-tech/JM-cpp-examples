// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file types.hpp
/// @brief Common types.

# pragma once

#include <cstdint>

namespace slint_gui_app::common {

  /// @brief Enum of status.
  enum class Status : int8_t {
    kError = -1,
    kFailure,
    kSuccess,
    kTimeout 
  };

} // namespace slint_gui_app::common
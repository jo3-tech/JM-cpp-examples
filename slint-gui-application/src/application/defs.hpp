// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file defs.hpp
/// @brief Definitions for the application.

# pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>

namespace slint_gui_app::application {

inline const std::vector<std::string> kBaudRates = { "115200", "57600", "38400", "19200", "9600" }; ///< Baud rates for serial communication.

inline const std::string kSerialPortsEmptyInfo = "None found."; ///< Message to display in the UI when no serial ports are found.
inline const std::string KSerialConnectInfo = "Connect"; ///< Message to display in the UI when the serial port is disconnected.
inline const std::string KSerialDisconnectInfo = "Disconnect"; ///< Message to display in the UI when the serial port is connected.

inline constexpr uint32_t kSerialReadTimeout_ms = 1000; ///< Timeout (ms) for reading from the serial port.
inline constexpr auto kSerialReadTimerInterval_ms = std::chrono::milliseconds(100); ///< Interval for the timer that periodically reads from the serial port.

} // namespace slint_gui_app::application
// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file serial_manager.hpp
/// @brief Class that handles serial communication.

# pragma once

#include <cstdint>
#include <cstddef>

#include "serial/serial.h"
#include "asio.hpp"

#include "common/types.hpp"

namespace slint_gui_app::comms {

/// @brief The Serial Manager class.  
class SerialManager {
 public:
  
  /// @brief Construct a Serial Manager object.
  SerialManager();

  /// @brief Destroy the Serial Manager object.
  ~SerialManager() = default;

  /// @brief Discover available serial ports.
  /// @return Vector of port descriptions from serial::PortInfo. 
  std::vector<std::string> EnumerateSerialPorts();

  /// @brief Connect to a serial port.
  /// @param port_index The index of the port from enumerated serial ports.
  /// @param baud_rate The baud rate.
  /// @param read_timeout_ms Timeout for serial read operations.
  /// @return The status of the connection attempt.
  common::Status ConnectSerial(int port_index, uint32_t baud_rate, uint32_t read_timeout_ms);

  /// @brief Disconnect to a serial port.
  /// @return The status of the disconnection attempt.
  common::Status DisconnectSerial();

  /// @brief Write bytes to the connected serial port.
  /// @param payload Pointer to the payload bytes.
  /// @param payload_size_bytes Size of the payload in bytes.
  /// @return Size of the bytes written.
  int64_t WriteBytesToSerial(const uint8_t* payload, size_t payload_size_bytes);

  /// @brief Read bytes from the connected serial port.
  /// @param buffer Pointer to the buffer to store the bytes.
  /// @param size_bytes Size of the bytes to read.
  /// @return Size of the bytes read.
  int64_t ReadBytesFromSerial(uint8_t* buffer, size_t size_bytes);

  /// @brief Get the flag that indicates whether a connection to the serial port is established.
  /// @return The flag that indicates if the serial port is open (True = open, false = closed).
  bool serial_port_open() const;

 private:

  /// @brief The serial object.
  serial::Serial serial_port_{};

  /// @brief Vector of enumerated serial ports.
  std::vector<serial::PortInfo> serial_ports_info_{};

  /// @brief Flag that indicates whether a serial connection is established (i.e., if the serial port is open).
  bool serial_port_open_{false};
};

} // namespace slint_gui_app::comms
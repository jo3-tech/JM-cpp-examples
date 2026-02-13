// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file serial_manager.cpp
/// @brief Class that handles serial communication.

#include "serial_manager.hpp"

#include <cstdint>
#include <cstddef>
#include <iostream>

#include "serial/serial.h"

#include "common/types.hpp"

namespace slint_gui_app::comms {

SerialManager::SerialManager() {}

std::vector<std::string> SerialManager::EnumerateSerialPorts() {
  // Discover available serial ports.
  serial_ports_info_ = serial::list_ports();

  // Get the friendly names into a string.
  std::vector<std::string> serial_ports_names{};
  serial_ports_names.reserve(serial_ports_info_.size());

  if (!serial_ports_info_.empty()) {
    std::cout << "Found " << serial_ports_info_.size() << " serial ports: " << std::endl;
    for (int i = 0; const auto& port_info : serial_ports_info_) {
      serial_ports_names.push_back(port_info.description);
      std::cout << "Port " << i << ": " << port_info.description << std::endl;
      i++;
    }
  }
  else {
    std::cout << "No serial ports found." << std::endl;
  }

  return serial_ports_names;
}

common::Status SerialManager::ConnectSerial(int port_index, uint32_t baud_rate, uint32_t read_timeout_ms) {
  common::Status status = common::Status::kFailure;
  if (!serial_port_open_) {
    std::cout << "Connecting to serial port: " << serial_ports_info_[port_index].port << " at baud rate: " << baud_rate << std::endl;
    serial_port_.setPort(serial_ports_info_[port_index].port);
    serial_port_.setBaudrate(baud_rate);
    auto timeout = serial::Timeout::simpleTimeout(read_timeout_ms);
    serial_port_.setTimeout(timeout);
    serial_port_.open();

    if(serial_port_.isOpen()) {
      serial_port_open_ = true;
      serial_port_.flush();
      status = common::Status::kSuccess;
    }
    else {
      std::cerr << "Failed to open port." << std::endl;
      status = common::Status::kFailure;      
    }
  }
  else {
    std::cout << "Serial port already open." << std::endl;
  }

  return status;
}

common::Status SerialManager::DisconnectSerial() {
  auto status = common::Status::kFailure;
  if (serial_port_open_) {
    std::cout << "Disconnecting serial." << std::endl;
    serial_port_.close();

    if(serial_port_.isOpen()) {
      std::cerr << "Failed to close port." << std::endl;
      status = common::Status::kFailure;
    }
    else {
      serial_port_open_ = false;
      status = common::Status::kSuccess;      
    }
  }

  return status;
}

int64_t SerialManager::WriteBytesToSerial(const uint8_t* payload, size_t payload_size_bytes) {
  if (serial_port_open_) {
    try {
      return serial_port_.write(payload, payload_size_bytes);
    }
    catch (const serial::IOException& e) {
    //catch (const std::exception& e) {  
      serial_port_open_ = false;
      serial_port_.close();
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
    }
    catch (...) {
      std::cerr << "Unknown exception" << std::endl;
      return -1;
    }
  }
  else {
    std::cout << "Serial port closed, cannot send data." << std::endl;
    return 0;
  }
}

int64_t SerialManager::ReadBytesFromSerial(uint8_t* buffer, size_t size_bytes) {
  if (serial_port_open_) {
    try {
      size_t bytes_available = serial_port_.available();
      if (bytes_available >= size_bytes) {
          return serial_port_.read(buffer, size_bytes);
      }
      else {
        //std::cerr << "No data or not enough data available to read requested length." << std::endl;
        return 0;
      }
    }
    catch (const serial::IOException& e) {
    //catch (const std::exception& e) {
      serial_port_open_ = false;
      serial_port_.close();
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
    }
    catch (...) {
      std::cerr << "Unknown exception" << std::endl;
      return -1;
    }
  }
  else {
    //std::cerr << "Serial port closed, cannot read data." << std::endl;
    return 0;
  }  
}

bool SerialManager::serial_port_open() const {
  return serial_port_open_;
}

} // namespace slint_gui_app::comms
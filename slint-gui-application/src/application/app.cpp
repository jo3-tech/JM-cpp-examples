// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file app.cpp
/// @brief Class that handles the application.

#include "app.hpp"

#include <cstdint>
#include <iostream>

#include "app_window.h"
#include "defs.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "common/types.hpp"
#include "comms/serial_manager.hpp"

namespace slint_gui_app::application {

App::App() {}

App::~App() {}

int App::Run() {
  Setup();
  ui_->run();
  return 0;
}

void App::Setup() {
  // Register callback functions.

  ui_->global<ModelState>().on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    application::open_url(std::string(url_output));
  });

  ui_->global<ModelState>().on_refresh_serial_ports([this]() {
    std::cout << "Refresh serial ports requested from UI." << std::endl;
    std::vector<std::string> serial_ports_names = serial_manager_.EnumerateSerialPorts();
    std::vector<slint::SharedString> ui_serial_ports_names{};
    ui_serial_ports_names.reserve(serial_ports_names.size());

    if (serial_ports_names.empty()) {
      std::cout << kSerialPortsEmptyInfo << std::endl;
      ui_serial_ports_names.push_back(slint::SharedString(kSerialPortsEmptyInfo));
    }
    else {
      std::cout << "Found " << serial_ports_names.size() << " serial ports: " << std::endl;
      for (int i = 0; const auto& name : serial_ports_names) {
        std::cout << "Port " << i << ": " << name << std::endl;
        //ui_serial_ports_names.push_back(slint::SharedString(name));
        i++;
      }

      ui_serial_ports_names.assign(serial_ports_names.begin(), serial_ports_names.end());
    }
    
    // Convert to UI model (slint::VectorModel<slint::SharedString>).
    auto ui_serial_ports_names_model = std::make_shared<slint::VectorModel<slint::SharedString>>(ui_serial_ports_names);

    // Set the model in the UI.
    //ui->set_serial_ports(ui_serial_port_ids_model);
    ui_->global<ModelState>().set_serial_ports(ui_serial_ports_names_model);    
  });

  ui_->global<ModelState>().on_connect_or_disconnect_serial([this](int ports_index, int bauds_index) {
    auto connection_info = slint::SharedString(KSerialConnectInfo);
    if (serial_manager_.serial_port_open()) {
      // Disconnect.
      serial_manager_.DisconnectSerial();
    }
    else {
      // Connect.
      uint32_t baud_rate = std::stoi(std::string(kBaudRates[bauds_index]));
      common::Status status = serial_manager_.ConnectSerial(ports_index, baud_rate, kSerialReadTimeout_ms);
      if (status == common::Status::kSuccess) connection_info = slint::SharedString(KSerialDisconnectInfo);
    }

    return connection_info;
  });

  ui_->global<ModelState>().on_send_byte_over_serial([this](int value_output) {
    uint8_t value = value_output;
    std::cout << "Send byte over serial: " << static_cast<int>(value) << std::endl;
    serial_manager_.WriteBytesToSerial(&value, 1);
  });

  ui_->global<ModelState>().on_up_clicked([this]() {
    uint8_t up_command = static_cast<uint8_t>(DPadMessage::kUp);
    std::cout << "Up clicked, sending command: " << static_cast<int>(up_command) << std::endl;
    serial_manager_.WriteBytesToSerial(&up_command, 1);
  });

  ui_->global<ModelState>().on_left_clicked([this]() {
    uint8_t left_command = static_cast<uint8_t>(DPadMessage::Left);
    std::cout << "Left clicked, sending command: " << static_cast<int>(left_command) << std::endl;
    serial_manager_.WriteBytesToSerial(&left_command, 1);
  });

  ui_->global<ModelState>().on_right_clicked([this]() {
    uint8_t right_command = static_cast<uint8_t>(DPadMessage::Right);
    std::cout << "Right clicked, sending command: " << static_cast<int>(right_command) << std::endl;
    serial_manager_.WriteBytesToSerial(&right_command, 1);
  });

  ui_->global<ModelState>().on_down_clicked([this]() {
    uint8_t down_command = static_cast<uint8_t>(DPadMessage::kDown);
    std::cout << "Down clicked, sending command: " << static_cast<int>(down_command) << std::endl;
    serial_manager_.WriteBytesToSerial(&down_command, 1);
  });

  // Set initial model state.

  ui_->global<ModelState>().invoke_refresh_serial_ports();

  std::vector<slint::SharedString> ui_baud_rates(kBaudRates.begin(), kBaudRates.end());
  auto ui_baud_rates_model = std::make_shared<slint::VectorModel<slint::SharedString>>(ui_baud_rates);
  ui_->global<ModelState>().set_baud_rates(ui_baud_rates_model);

  ui_->global<ModelState>().set_serial_connection_info(slint::SharedString(KSerialConnectInfo));

  // Verify initial page is the splash page.
  
  auto current_page = ui_->get_page();
  if (current_page == Page::Splash) std::cout << "UI is on the Splash page" << std::endl;

  // Initialise timer to periodically read from the serial port.

  serial_read_timer_.start(slint::TimerMode::Repeated, kSerialReadTimerInterval_ms, [this]() {
    uint8_t serial_input{};
    int64_t bytes_read = serial_manager_.ReadBytesFromSerial(&serial_input, 1);
    if (bytes_read > 0) {
      std::cout << "Byte from serial: " << static_cast<int>(serial_input) << std::endl;
    }
    else if (bytes_read < 0) {
      ui_->global<ModelState>().set_serial_connection_info(slint::SharedString(KSerialConnectInfo));
    }
  });
}

} // namespace slint_gui_app::application
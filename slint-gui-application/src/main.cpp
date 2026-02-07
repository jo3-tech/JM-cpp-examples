#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>

#include "serial/serial.h"
#include "asio.hpp"

#include "app_window.h"
#include "utils.hpp"

enum class DPadMessage : uint8_t {
  kUp = 'u', // 117
  kDown = 'd', // 100
  Left = 'l', // 108
  Right = 'r' // 114
};

auto ui = AppWindow::create();

serial::Serial serial_port{};

std::vector<serial::PortInfo> serial_ports_info{};
int current_serial_port_index = 0;

std::string serial_ports_empty_info = "None found.";

std::vector<slint::SharedString> baud_rates = { "115200", "57600", "38400", "19200", "9600" };
int current_baud_rate_index = 0;

serial::Timeout serial_read_timeout = serial::Timeout::simpleTimeout(1000); // (ms).

slint::Timer serial_read_timer{};
auto serial_read_timer_interval = std::chrono::milliseconds(100);
auto serial_read_timer_mode = slint::TimerMode::Repeated;

slint::SharedString ui_serial_connect_info = "Connect";
slint::SharedString ui_serial_disconnect_info = "Disconnect";

bool serial_port_open = false;

void refresh_serial_ports() {
  std::cout << "Refresh serial ports requested from UI." << std::endl;

  // Discover available serial ports.
  //std::vector<std::string> serial_port_ids = { "first", "second", "third" };
  serial_ports_info = serial::list_ports();

  // Get the friendly names into a slint::SharedString
  //auto serial_port_ids_temp = std::vector<slint::SharedString>(serial_port_ids.begin(), serial_port_ids.end());
  std::vector<slint::SharedString> serial_ports_names{};

  if (serial_ports_info.empty()) {
    std::cout << serial_ports_empty_info << std::endl;
    serial_ports_names.push_back(slint::SharedString(serial_ports_empty_info));
  }
  else {
    std::cout << "Found " << serial_ports_info.size() << " serial ports: " << std::endl;
    for (const auto& port_info : serial_ports_info) {
      serial_ports_names.push_back(slint::SharedString(port_info.description));
      std::cout << port_info.description << std::endl;
    }
  }
  
  // Convert to UI model (slint::VectorModel<slint::SharedString>).
  //auto ui_serial_port_ids_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_port_ids_temp);
  auto ui_serial_ports_names_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_ports_names);

  // Set the model in the UI.
  //ui->set_serial_ports(ui_serial_port_ids_model);
  ui->global<ModelState>().set_serial_ports(ui_serial_ports_names_model);
}

void connect_or_disconnect_serial() {
  if (!serial_port_open) {
    if (serial_ports_info.empty()) {
      std::cout << "No serial ports available to connect." << std::endl;
      return;
    }
    // Connect.
    int port_index = ui->global<ModelState>().get_serial_ports_index();
    std::cout << "Port index: " << port_index << std::endl;
    int baud_index = ui->global<ModelState>().get_baud_rates_index();
    std::cout << "Baud index: " << baud_index << std::endl;
    std::cout << "Connecting to: " << serial_ports_info[port_index].port << std::endl;
    std::cout << "Baudrate: " << std::string(baud_rates[baud_index]) << std::endl; 
    serial_port.setPort(serial_ports_info[port_index].port);
    serial_port.setBaudrate(std::stoi(std::string(baud_rates[baud_index])));
    serial_port.setTimeout(serial_read_timeout);
    serial_port.open();

    if(!serial_port.isOpen()) {
      std::cerr << "Failed to open port." << std::endl;
    }
    else {
      serial_port_open = true;
      serial_port.flush();
      ui->global<ModelState>().set_serial_connection_info(ui_serial_disconnect_info);
    }
  }
  else {
    // Disconnect.
    std::cout << "Disconnecting serial." << std::endl;
    serial_port.close();
    serial_port_open = false;
    ui->global<ModelState>().set_serial_connection_info(ui_serial_connect_info);
  }
}

size_t send_bytes_over_serial(const uint8_t* data, size_t length) {
  if (serial_port_open) {
    return serial_port.write(data, length);
  }
  else {
    std::cerr << "Serial port closed, cannot send data." << std::endl;
    return 0;
  }
}

size_t read_bytes_from_serial(uint8_t* buffer, size_t length) {
  if (serial_port_open) {
    size_t bytes_available = serial_port.available();
    if (bytes_available >= length) {
      return serial_port.read(buffer, length);
    }
    else {
      //std::cerr << "No data or not enough data available to read requested length." << std::endl;
      return 0;
    }
  }
  else {
    //std::cerr << "Serial port closed, cannot read data." << std::endl;
    return 0;
  }
}

int main(int argc, char **argv)
{
  // Register callback functions.

  ui->global<ModelState>().on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    utils::open_url(std::string(url_output));
  });

  ui->global<ModelState>().on_refresh_serial_ports([]() {
    refresh_serial_ports();
  });

  ui->global<ModelState>().on_connect_or_disconnect_serial([]() {
    connect_or_disconnect_serial();
  });

  ui->global<ModelState>().on_send_byte_over_serial([](int value_output) {
    uint8_t value = value_output;
    std::cout << "Send byte over serial: " << static_cast<int>(value) << std::endl;
    send_bytes_over_serial(&value, 1);
  });

  ui->global<ModelState>().on_up_clicked([]() {
    uint8_t up_command = static_cast<uint8_t>(DPadMessage::kUp);
    std::cout << "Up clicked, sending command: " << static_cast<int>(up_command) << std::endl;
    send_bytes_over_serial(&up_command, 1);
  });

  ui->global<ModelState>().on_left_clicked([]() {
    uint8_t left_command = static_cast<uint8_t>(DPadMessage::Left);
    std::cout << "Left clicked, sending command: " << static_cast<int>(left_command) << std::endl;
    send_bytes_over_serial(&left_command, 1);
  });

  ui->global<ModelState>().on_right_clicked([]() {
    uint8_t right_command = static_cast<uint8_t>(DPadMessage::Right);
    std::cout << "Right clicked, sending command: " << static_cast<int>(right_command) << std::endl;
    send_bytes_over_serial(&right_command, 1);
  });

  ui->global<ModelState>().on_down_clicked([]() {
    uint8_t down_command = static_cast<uint8_t>(DPadMessage::kDown);
    std::cout << "Down clicked, sending command: " << static_cast<int>(down_command) << std::endl;
    send_bytes_over_serial(&down_command, 1);
  });

  // Set initial model state.

  refresh_serial_ports();

  auto ui_baud_rates_model = std::make_shared<slint::VectorModel<slint::SharedString>>(baud_rates);
  ui->global<ModelState>().set_baud_rates(ui_baud_rates_model);

  ui->global<ModelState>().set_serial_connection_info(ui_serial_connect_info);

  // Initialise timer to periodically read from the serial port.

  serial_read_timer.start(serial_read_timer_mode, serial_read_timer_interval, []() {
    uint8_t serial_input{};
    size_t bytes_read{};
    try {
      bytes_read = read_bytes_from_serial(&serial_input, 1);
    }
    catch (const serial::IOException& e) {
    //catch (const std::exception& e) {
      serial_port_open = false;
      serial_port.close();
      refresh_serial_ports();      
      ui->global<ModelState>().set_serial_connection_info(ui_serial_connect_info);
      std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...) {
      std::cerr << "Unknown exception" << std::endl;
    }
    
    if (bytes_read > 0) {
      std::cout << "Byte from serial: " << static_cast<int>(serial_input) << std::endl;
    }
  });

  // Run the UI event loop.

  ui->run();

  return 0;
}

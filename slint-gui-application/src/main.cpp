#include <cstdint>
#include <iostream>
#include <string>

#include "serial/serial.h"
#include "asio.hpp"

#include "app_window.h"
#include "utils.hpp"

auto ui = AppWindow::create();

serial::Serial serial_port;

std::vector<serial::PortInfo> serial_ports_info;
int current_serial_port_index = 0;

std::string serial_ports_empty_info = "None found.";

std::vector<slint::SharedString> baud_rates = { "115200", "57600", "38400", "19200", "9600" };
int current_baud_rate_index = 0;

serial::Timeout serial_read_timeout = serial::Timeout::simpleTimeout(1000); // (ms).

//slint::SharedString serial_connect_info = "Connect";
//slint::SharedString serial_disconnect_info = "Disconnect";
//slint::SharedString serial_connection_info = serial_connect_info;

bool serial_port_open = false;

void refresh_serial_ports() {
  std::cout << "Refresh serial ports requested from UI." << std::endl;

  // Discover available serial ports.
  //std::vector<std::string> serial_port_ids = { "first", "second", "third" };
  std::vector<serial::PortInfo> serial_ports_info = serial::list_ports();

  // Get the friendly names into a slint::SharedString
  //auto serial_port_ids_temp = std::vector<slint::SharedString>(serial_port_ids.begin(), serial_port_ids.end());
  std::vector<slint::SharedString> serial_ports_names;

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
  //auto serial_port_ids_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_port_ids_temp);
  auto serial_ports_names_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_ports_names);

  // Set the model in the UI.
  //ui->set_serial_ports(serial_port_ids_model);
  ui->global<ModelState>().set_serial_ports(serial_ports_names_model);
}
/*
void connect_serial() {
  if (!serial_port_open) {
    uint8_t port_index = ui->global<ModelState>().get_serial_ports_index();
    uint8_t baud_index = ui->global<ModelState>().get_baud_rates_index();
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
    }
  }
}
*/
int main(int argc, char **argv)
{
  // Register callback functions.

  ui->global<ModelState>().on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    utils::open_url(std::string(url_output));
  });

  ui->global<ModelState>().on_refresh_serial_ports([&]() {
    refresh_serial_ports();
  });

  // Set model state.

  refresh_serial_ports();

  auto baud_rates_model = std::make_shared<slint::VectorModel<slint::SharedString>>(baud_rates);
  ui->global<ModelState>().set_baud_rates(baud_rates_model);

  //auto serial_connection_info_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_connection_info);
  //ui->global<ModelState>().

  ui->run();
  return 0;
}

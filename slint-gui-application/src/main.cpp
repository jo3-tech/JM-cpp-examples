#include <cstdint>
#include <iostream>
#include <string>

#include "serial/serial.h"
#include "asio.hpp"

#include "app_window.h"
#include "utils.hpp"

auto ui = AppWindow::create();

std::vector<serial::PortInfo> serial_ports_info;


void refresh_serial_ports() {
  std::cout << "Refresh serial ports requested from UI." << std::endl;

  // Discover available serial ports.
  //std::vector<std::string> serial_port_ids = { "first", "second", "third" };
  std::vector<serial::PortInfo> serial_ports_info = serial::list_ports();

  // Get the friendly names into a slint::SharedString
  //auto serial_port_ids_temp = std::vector<slint::SharedString>(serial_port_ids.begin(), serial_port_ids.end());
  std::vector<slint::SharedString> serial_ports_names;

  if (serial_ports_info.empty()) {
    std::cout << "No serial ports found." << std::endl;
    serial_ports_names.push_back(slint::SharedString("No serial ports found"));
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

void connect_serial() {
  std::string port_name = ui->global<ModelState>().get_serial_ports_index

}

int main(int argc, char **argv)
{
  ui->global<ModelState>().on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    utils::open_url(std::string(url_output));
  });

  ui->global<ModelState>().on_refresh_serial_ports([&]() {
    refresh_serial_ports();
  });

/*
  ui->on_request_increase_value([&]{
      ui->set_counter(ui->get_counter() + 1);
  });
*/
  ui->run();
  return 0;
}

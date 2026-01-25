#include <cstdint>
#include <iostream>
#include <string>

#include "serial/serial.h"
#include "asio.hpp"

#include "app_window.h"
#include "utils.hpp"

int main(int argc, char **argv)
{
  auto ui = AppWindow::create();

  ui->global<ModelState>().on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    utils::open_url(std::string(url_output));
  });

  // Serial

  // Discover available serial ports.
  //std::vector<std::string> serial_port_ids = { "first", "second", "third" };
  std::vector<serial::PortInfo> serial_ports_info = serial::list_ports();

  // Get the friendly names into a slint::SharedString
  //auto serial_port_ids_temp = std::vector<slint::SharedString>(serial_port_ids.begin(), serial_port_ids.end());
  std::cout << "Available serial ports: " << std::endl;
  std::vector<slint::SharedString> serial_ports_names;
  for (const auto& port_info : serial_ports_info) {
    serial_ports_names.push_back(slint::SharedString(port_info.description));
    std::cout << port_info.description << std::endl;
  }

  // Convert to UI model (slint::VectorModel<slint::SharedString>).
  //auto serial_port_ids_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_port_ids_temp);
  auto serial_ports_names_model = std::make_shared<slint::VectorModel<slint::SharedString>>(serial_ports_names);

  // Set the model on the UI.
  //ui->set_serial_ports(serial_port_ids_model);
  ui->global<ModelState>().set_serial_ports(serial_ports_names_model);

/*
  ui->on_request_increase_value([&]{
      ui->set_counter(ui->get_counter() + 1);
  });
*/
  ui->run();
  return 0;
}

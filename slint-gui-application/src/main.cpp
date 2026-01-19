#include "app_window.h"

#include "utils.hpp"
//#include <cstdint>
#include <iostream>
//#include <string>

int main(int argc, char **argv)
{
  auto ui = AppWindow::create();

/*
  ui->on_request_increase_value([&]{
      ui->set_counter(ui->get_counter() + 1);
  });
*/

  ui->on_open_github([](slint::SharedString url_output) {
    std::cout << "Open GitHub: " << url_output << std::endl;
    utils::open_url(static_cast<std::string>(url_output));
  });

  ui->run();
  return 0;
}

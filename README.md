# C++ Examples

Useful C++ examples and algorithm implementations.

This repository is a collection of examples, usually written during feasibility studies for the implementation of a library or project.

## slint-gui-application

![slint-gui-application demo](slint-gui-application/images/slint-gui-application%201080%20x%20720%20gif.gif)

![slint-gui-application demo](slint-gui-application/images/slint-gui-application%201080p%20gif.gif)

This example shows how to create a native cross platform graphical user interface (GUI) in C++. It also shows how to send/receive data over a USB/serial port.

The following libraries are used:

- [Slint](https://github.com/slint-ui/slint); a modern, cross-platform declarative UI toolkit.
- [wjwwood/serial](https://github.com/wjwwood/serial); a cross-platform serial communication library.

It has been developed to work on Windows, Linux and macOS. It has been tested on Windows 11 and Linux (Ubuntu 22.04 LTS).

Pre-requisites for all platforms:

- A C++ compiler that supports C++ 20.
- [Rust](https://rustup.rs)
- [CMake](https://cmake.org) (3.21 or newer)
- [Ninja](https://ninja-build.org)

The following is a simple counterpart program for the [Arduino platform](https://www.arduino.cc), for testing the serial communication aspect of this application via the GUI:

``` C++
void setup(){
  Serial.begin(115200);
}

void loop(){
  // Get serial data.
  if (Serial.available()) {
    char serial_input = Serial.read();
    // Echo/reply.
    Serial.print(serial_input);
  }
}
```

// Copyright (C) 2026 Joseph Morgridge
//
// Licensed under MIT License.
// See the LICENSE file in the project root for full license details.

/// @file utils.hpp
/// @brief Utility functions for the application.

#pragma once

#include <string>

namespace slint_gui_app::application {

/// @brief Open a URL in the default web browser.
/// @param url The URL to open.
void open_url(const std::string& url);

} // namespace slint_gui_app::application
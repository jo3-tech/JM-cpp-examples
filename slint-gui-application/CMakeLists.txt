# CMake properties.
cmake_minimum_required(VERSION 3.21)
#cmake_policy(SET CMP0155 NEW)

# Project properties.
project(slint_gui_application LANGUAGES CXX)

# Compiler settings.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Build type. 
# This should be set explicitly using: cmake -DCMAKE_BUILD_TYPE=Release ..
# OR in CMakePresets.json
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

# Package/library managers.
include(FetchContent) # CMake's FetchContent.

# Project dependencies.
find_package(Slint QUIET)
if (NOT Slint_FOUND)
  message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")
  FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    # `release/1` will auto-upgrade to the latest Slint >= 1.0.0 and < 2.0.0
    # `release/1.0` will auto-upgrade to the latest Slint >= 1.0.0 and < 1.1.0
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
  )
  FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

# Project executable.
add_executable(${CMAKE_PROJECT_NAME})

# Project sources.
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  src/main.cpp
  src/utils.cpp
)

# Project include paths.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  src/
)

# Project linked libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
  Slint::Slint
)

# Library targets.
slint_target_sources(${CMAKE_PROJECT_NAME} 
  ui/app_window.slint
)

# Custom build targets.
# On Windows, copy the Slint DLL next to the application binary so that it's found.
if (WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
      POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:slint_gui_application> $<TARGET_FILE_DIR:slint_gui_application> 
      COMMAND_EXPAND_LISTS
    )
endif()

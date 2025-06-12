# C++ Hello World with CMake and vcpkg

Main Article: https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd

This tutorial shows how to create a C++ "Hello World" program that uses the fmt library with CMake and vcpkg.

## Prerequisites

- A terminal
- A C++ compiler (MSVC for Windows users)
- CMake
- Git

## 1. Set up vcpkg

### Clone the repository

```bash
git clone https://github.com/microsoft/vcpkg.git
```

### Run the bootstrap script

Navigate to the vcpkg directory and run:

```bash
cd vcpkg && bootstrap-vcpkg.bat
```

## 2. Set up the project

### Configure environment variables

```bash
set "VCPKG_ROOT=C:\path\to\vcpkg"
set PATH=%VCPKG_ROOT%;%PATH%
```

> **Note:** These commands only affect the current terminal session. For permanent changes, set them through Windows System Environment Variables.

### Create project directory

```bash
mkdir helloworld && cd helloworld
```

## 3. Add dependencies and project files

### Create manifest file

```bash
vcpkg new --application
vcpkg add port fmt
```

This creates a `vcpkg.json` file:

```json
{
    "dependencies": [
        "fmt"
    ]
}
```

### Create CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld PRIVATE fmt::fmt)
```

### Create helloworld.cpp

```cpp
#include <fmt/core.h>

int main()
{
    fmt::print("Hello World!\n");
    return 0;
}
```

### Create CMake preset files

**CMakePresets.json:**

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "vcpkg",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

**CMakeUserPresets.json:**

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "vcpkg",
      "environment": {
        "VCPKG_ROOT": "<path to vcpkg>"
      }
    }
  ]
}
```

> **Note:** Replace `<path to vcpkg>` with your actual vcpkg installation path. Don't commit CMakeUserPresets.json to version control.

## 4. Build and run

### Configure with CMake

```bash
cmake --preset=default
```

### Build the project

```bash
cmake --build build
```

### Run the application

```bash
.\build\HelloWorld.exe
```

Expected output:
```
Hello World!
```

## Project Structure

Your final project should look like this:

```
helloworld/
├── CMakeLists.txt
├── CMakePresets.json
├── CMakeUserPresets.json
├── helloworld.cpp
├── vcpkg.json
├── vcpkg-configuration.json (auto-generated)
└── build/ (created during build)
```
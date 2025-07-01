# CMake Project Template

## Pre-Requisites
- Install Visual Studio Code
	- Add CMake Extension
- Install Microsoft Visual Studio 2022
	- Install v143
- Install vcpkg or conan

## Setup Package Managers
### Using vcpkg
Don't need to do anything, just configure normally

### Using conan
```shell
conan install . -s build_type=Debug -s compiler.cppstd=20 --build=missing # will locally build any missing deps, might take time at first call
```

This tutorial shows how to create a C++ "Hello World" program that uses the fmt library with CMake and vcpkg.

Articles:
- [Tutorial: Install and use packages with CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)
- [Tutorial: Install and use packages with CMake in Visual Studio](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vs)

## How to Run this?
### Preparation
- [Set Up `vcpkg`](#1-set-up-vcpkg) (Follow steps below on How to Recreate)
- [Manually create `CMakeUserPresets.json`](#create-cmake-preset-files) in `{Project Root}/CMakeUserPresets.json`
- Open vcpkg or
- Install all pre-configured packages
  ```bash
  vcpkg install
  ```

### Run in VS Code
- Run Task: `Configure`
- Run Task: `Clean -> Build -> Run`

> **Note:** You can check `.vscode/tasks.json` if you want to see what cmd script is being run for each task

## How to Recreate

### Prerequisites

- A terminal
- A C++ compiler (MSVC for Windows users)
- CMake
- Git

### 1. Set up vcpkg

#### Clone the repository

```bash
git clone https://github.com/microsoft/vcpkg.git

# optionally move to C:
mv vcpkg C:/
```

#### Run the bootstrap script

Navigate to the vcpkg directory and run:

```bash
cd vcpkg && bootstrap-vcpkg.bat
```

#### Integrate with Visual Studio MSBuild

```bash
.\vcpkg.exe integrate install
```

### 2. Set up the project

### 3. Add dependencies and project files

#### Create manifest file

- create `vcpkg.json` at root of project, add all needed dependencies

```json
{
    "dependencies": [
        "fmt",
        "paho-mqttpp3",
        "cppwinrt"
    ]
}
```

#### Create CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)
find_package(cppwinrt CONFIG REQUIRED)

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld
    PRIVATE
        windowsapp # REQUIRED!
        fmt::fmt
        PahoMqttCpp::paho-mqttpp3
        Microsoft::CppWinRT
)
```

#### Include all vcpkg headers

```cpp
#include <fmt/core.h>
#include <winrt/base.h>
#include <mqtt/async_client.h>
// ...

int main()
{
    // ...
}
```

#### Create CMake preset files

**CMakePresets.json:**

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "my-vcpkg-cmake-preset",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

> **Note:** For a more advanced `CMakePresets.json`, check out one in thsi project

**CMakeUserPresets.json:**

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "my-vcpkg-cmake-preset",
      "environment": {
        "VCPKG_ROOT": "<path to vcpkg>" // "VCPKG_ROOT": "C:/vcpkg"
      }
    }
  ]
}
```

> **Note:** Replace `<path to vcpkg>` with your actual vcpkg installation path. Don't commit CMakeUserPresets.json to version control.
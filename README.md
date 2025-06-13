# C++ Hello World with CMake and vcpkg

This tutorial shows how to create a C++ "Hello World" program that uses the fmt library with CMake and vcpkg.

Articles:
- [Tutorial: Install and use packages with CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)
- [Tutorial: Install and use packages with CMake in Visual Studio](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vs)

## How to Run this?
### Preparation
- [Set Up `vcpkg`](#1-set-up-vcpkg) (Follow steps below on How to Recreate)

- Install any vcpkg packages
  ```bash
  # example
  vcpkg install fmt:x64-windows
  vcpkg install paho-mqttpp3:x64-windows
  ```

- [Manually create `CMakeUserPresets.json`](#create-cmake-preset-files) in `{Project Root}/CMakeUserPresets.json`

### Run in VS Code
- Run Task: `Configure`
- Run Task: `Clean -> Build -> Run`

> **Note:** You can check `.vscode/tasks.json` if you want to see what cmd script is being run for each task

### Run in Visual Studio 2022 as CMake Project
- Open Visual Studio 2022 > File > Open > CMake...
- Choose `{Project Root}/CMakeLists.txt`
- Build > Build All
- Debug > Start Debugging

### Port into Visual Studio 2022 as C++ Console App
- Create new C++ Console App project in Visual Studio 2022 (or open an existing one)
- Add all necessary code that uses vcpkg libraries

### Port into Visual Studio 2022 as C++/WinRT Component
- Create new C++/WinRT Component project in Visual Studio 2022 (or open an existing one)
- Add all necessary code that uses vcpkg libraries
- Add all libraries to help Visual Studio 2022 find them
  - Project Properties > C/C++ > General > Additional Include Libraries
    ```bash
    # add libs included as seperate lines
    C:\vcpkg\installed\x64-windows\include
    C:\vcpkg\installed\x64-windows\include\mqtt
    ...
    ```
  - Project Properties > Linker > General > Additional Library Directories
    ```bash
    C:\vcpkg\installed\x64-windows\lib
    ```
  - Project Properties > Linker > Input > Additional Dependencies
    ```
    # add libs as seperate lines
    C:\vcpkg\installed\x64-windows\lib\paho-mqttpp3.lib
    C:\vcpkg\installed\x64-windows\lib\paho-mqtt3a.lib
    C:\vcpkg\installed\x64-windows\lib\paho-mqtt3c.lib
    ...
    ```
- Build > Build All
- Debug > Start Debugging

---

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

#### Configure environment variables (optional)

- Disclaimer: You can skip this entire step if using VSCode and had added `CMakePresets.json` and `CMakeUserPresets.json`, or are using Visual Studio and had already called `.\vcpkg.exe integrate install`
- Permanently add your Windows Environment Variables by adding `VCPKG_ROOT=C:\path\to\vcpkg` and `PATH=C:\path\to\vcpkg`
- or, temporarily add to your active terminal session by calling:

  ```bash
  set "VCPKG_ROOT=C:\path\to\vcpkg" # set "VCPKG_ROOT=C:\vcpkg"
  set PATH=%VCPKG_ROOT%;%PATH%
  ```

> **Note:** These commands only affect the current terminal session. For permanent changes, set them through Windows System Environment Variables.

#### Create project directory

```bash
mkdir helloworld && cd helloworld
```

### 3. Add dependencies and project files

#### Create manifest file

```bash
vcpkg new --application   # required step! will generate `vcpkg.json` and `vcpkg-configuration.json`
vcpkg add port fmt        # optional step
```

This creates a `vcpkg.json` file below. You can also just **add any dependencies by adding manually to the file**

```json
{
    "dependencies": [
        "fmt"
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

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld
    PRIVATE
        fmt::fmt
        PahoMqttCpp::paho-mqttpp3
)
```

#### Include all vcpkg headers

```cpp
#include <fmt/core.h>
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
        "VCPKG_ROOT": "<path to vcpkg>" // "VCPKG_ROOT": "C:/vcpkg"
      }
    }
  ]
}
```

> **Note:** Replace `<path to vcpkg>` with your actual vcpkg installation path. Don't commit CMakeUserPresets.json to version control.

### 4. Build and run

#### Configure with CMake

```bash
cmake --preset=default
```

#### Build the project

```bash
cmake --build build
```

#### Run the application

```bash
.\build\HelloWorld.exe
```

Expected output:
```
Hello World!
```

### Project Structure

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
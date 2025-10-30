## To Do cmake-vcpkg-winrt
- essentials
   - [ ] convert all to cpp23 (print)
   - [ ] refactor classes & functions
- poc
   - [ ] system testing pytest (in development in the pytest repo)
   - [ ] use msvc, use cpp23 import std & new custom module
- backlog
   - clone & learn from https://github.com/filipdutescu/modern-cpp-template
   - [ ] add doxygen to cmake

## To Do cmake-conan
- [ ] add conan as package manager
- [ ] add .devcontainer

## Future developments
- static analyzers
   - [ ] add clang tidy
   - [ ] add cppcheck configurations
   - [ ] add sanitizer
   - [ ] add CODING_STYLE.md
   - [ ] add coverage
      - dive deeper into [TheLartians/ModernCppStarter](https://github.com/TheLartians/ModernCppStarter)
- add fuzz testing
- documentation
   - [ ] add doxyfile
      - [ ] add build sphinx docs (+ doxygen)	
- setups
   - [ ] add advanced `.cmake` [rutura/CMakeSeries](https://github.com/rutura/CMakeSeries/tree/main/Ep034/rooster/cmake)
      - [ ] add version.cmake for versioning
      - [ ] add generate & add build success/failed in readme

## Done
- [x] add clang format
- [x] add windows.h com port listing
- [x] add gtest unit testing
- [x] add to `vcpkg.json` more verbose configurations
- [x] add CI script
	- [x] finding out if still requires "Developer Command Prompt Visual Studio 2022" to build
	- [x] ensure will also build outside windows (linux, macOS)
- [x] fix windows build, removing dependency to cl compiler
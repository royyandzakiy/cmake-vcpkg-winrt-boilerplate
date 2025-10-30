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
   - try developing via CLion

## To Do cmake-conan
- [ ] add conan as package manager
- [ ] add .devcontainer

## Future developments
- static analyzers
   - [ ] add clang tidy
      - https://www.kdab.com/clang-tidy-part-1-modernize-source-code-using-c11c14/
      - https://devblogs.microsoft.com/cppblog/visual-studio-code-c-december-2021-update-clang-tidy/
   - [ ] add cppcheck configurations
   - [ ] add sanitizer
   - [ ] add CODING_STYLE.md
   - [ ] add coverage
- add fuzz testing
- documentation
   - [ ] add doxyfile
      - [ ] add build sphinx docs (+ doxygen)	
- setups
   - [ ] add advanced `.cmake` [rutura/CMakeSeries](https://github.com/rutura/CMakeSeries/tree/main/Ep034/rooster/cmake)
      - [ ] add version.cmake for versioning
      - [ ] add generate & add build success/failed in readme
      - refactor with modern cmake https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/

## Done
- [x] add clang format
- [x] add windows.h com port listing
- [x] add gtest unit testing
- [x] add to `vcpkg.json` more verbose configurations
- [x] add CI script
	- [x] finding out if still requires "Developer Command Prompt Visual Studio 2022" to build
	- [x] ensure will also build outside windows (linux, macOS)
- [x] fix windows build, removing dependency to cl compiler
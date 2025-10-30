## To Do cmake-vcpkg-winrt
- essentials
   - [ ] convert all to cpp23
   - [ ] refactor classes & functions
- poc
   - [ ] system testing pytest (in development in the pytest repo)
   - [ ] cpp23 import std & new custom module
- nice to haves
	- [ ] add clang format
	- [ ] add clang tidy
	- [ ] add cppcheck configurations
	- [ ] add sanitizers
	- [ ] add CODING_STYLE.md
 - [ ] add coverage
  	- dive deeper into [TheLartians/ModernCppStarter](https://github.com/TheLartians/ModernCppStarter)
- [ ] add doxyfile
	- [ ] add build sphinx docs (+ doxygen)	
- [ ] add advanced `.cmake` [rutura/CMakeSeries](https://github.com/rutura/CMakeSeries/tree/main/Ep034/rooster/cmake)
	- [ ] add version.cmake for versioning
	- [ ] add generate & add build success/failed in readme

## To Do cmake-conan
- [ ] add conan as package manager
- [ ] add .devcontainer
	- [ ] add setup.cmake
- [ ] add sphynx docs generation
- [ ] add fuzztest

## Done
- [x] add windows.h com port listing
- [x] add gtest unit testing
- [x] add to `vcpkg.json` more verbose configurations
- [x] add CI script
	- [x] finding out if still requires "Developer Command Prompt Visual Studio 2022" to build
	- [x] ensure will also build outside windows (linux, macOS)
- [x] fix windows build, removing dependency to cl compiler
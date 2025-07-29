## To Do
- [ ] add .clang-tidy, .clang-format & cppcheck configurations
	- dive deeper into [TheLartians/ModernCppStarter](https://github.com/TheLartians/ModernCppStarter)
- [ ] add CODING_STYLE.md
- [ ] add doxyfile
	- [ ] add build sphinx docs (+ doxygen)	
- [ ] add conan to make package manager agnostic
- [ ] add advanced `.cmake` [rutura/CMakeSeries](https://github.com/rutura/CMakeSeries/tree/main/Ep034/rooster/cmake)
	- [ ] add version.cmake for versioning
	- [ ] add test coverage (gcov)
	- [ ] add generate & add build success/failed in readme
- [ ] add more tests

## Done
- [x] add to `vcpkg.json` more verbose configurations
- [x] add CI script
	- [x] finding out if still requires "Developer Command Prompt Visual Studio 2022" to build
	- [x] ensure will also build outside windows (linux, macOS)
- [x] fix windows build, removing dependency to cl compiler
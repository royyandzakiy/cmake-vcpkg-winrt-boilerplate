# cmake/SetupWindowsDeps.cmake
cmake_minimum_required(VERSION 3.20)

message(STATUS "Running Windows dependency setup...")

# --- Admin privileges check (best effort) ---
execute_process(
    COMMAND net session
    RESULT_VARIABLE IS_ADMIN
    OUTPUT_QUIET ERROR_QUIET
)
if(NOT IS_ADMIN EQUAL 0)
    message(WARNING "This setup ideally requires administrator privileges to modify PATH and install tools.")
endif()

# --- Configuration ---
set(VCPKG_ROOT "C:/vcpkg" CACHE PATH "Path to vcpkg")
set(GIT_PATH "C:/Program Files/Git" CACHE PATH "Git install path")
set(DOXYGEN_PATH "C:/Program Files/doxygen" CACHE PATH "Doxygen install path")

# --- Helper macro ---
function(download_and_run name url installer_args)
    set(tmpfile "${CMAKE_BINARY_DIR}/${name}-installer.exe")
    message(STATUS "Downloading ${name} from ${url}")
    file(DOWNLOAD "${url}" "${tmpfile}" SHOW_PROGRESS)
    execute_process(
        COMMAND "${tmpfile}" ${installer_args}
        RESULT_VARIABLE install_result
    )
    if(NOT install_result EQUAL 0)
        message(FATAL_ERROR "Failed to install ${name} (code ${install_result})")
    endif()
    file(REMOVE "${tmpfile}")
endfunction()

# --- Git ---
message(STATUS "Checking Git installation...")
find_program(GIT_EXE git PATHS "${GIT_PATH}/bin")
if(NOT GIT_EXE)
    message(STATUS "Git not found. Installing Git for Windows...")
    download_and_run(
        "git"
        "https://github.com/git-for-windows/git/releases/download/v2.45.1.windows.1/Git-2.45.1-64-bit.exe"
        "/VERYSILENT /NORESTART /NOCANCEL /SP- /CLOSEAPPLICATIONS /RESTARTAPPLICATIONS /COMPONENTS=icons,ext\\reg\\shellhere,assoc,assoc_sh"
    )
else()
    message(STATUS "Git already installed: ${GIT_EXE}")
endif()

# --- Doxygen ---
message(STATUS "Checking Doxygen installation...")
find_program(DOXYGEN_EXE doxygen PATHS "${DOXYGEN_PATH}/bin")
if(NOT DOXYGEN_EXE)
    message(STATUS "Doxygen not found. Installing...")
    download_and_run(
        "doxygen"
        "https://www.doxygen.nl/files/doxygen-1.10.0-setup.exe"
        "/VERYSILENT /NORESTART /SUPPRESSMSGBOXES"
    )
else()
    message(STATUS "Doxygen already installed: ${DOXYGEN_EXE}")
endif()

# --- vcpkg ---
message(STATUS "Checking vcpkg installation...")
if(NOT EXISTS "${VCPKG_ROOT}/vcpkg.exe")
    message(STATUS "vcpkg not found. Cloning and bootstrapping...")
    execute_process(
        COMMAND "${GIT_EXE}" clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}"
        RESULT_VARIABLE git_clone_result
    )
    if(NOT git_clone_result EQUAL 0)
        message(FATAL_ERROR "Failed to clone vcpkg repository.")
    endif()

    execute_process(
        COMMAND cmd /c bootstrap-vcpkg.bat -disableMetrics
        WORKING_DIRECTORY "${VCPKG_ROOT}"
    )
else()
    message(STATUS "vcpkg already installed: ${VCPKG_ROOT}")
endif()

# --- Set environment ---
set(ENV{VCPKG_ROOT} "${VCPKG_ROOT}")
message(STATUS "Environment variable set: VCPKG_ROOT=$ENV{VCPKG_ROOT}")

message(STATUS "============================================")
message(STATUS " Windows setup completed successfully! ")
message(STATUS "--------------------------------------------")
message(STATUS " Git:      ${GIT_PATH}")
message(STATUS " Doxygen:  ${DOXYGEN_PATH}")
message(STATUS " vcpkg:    ${VCPKG_ROOT}")
message(STATUS "============================================")

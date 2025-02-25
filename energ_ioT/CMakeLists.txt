# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(energ_ioT C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Alvo 2: Modo simulação
add_executable(simulation automatic_simulation.c)
target_link_libraries(simulation pico_stdlib hardware_uart)
pico_add_extra_outputs(simulation)

# Alvo 3: Modo terminal
add_executable(terminal terminal_simulation.c)
target_link_libraries(terminal pico_stdlib)
pico_add_extra_outputs(terminal)

# Add executable. Default name is the project name, version 0.1

add_executable(energ_ioT energ_ioT.c )

pico_set_program_name(energ_ioT "energ_ioT")
pico_set_program_version(energ_ioT "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(energ_ioT 0)
pico_enable_stdio_usb(energ_ioT 1)

# Add the standard library to the build
target_link_libraries(energ_ioT
        pico_stdlib hardware_uart pico_cyw43_arch pico_lwip lwip)

# Add the standard include files to the build
target_include_directories(energ_ioT PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${PICO_SDK_PATH}/lib/cyw43-driver/src
        ${PICO_SDK_PATH}/lib/lwip/src/include
)

# Add any user requested libraries
target_link_libraries(energ_ioT 
        
        )

pico_add_extra_outputs(energ_ioT)


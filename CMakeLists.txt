cmake_minimum_required(VERSION 3.17)
include(pico_sdk_import.cmake)
set(ENV{PICO_SDK_PATH} D:/Projects/pico/pico-sdk/)

project(ssd1351 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(ssd1351 main.cpp sprite.h ssd1351.cpp ssd1351.h)
pico_sdk_init()

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(ssd1351 pico_stdlib hardware_spi hardware_pwm)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(ssd1351)

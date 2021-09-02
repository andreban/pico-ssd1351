add_library(ssd1351 INTERFACE)

target_sources(ssd1351 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/ssd1351.cpp
        )

target_include_directories(ssd1351 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(ssd1351 INTERFACE pico_stdlib hardware_spi)
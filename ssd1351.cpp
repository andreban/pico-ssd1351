#include "ssd1351.h"
#include "pico/stdlib.h"

void Ssd1351::sendCommand(uint8_t command, uint8_t *data, uint length) {
    gpio_put(cs_pin, 0); // Set CS to LOW to select device.
    gpio_put(dc_pin, 0); // Set DC to LOW for sending COMMAND.
    spi_write_blocking(spi, &command, 1);
    if (data) {
        gpio_put(dc_pin, 1); // Set DC to HIGH for sending DATA.
        spi_write_blocking(spi, data, length);
    }
    gpio_put(cs_pin, 1); // Set CS to HIGH to deselect device.
}

void Ssd1351::sendData(uint8_t *data, uint length) {
    gpio_put(cs_pin, 0); // Set CS to LOW to select device.
    gpio_put(dc_pin, 1); // Set DC to HIGH for sending DATA.
    spi_write_blocking(spi, data, length);
    gpio_put(cs_pin, 1); // set CS to HIGH to deselect device.
}

void Ssd1351::init() {
    reset();
    adafruitInit();
    // pythonInit();
    fill(0xFFFF);
}

void Ssd1351::invertDisplay(bool invert) {
    sendCommand(invert ? SSD1351_CMD_INVERTDISPLAY : SSD1351_CMD_NORMALDISPLAY);
}

void Ssd1351::reset() {
    gpio_put(rst_pin, 1);
    sleep_ms(50);
    gpio_put(rst_pin, 0);
    sleep_ms(50);
    gpio_put(rst_pin, 1);
    sleep_ms(150);
}

void Ssd1351::adafruitInit() {
    sendCommand(SSD1351_CMD_COMMANDLOCK, 0x12);
    sendCommand(SSD1351_CMD_COMMANDLOCK, 0xB1);
    sendCommand(SSD1351_CMD_DISPLAYOFF);
    sendCommand(SSD1351_CMD_CLOCKDIV, 0xF1); // 7:4 = Oscillator Freq, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    sendCommand(SSD1351_CMD_MUXRATIO, 127);
    sendCommand(SSD1351_CMD_DISPLAYOFFSET, 0x0);
    sendCommand(SSD1351_CMD_SETGPIO, 0x00);
    sendCommand(SSD1351_CMD_FUNCTIONSELECT, 0x01); // internal (diode drop)
    sendCommand(SSD1351_CMD_PRECHARGE, 0x32);
    sendCommand(SSD1351_CMD_VCOMH, 0x05);
    sendCommand(SSD1351_CMD_NORMALDISPLAY);
    sendCommand(SSD1351_CMD_STARTLINE, 0x00); // Set Display start line
    sendCommand(SSD1351_CMD_CONTRASTABC, new uint8_t[3]{0xFF, 0xFF, 0xFF}, 3);
    sendCommand(SSD1351_CMD_CONTRASTMASTER, 0x0F);
    sendCommand(SSD1351_CMD_SETVSL, new uint8_t[3]{0xA0, 0xB5, 0x55}, 3);
    sendCommand(SSD1351_CMD_PRECHARGE2, 0x01);
    sendCommand(SSD1351_CMD_DISPLAYON);
    // madctl bits:
    // 6,7 Color depth (01 = 64K)
    // 5   Odd/even split COM (0: disable, 1: enable)
    // 4   Scan direction (0: top-down, 1: bottom-up)
    // 3   Reserved
    // 2   Color remap (0: A->B->C, 1: C->B->A)
    // 1   Column remap (0: 0-127, 1: 127-0)
    // 0   Address increment (0: horizontal, 1: vertical)
    sendCommand(SSD1351_CMD_SETREMAP, 0b01110100); // Segment remapping
}

/**
 * https://github.com/rdagger/micropython-ssd1351/blob/8e386c5c1c849c7ab3454bd9c02fa042975b1919/ssd1351.py#L91-L111
 */
void Ssd1351::pythonInit() {
    sendCommand(SSD1351_CMD_COMMANDLOCK, 0x12); // Unlock IC MCU interface
    sendCommand(SSD1351_CMD_COMMANDLOCK, 0xB1); // A2,B1,B3,BB,BE,C1
    sendCommand(SSD1351_CMD_DISPLAYOFF); // Display off
    sendCommand(SSD1351_CMD_DISPLAYENHANCE, new uint8_t[3]{0xA4, 0x00, 0x00}, 3);
    sendCommand(SSD1351_CMD_CLOCKDIV, 0xF0);  // Clock divider F1 or F0
    sendCommand(SSD1351_CMD_MUXRATIO, 0x7F); // Mux ratio
    sendCommand(SSD1351_CMD_SETREMAP, 0x74); // Segment remapping
    sendCommand(SSD1351_CMD_STARTLINE, 0x00); // Set Display start line
    sendCommand(SSD1351_CMD_DISPLAYOFFSET, 0x00); // Set display offset
    sendCommand(SSD1351_CMD_SETGPIO, 0x00); // Set GPIO
    sendCommand(SSD1351_CMD_FUNCTIONSELECT, 0x01); // Function select
    sendCommand(SSD1351_CMD_PRECHARGE, 0x32); // Precharge
    sendCommand(SSD1351_CMD_PRECHARGELEVEL, 0x1F); // Precharge level
    sendCommand(SSD1351_CMD_VCOMH, 0x05); // Set VcomH voltage
    sendCommand(SSD1351_CMD_NORMALDISPLAY); // Normal Display
    sendCommand(SSD1351_CMD_CONTRASTMASTER, 0x0A); // Contrast master
    sendCommand(SSD1351_CMD_CONTRASTABC, new uint8_t[3]{0xFF, 0xFF, 0xFF}, 3); // Contrast RGB
    sendCommand(SSD1351_CMD_SETVSL, new uint8_t[3]{0xA0, 0xB5, 0x55}, 3); //Set segment low volt.
    sendCommand(SSD1351_CMD_PRECHARGE2, 0x01); // Precharge2
    sendCommand(SSD1351_CMD_DISPLAYON); // Display on
}

void Ssd1351::fill(uint16_t color) {
    sendCommand(SSD1351_CMD_SETCOLUMN, new uint8_t[2]{0x00, 0x7F}, 2);
    sendCommand(SSD1351_CMD_SETROW, new uint8_t[2]{0x00, 0x7F}, 2);
    sendCommand(SSD1351_CMD_WRITERAM);

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            sendData((uint8_t *)&color, 2);
        }
    }
}

void Ssd1351::drawImage(uint16_t *image) {
    sendCommand(SSD1351_CMD_SETCOLUMN, new uint8_t[2]{0x00, 0x7F}, 2);
    sendCommand(SSD1351_CMD_SETROW, new uint8_t[2]{0x00, 0x7F}, 2);
    sendCommand(SSD1351_CMD_WRITERAM);
    sendData((uint8_t *)image, width * height * 2);
}

void Ssd1351::setMasterBrightness(uint8_t brightness) {
    // Clamp brightness to 16.
    if (brightness >= 0x0F) {
        brightness = 0x0F;
    }
    sendCommand(SSD1351_CMD_CONTRASTMASTER, brightness);
}

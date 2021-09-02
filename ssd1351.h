
/*
 * Implementation of Adafruit's ssd1351 driver for the RP2040
 * This library should work with Adafruit's 1.5" color OLED:
 *     http://www.adafruit.com/products/1431
 * and 1.27" color OLED:
 *     http://www.adafruit.com/products/1673
 *
 * Adafruit's source code:
 *     https://github.com/adafruit/Adafruit-SSD1351-library/
 */
#ifndef SSD1351_H
#define SSD1351_H

#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "hardware/spi.h"

#define SSD1351_CMD_SETCOLUMN 0x15
#define SSD1351_CMD_SETROW 0x75
#define SSD1351_CMD_WRITERAM 0x5C
#define SSD1351_CMD_READRAM 0x5D
#define SSD1351_CMD_SETREMAP 0xA0
#define SSD1351_CMD_STARTLINE 0xA1
#define SSD1351_CMD_DISPLAYOFFSET 0xA2
#define SSD1351_CMD_DISPLAYALLOFF 0xA4
#define SSD1351_CMD_DISPLAYALLON 0xA5
#define SSD1351_CMD_NORMALDISPLAY 0xA6
#define SSD1351_CMD_INVERTDISPLAY 0xA7
#define SSD1351_CMD_FUNCTIONSELECT 0xAB
#define SSD1351_CMD_DISPLAYOFF 0xAE
#define SSD1351_CMD_DISPLAYON 0xAF
#define SSD1351_CMD_PRECHARGE 0xB1
#define SSD1351_CMD_DISPLAYENHANCE 0xB2
#define SSD1351_CMD_CLOCKDIV 0xB3
#define SSD1351_CMD_SETVSL 0xB4
#define SSD1351_CMD_SETGPIO 0xB5
#define SSD1351_CMD_PRECHARGE2 0xB6
#define SSD1351_CMD_SETGRAY 0xB8
#define SSD1351_CMD_USELUT 0xB9
#define SSD1351_CMD_PRECHARGELEVEL 0xBB
#define SSD1351_CMD_VCOMH 0xBE
#define SSD1351_CMD_CONTRASTABC 0xC1
#define SSD1351_CMD_CONTRASTMASTER 0xC7
#define SSD1351_CMD_MUXRATIO 0xCA
#define SSD1351_CMD_COMMANDLOCK 0xFD
#define SSD1351_CMD_HORIZSCROLL 0x96
#define SSD1351_CMD_STOPSCROLL 0x9E
#define SSD1351_CMD_STARTSCROLL 0x9F

class Ssd1351 {
private:
    uint8_t width, height, sck_pin, mosi_pin;
    int8_t cs_pin, dc_pin, rst_pin;
    spi_inst *spi;
    void adafruitInit();
    void pythonInit();
public:
    Ssd1351(uint8_t width, uint8_t height, spi_inst *spi, uint8_t sck_pin, uint8_t mosi_pin, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1):
    width(width), height(height), spi(spi), sck_pin(sck_pin), mosi_pin(mosi_pin), cs_pin(cs_pin), dc_pin(dc_pin), rst_pin(rst_pin) {
        gpio_set_function(dc_pin, GPIO_FUNC_SIO); // Same as "gpio_init(PIN_DC);
        gpio_set_dir(dc_pin, GPIO_OUT);

        gpio_set_function(rst_pin, GPIO_FUNC_SIO); // gpio_init(PIN_RESET);
        gpio_set_dir(rst_pin, GPIO_OUT);

        gpio_set_function(cs_pin, GPIO_FUNC_SIO);
        gpio_set_dir(cs_pin, GPIO_OUT);

        gpio_put(cs_pin, 1);
        gpio_put(dc_pin, 0);
        gpio_put(rst_pin, 1);

        spi_init(spi, 1000000);
        gpio_set_function(sck_pin, GPIO_FUNC_SPI);
        gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    }

    void sendCommand(uint8_t command) {
        sendCommand(command, nullptr, 0);
    }

    void sendCommand(uint8_t command, uint8_t data) {
        sendCommand(command, &data, 1);
    }
    void sendCommand(uint8_t command, uint8_t *data, uint length);
    void sendData(uint8_t *data, uint length);
    void init();
    void invertDisplay(bool invert);
    void reset();
    void fill(uint16_t color);
    void drawImage(uint16_t *image);
    /**
     * @param brightness a number between 0 and 16
     */
    void setMasterBrightness(uint8_t brightness);
};

#endif // SSD1351_H

#include "hardware/spi.h"
#include "ssd1351.h"
#include "sprite.h"

const uint WIDTH = 128;
const uint HEIGHT = 128;

// Pins for the OLED screen.
const int8_t PIN_SPI1_SCK_SCL = 10;
const int8_t PIN_SPI1_MOSI_SDA = 11;
const int8_t PIN_CS = 13;
const int8_t PIN_DC = 14;
const int8_t PIN_RESET = 15;
const int8_t PIN_SPI1_MISO_SDA = 12;
spi_inst *SPI = spi1;

const uint8_t PIN_ONBOARD_LED = PICO_DEFAULT_LED_PIN;

int main() {
    gpio_set_function(PIN_ONBOARD_LED, GPIO_FUNC_SIO); //  gpio_init(PIN_ONBOARD_LED);
    gpio_set_dir(PIN_ONBOARD_LED, GPIO_OUT);
    gpio_put(PIN_ONBOARD_LED, 0);

    Ssd1351 display = Ssd1351(
            WIDTH, HEIGHT, SPI, PIN_SPI1_SCK_SCL, PIN_SPI1_MOSI_SDA, PIN_CS, PIN_DC, PIN_RESET);
    display.init();
    display.fill(0x001F);
    display.drawImage(llama_sprite);
    for (int i = 0; i < 16; i++) {
        gpio_put(PIN_ONBOARD_LED, 1);
        sleep_ms(1000);
        display.setMasterBrightness(i);
        gpio_put(PIN_ONBOARD_LED, 0);
    }
    return 0;
}
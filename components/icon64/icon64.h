/* Icon64 board related definitions and helper functions.
 * https://thingpulse.com/product/icon64/
 *
 * This code is in public domain, or, at your option, CC0-licensed.
 */

#pragma once

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Pin definitions */

#define GPIO_I2S_DOUT      25
#define GPIO_I2S_BCLK      26
#define GPIO_I2S_LRC       22
#define GPIO_I2S_MODE      33

#define GPIO_LED_CHAIN     32
#define NUM_LEDS           64
#define NUM_LEDS_X         8
#define NUM_LEDS_Y         8

/**
 * @brief Initialize I2S peripheral pins
 * Calls i2s_set_pin with the pins specific to this board.
 * 
 * @param i2s_num I2S peripheral to set up
 * @return ESP_OK on success
 */
esp_err_t icon64_i2s_pins_init(int i2s_num);

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

esp_err_t icon64_leds_init(void);
esp_err_t icon64_leds_set_all(rgb_t color);
esp_err_t icon64_leds_set(int led_num, rgb_t color);
esp_err_t icon64_leds_set_xy(int x, int y, rgb_t color);
esp_err_t icon64_leds_update(void);

#ifdef __cplusplus
}
#endif

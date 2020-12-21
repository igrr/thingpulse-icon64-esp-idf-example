/* Icon64 board related helper functions.
 * https://thingpulse.com/product/icon64/
 *
 * This code is in public domain, or, at your option, CC0-licensed.
 */

#include "icon64.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "driver/rmt.h"
#include "led_strip.h"

static led_strip_t* s_led_strip;

esp_err_t icon64_i2s_pins_init(int i2s_num)
{
    gpio_config_t gpio_cfg = {
        .pin_bit_mask = BIT64(GPIO_I2S_MODE),
        .mode = GPIO_MODE_OUTPUT
    };
    esp_err_t err = gpio_config(&gpio_cfg);
    if (err != ESP_OK) {
        return err;
    }
    gpio_set_level(GPIO_I2S_MODE, 1);

    const i2s_pin_config_t pin_config = {
        .bck_io_num = GPIO_I2S_BCLK,
        .ws_io_num = GPIO_I2S_LRC,
        .data_out_num = GPIO_I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    return i2s_set_pin(i2s_num, &pin_config);
}

esp_err_t icon64_leds_init(void)
{
    if (s_led_strip != NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(GPIO_LED_CHAIN, RMT_CHANNEL_0);
    config.clk_div = 2;
    esp_err_t err = rmt_config(&config);
    if (err != ESP_OK) {
        return err;
    }
    err = rmt_driver_install(config.channel, 0, ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM);
    if (err != ESP_OK) {
        return err;
    }
    const led_strip_config_t cfg = LED_STRIP_DEFAULT_CONFIG(NUM_LEDS, 0);
    s_led_strip = led_strip_new_rmt_ws2812(&cfg);
    if (s_led_strip == NULL) {
        return ESP_ERR_NO_MEM;
    }
    s_led_strip->clear(s_led_strip, 100);
    return ESP_OK;
}

esp_err_t icon64_leds_set_all(rgb_t color)
{
    if (s_led_strip == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    for (int i = 0; i < 64; ++i) {
        esp_err_t err = icon64_leds_set(i, color);
        if (err != ESP_OK) {
            return err;
        }
    }
    return ESP_OK;
}

esp_err_t icon64_leds_set(int led_num, rgb_t color)
{
    if (s_led_strip == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    if (led_num < 0 || led_num >= NUM_LEDS) {
        return ESP_ERR_INVALID_ARG;
    }
    return s_led_strip->set_pixel(s_led_strip, led_num, color.r, color.g, color.b);
}

esp_err_t icon64_leds_set_xy(int x, int y, rgb_t color)
{
    if (s_led_strip == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    if (x < 0 || x >= NUM_LEDS_X ||
        y < 0 || y >= NUM_LEDS_Y) {
        return ESP_ERR_INVALID_ARG;
    }
    return icon64_leds_set(x + y * 8, color);
}

esp_err_t icon64_leds_update(void)
{
    if (s_led_strip == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return s_led_strip->refresh(s_led_strip, 100);
}


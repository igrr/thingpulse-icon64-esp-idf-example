#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/i2s.h"
#include "example.h"
#include "wav.h"
#include "icon64.h"

#define I2S_NUM  0

static const char* TAG = "play";

static void init_i2s(int sample_rate)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = sample_rate,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .dma_buf_count = 10,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1 | ESP_INTR_FLAG_IRAM
    };

    ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL));
    ESP_ERROR_CHECK(icon64_i2s_pins_init(I2S_NUM));
    ESP_ERROR_CHECK(i2s_set_clk(I2S_NUM, sample_rate, 16, 2));
}

static void deinit_i2s(void)
{
    i2s_stop(I2S_NUM);
    i2s_driver_uninstall(I2S_NUM);
}

void play_wav(const char* filename, float volume)
{
    WavFile* f = wav_open(filename, "rb");
    if (!f) {
        ESP_LOGE(TAG, "failed to open %s", filename);
        return;
    }

    int sample_rate = wav_get_sample_rate(f);
    ESP_LOGI(TAG, "File %s: Channels: %d Sample rate: %d Bits per sample: %d",
        filename, wav_get_num_channels(f), sample_rate, wav_get_valid_bits_per_sample(f)
    );

    assert(wav_get_num_channels(f) == 2 && wav_get_valid_bits_per_sample(f) == 16 && "TODO: support other formats");

    init_i2s(sample_rate);

    size_t buffer_size_bytes = 1024;
    int16_t* buffer = (int16_t*) malloc(buffer_size_bytes);
    size_t bytes_per_frame = wav_get_num_channels(f) * (wav_get_valid_bits_per_sample(f) / 8);
    size_t frames_per_buffer = buffer_size_bytes / bytes_per_frame;
    while (true) {
        size_t frames_read = wav_read(f, buffer, frames_per_buffer);
        if (frames_read == 0) {
            break;
        }
        for (size_t i = 0; i < buffer_size_bytes / sizeof(int16_t); ++i) {
            buffer[i] = buffer[i] * volume;
        }
        size_t bytes_written = 0;
        ESP_ERROR_CHECK(i2s_write(I2S_NUM, buffer, bytes_per_frame * frames_read, &bytes_written, portMAX_DELAY));
        if (frames_read < frames_per_buffer) {
            break;
        }
    }
    free(buffer);
    wav_close(f);
    deinit_i2s();
    ESP_LOGD(TAG, "heap: %d\n", esp_get_free_heap_size());
}


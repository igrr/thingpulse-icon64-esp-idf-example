#include <thread>
#include <chrono>
#include "esp_err.h"
#include "icon64.h"
#include "example.h"

static void do_demo(void);

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(icon64_leds_init());
    ESP_ERROR_CHECK(storage_init());

    do_demo();
}

static void do_demo(void)
{
    std::thread sound_thread([](){
        play_wav("/spiffs/Neptunium.wav", 0.3);
    });

    auto set_all_leds = [](const rgb_t& color) {
        icon64_leds_set_all(color);
        icon64_leds_update();
    };

    std::tuple<rgb_t, int> sequence[] = {
        {{255, 0, 0}, 200},
        {{0, 0, 0}, 100},
        {{0, 255, 0}, 200},
        {{0, 0, 0}, 100},
        {{0, 0, 255}, 200},
        {{0, 0, 0}, 500},

        {{255, 0, 0}, 200},
        {{0, 0, 0}, 100},
        {{0, 255, 0}, 200},
        {{0, 0, 0}, 150},
        {{0, 0, 255}, 200},
        {{0, 0, 0}, 150},

    };
    for (auto& it: sequence) {
        set_all_leds(std::get<0>(it));
        std::this_thread::sleep_for(std::chrono::milliseconds(std::get<1>(it)));
    }
    sound_thread.join();
}

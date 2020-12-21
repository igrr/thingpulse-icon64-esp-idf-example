# Icon64 Example

This is a simple "getting started" example for the [ThingPulse Icon64](https://thingpulse.com/product/icon64/) device. Icon64 includes 64 WS2812B RGB LEDs, an MAX98357A I2S DAC with built-in amplifier, and an ESP32 WROVER-B module.

The example plays a .wav file and blinks the LEDs. See [main/example_main.cpp](main/example_main.cpp) — there is not much to it.

Other supporting components of the example include:

* [WAV reader](components/wav) library from https://github.com/brglng/libwav/.
* [icon64 helper](components/icon64) for board-specific functions
* code to [mount an SPIFFS partition](main/storage.c)
* code to [play a WAV file over I2S](main/play.c)


### Build and Flash

You need ESP-IDF 4.x to build this project. See the ESP-IDF Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

When IDF is set up, use the following command to build the project, flash it to the board, and run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

Files from [data](data/) directory are automatically uploaded to an SPIFFS partition when `idf.py flash` is executed. You can use this partition to store sounds and images.

To build and flash the app only (which is faster), run instead:
```
idf.py -p PORT app-flash monitor
```

### Credits

Thanks to [@squix78](https://github.com/squix78) for sending me an Icon64 board!

[WAV](components/wav) library from https://github.com/brglng/libwav/ is licensed under MPL license. You might consider using something else if you are building a commercial project.

WAV files in [data](data/) directory are from [AOSP project](https://android.googlesource.com/platform/frameworks/base/+/android-6.0.1_r68/data/sounds/alarms/wav/), which is Apache 2.0 licensed.

The rest of the code is based on ESP-IDF examples. It is released into public domain or CC0-licensed, at your option.

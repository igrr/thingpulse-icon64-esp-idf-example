#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t storage_init(void);
void play_wav(const char* filename, float volume);

#ifdef __cplusplus
}
#endif

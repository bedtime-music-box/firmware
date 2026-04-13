/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "driver/sdmmc_defs.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include "sdkconfig.h"

#include "fs.h"

static const char *TAG = "fs";
static const char *mount_point = "/sdcard";

bool fs_init(spi_host_device_t host_id)
{
    // Initialize the SD host
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = host_id;

    sdspi_device_config_t device_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    device_config.gpio_cs = (gpio_num_t)CONFIG_PIN_SD_CS;
    device_config.host_id = host_id;

    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };
    sdmmc_card_t *card;
    if (esp_vfs_fat_sdspi_mount(
            mount_point,
            &host,
            &device_config,
            &mount_cfg,
            &card
        ) != ESP_OK) {
        ESP_LOGE(TAG, "esp_vfs_fat_sdspi_mount() failed");
        return false;
    }

    FILE *f = fopen("/sdcard/test.txt", "r");
    if (!f) {
        ESP_LOGE(TAG, "Unable to open file");
    } else {
        char line[128];
        while (fgets(line, sizeof(line), f)) {
            ESP_LOGI(TAG, "Line: %s", line);
        }
        fclose(f);
    }

    // Everything was successful
    return true;
}

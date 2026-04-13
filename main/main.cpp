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

#include "driver/spi_master.h"
#include "esp_log.h"

#include "sdkconfig.h"

#include "fs.h"
#include "ui.h"

static const char *TAG = "main";

extern "C" void app_main()
{
    // Initialize SPI bus
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    spi_bus_config_t bus_config = {
        .mosi_io_num = CONFIG_PIN_SPI_MOSI,
        .miso_io_num = CONFIG_PIN_SPI_MISO,
        .sclk_io_num = CONFIG_PIN_SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
#pragma GCC diagnostic pop
    ESP_ERROR_CHECK(
        spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CH_AUTO)
    );

    if (!ui_init(SPI2_HOST)) {
        ESP_LOGE(TAG, "display error");
    }

    /*
    if (!fs_init(SPI2_HOST)) {
        ESP_LOGE(TAG, "sdcard error");
    }
    */
}

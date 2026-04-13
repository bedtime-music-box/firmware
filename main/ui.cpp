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

#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"

#include "sdkconfig.h"

#include "screens/boot.h"
#include "ui.h"
#include "util/lvgllocker.h"

static const char *TAG = "ui";

static lv_disp_t *disp;

static lv_obj_t *screen_boot;

bool ui_init(spi_host_device_t host_id)
{
    // Initialize the LCD panel
    esp_lcd_panel_io_handle_t io_handle;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = CONFIG_PIN_LCD_CS,
        .dc_gpio_num = CONFIG_PIN_LCD_DC,
        .pclk_hz = 40'000'000,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
#pragma GCC diagnostic pop
    if (esp_lcd_new_panel_io_spi(
            (esp_lcd_spi_bus_handle_t)host_id,
            &io_config,
            &io_handle
        ) != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_panel_io_spi() failed");
        return false;
    }

    // Initialize the ST7789 driver
    esp_lcd_panel_handle_t panel_handle;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    esp_lcd_panel_dev_config_t panel_dev_config = {
        .reset_gpio_num = CONFIG_PIN_PANEL_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };
#pragma GCC diagnostic pop
    if (esp_lcd_new_panel_st7789(io_handle, &panel_dev_config, &panel_handle) !=
        ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_panel_st7789() failed");
        return false;
    }

    // Setup the display
    if (esp_lcd_panel_reset(panel_handle) != ESP_OK ||
        esp_lcd_panel_init(panel_handle) != ESP_OK ||
        esp_lcd_panel_disp_on_off(panel_handle, true) != ESP_OK ||
        esp_lcd_panel_invert_color(panel_handle, true) != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_*() functions failed");
        return false;
    }

    // Initialize the GPIO pin used for the backlight
    if (gpio_set_direction((gpio_num_t)CONFIG_PIN_PANEL_BL, GPIO_MODE_OUTPUT) !=
            ESP_OK ||
        gpio_set_level((gpio_num_t)CONFIG_PIN_PANEL_BL, 1) != ESP_OK) {
        ESP_LOGE(TAG, "gpio_*() functions failed");
        return false;
    }

    // Initialize the LVGL port configuration
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    if (lvgl_port_init(&lvgl_cfg) != ESP_OK) {
        ESP_LOGE(TAG, "lvgl_port_init() failed");
        return false;
    }

    // Create the display
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = CONFIG_LCD_H_RES * 40,
        .hres = CONFIG_LCD_V_RES,
        .vres = CONFIG_LCD_H_RES,
        .rotation =
            {
                .swap_xy = true,
                .mirror_y = true,
            },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {
            .buff_dma = true,
            .swap_bytes = true,
        },
    };
#pragma GCC diagnostic pop
    disp = lvgl_port_add_disp(&disp_cfg);
    if (disp == nullptr) {
        ESP_LOGE(TAG, "lvgl_port_add_disp() failed");
        return false;
    }

    // Initialize the default theme
    auto theme = lv_theme_default_init(
        disp,
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_RED),
        false,
        LV_FONT_DEFAULT
    );
    lv_disp_set_theme(disp, theme);

    // Create the screens
    screen_boot = screen_boot_create(disp);

    // Set the boot screen active
    {
        LVGLLocker();
        lv_disp_load_scr(screen_boot);
    }

    // Everything was successful
    return true;
}

void ui_set_error(const char *message)
{
    //...
}

void ui_send_button(int which)
{
    //...
}

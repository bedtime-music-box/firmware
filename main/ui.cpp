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

#include "ui.h"

#include "macros.h"
#include "sdkconfig.h"

UI::UI(spi_host_device_t host_id)
    : m_host_id(host_id)
{
}

UI::Error UI::Initialize()
{
    if (!InitializeLCD()) {
        return PanelIO;
    }

    if (!InitializeST7789()) {
        return PanelST7789;
    }

    if (!InitializeDisplay()) {
        return Display;
    }

    if (!InitializeGPIO()) {
        return GPIO;
    }

    if (!InitializeLVGL()) {
        return LVGL;
    }

    // TODO: this is currently for testing
    if (lvgl_port_lock(0)) {
        lv_obj_t *screen = lv_disp_get_scr_act(m_disp);

        lv_obj_t *label = lv_label_create(screen);
        lv_label_set_text(label, "Hello World!");
        lv_obj_set_style_text_font(label, LV_FONT_DEFAULT, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

        lvgl_port_unlock();
    }

    return None;
}

bool UI::InitializeLCD()
{
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = CONFIG_PIN_LCD_CS,
        .dc_gpio_num = CONFIG_PIN_LCD_DC,
        .spi_mode = 0,
        .pclk_hz = 5'000'000, // TODO: turn this up after soldering
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    ENSURE(esp_lcd_new_panel_io_spi(
        (esp_lcd_spi_bus_handle_t)m_host_id,
        &io_config,
        &m_io_handle
    ))
    return true;
}

bool UI::InitializeST7789()
{
    esp_lcd_panel_dev_config_t panel_dev_config = {
        .reset_gpio_num = CONFIG_PIN_PANEL_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };
    ENSURE(esp_lcd_new_panel_st7789(
        m_io_handle,
        &panel_dev_config,
        &m_panel_handle
    ))
    return true;
}

bool UI::InitializeDisplay()
{
    ENSURE(esp_lcd_panel_reset(m_panel_handle))
    ENSURE(esp_lcd_panel_init(m_panel_handle))
    ENSURE(esp_lcd_panel_disp_on_off(m_panel_handle, true))
    ENSURE(esp_lcd_panel_invert_color(m_panel_handle, true))
    return true;
}

bool UI::InitializeGPIO()
{
    ENSURE(
        gpio_set_direction((gpio_num_t)CONFIG_PIN_PANEL_BL, GPIO_MODE_OUTPUT)
    )
    ENSURE(gpio_set_level((gpio_num_t)CONFIG_PIN_PANEL_BL, 1))
    return true;
}

bool UI::InitializeLVGL()
{
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ENSURE(lvgl_port_init(&lvgl_cfg))

    lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = m_io_handle,
        .panel_handle = m_panel_handle,
        .buffer_size = CONFIG_LCD_H_RES * 40,
        .hres = CONFIG_LCD_H_RES,
        .vres = CONFIG_LCD_V_RES,
        .flags = {
            .buff_dma = true,
        },
    };

    m_disp = lvgl_port_add_disp(&disp_cfg);
    return m_disp != nullptr;
}

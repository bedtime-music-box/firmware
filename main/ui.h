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

#ifndef UI_H
#define UI_H

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"

/**
 * Initialize and manage the ST7789 panel and user interface
 *
 * It is expected that the provided SPI bus is already initialized.
 */
class UI
{
public:

    /**
     * Indicate what stage of initialization caused an error
     */
    enum Error {
        None = 0,
        PanelIO,
        PanelST7789,
        Display,
        GPIO,
        LVGL,
    };

    UI(spi_host_device_t);

    /**
     * Initialize the panel
     *
     * This must be called before any other methods.
     *
     * @return error category upon failure
     */
    Error Initialize();

    /**
     * Control power to the display
     */
    bool SetPower(bool);

    /**
     * Control power to the backlight
     */
    bool SetBacklight(bool);

private:

    bool InitializeLCD();
    bool InitializeST7789();
    bool InitializeDisplay();
    bool InitializeGPIO();
    bool InitializeLVGL();

    spi_host_device_t m_host_id;
    esp_lcd_panel_io_handle_t m_io_handle;
    esp_lcd_panel_handle_t m_panel_handle;
    lv_disp_t *m_disp;
};

#endif // UI_H

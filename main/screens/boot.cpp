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

#include "esp_lvgl_port.h"

#include "util/lvgllocker.h"

#include "sdkconfig.h"

#include "boot.h"

extern const lv_image_dsc_t splash;

lv_obj_t *screen_boot_create(lv_disp_t *disp)
{
    LVGLLocker();

    // Create the screen with a white background
    lv_obj_t *screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);

    // Load the splash image
    lv_obj_t *img = lv_image_create(screen);
    lv_image_set_src(img, &splash);
    lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // Show the title at the top
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Bedtime Music Box");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_30, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Show the author underneath
    lv_obj_t *author = lv_label_create(screen);
    lv_label_set_text(author, "by Nathan Osman");
    lv_obj_align(author, LV_ALIGN_TOP_MID, 0, 50);

    // Show current firmware version:
    char buf[256];
    snprintf(buf, sizeof(buf), "Booting with\nfirmware\nv%s", "1.0.0");
    lv_obj_t *firmware = lv_label_create(screen);
    lv_label_set_text(firmware, buf);
    lv_obj_set_style_text_align(firmware, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align(firmware, LV_ALIGN_TOP_RIGHT, -10, 100);

    // Indicate loading
    lv_obj_t *spinner = lv_spinner_create(screen);
    lv_obj_set_size(spinner, 40, 40);
    lv_obj_align(spinner, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

    return screen;
}

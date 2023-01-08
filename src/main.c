/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 dragonkomat (komat.jp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

// SPDX-License-Identifier: MIT

#include <string.h>
#include "hardware/gpio.h"
//#include "pico/stdio_uart.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "usb_main.h"
#include "button.h"
#include "reverser.h"
#include "footcon.h"
#include "mascon.h"
#include "main.h"

static void main2(void);

repeating_timer_t g_rt;
states_t g_states;

static bool main_repeating_timer_cb(repeating_timer_t *rt) {

    if( !g_states.ready ) return true;

    uint32_t gpio = gpio_get_all();

    mascon_task(gpio);
    button_task(gpio);
    reverser_task(gpio);
    footcon_task();
    usb_report_task();

    return true;
}

void main_init() {
    memset(&g_states,0x00,sizeof(g_states));

    add_repeating_timer_ms(10, main_repeating_timer_cb, NULL, &g_rt);
}

int main() {
    //stdio_uart_init();
    main_init();
    usb_init();
    mascon_init();
    button_init();
    reverser_init();
    footcon_init();

    multicore_launch_core1(main2);

    while (true) {
        usb_task();
        if( !g_states.ready && tud_hid_ready() ) {
            g_states.ready = true;
        }
    }
    
    return 0;
}

static void main2() {
    while (true) {
        if( g_states.ready ) break;
    }

    while (true) {
        footcon_capture_task();
    }
}
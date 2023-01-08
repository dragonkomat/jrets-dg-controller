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

#include "hardware/gpio.h"

#include "main.h"
#include "button.h"

#define JRETS_GPIO_ATS_CONFIRM              16
#define JRETS_GPIO_ATS_PERSIST              18
#define JRETS_GPIO_ATS_RECOVER_NORMAL       19
#define JRETS_GPIO_ATS_RECOVER_EMERGENCY    20
#define JRETS_GPIO_EB_CONFIRM               7
#define JRETS_GPIO_BUZZER                   17
#define JRETS_GPIO_SUPPRESSION_1            3
#define JRETS_GPIO_SUPPRESSION_2            4
#define JRETS_GPIO_PAUSE                    6
#define JRETS_GPIO_GRADIENT_LAUNCH          2

inline void _init_gpio(uint gpio) {
    gpio_init(gpio);
    gpio_set_oeover(gpio, GPIO_OVERRIDE_LOW);
    gpio_set_pulls(gpio, true, false);
}

void button_init(void) {
    _init_gpio(JRETS_GPIO_ATS_CONFIRM);
    _init_gpio(JRETS_GPIO_ATS_PERSIST);
    _init_gpio(JRETS_GPIO_ATS_RECOVER_NORMAL);
    _init_gpio(JRETS_GPIO_ATS_RECOVER_EMERGENCY);
    _init_gpio(JRETS_GPIO_EB_CONFIRM);
    _init_gpio(JRETS_GPIO_BUZZER);
    _init_gpio(JRETS_GPIO_SUPPRESSION_1);
    _init_gpio(JRETS_GPIO_SUPPRESSION_2);
    _init_gpio(JRETS_GPIO_PAUSE);
    _init_gpio(JRETS_GPIO_GRADIENT_LAUNCH);
}

void button_task(uint32_t gpio) {
    g_states.keys[JRETS_KEY_ATS_CONFIRM]            = ((gpio & (1U << JRETS_GPIO_ATS_CONFIRM)) == 0);
    g_states.keys[JRETS_KEY_ATS_PERSIST]            = ((gpio & (1U << JRETS_GPIO_ATS_PERSIST)) == 0);
    g_states.keys[JRETS_KEY_ATS_RECOVER_NORMAL]     = ((gpio & (1U << JRETS_GPIO_ATS_RECOVER_NORMAL)) == 0);
    g_states.keys[JRETS_KEY_ATS_RECOVER_EMERGENCY]  = ((gpio & (1U << JRETS_GPIO_ATS_RECOVER_EMERGENCY)) == 0);
    g_states.keys[JRETS_KEY_EB_CONFIRM]             = ((gpio & (1U << JRETS_GPIO_EB_CONFIRM)) == 0);
    g_states.keys[JRETS_KEY_BUZZER]                 = ((gpio & (1U << JRETS_GPIO_BUZZER)) == 0);
    g_states.keys[JRETS_KEY_SUPPRESSION_1]          = ((gpio & (1U << JRETS_GPIO_SUPPRESSION_1)) == 0);
    g_states.keys[JRETS_KEY_SUPPRESSION_2]          = ((gpio & (1U << JRETS_GPIO_SUPPRESSION_2)) == 0);
    g_states.keys[JRETS_KEY_PAUSE]                  = ((gpio & (1U << JRETS_GPIO_PAUSE)) == 0);
    g_states.keys[JRETS_KEY_GRADIENT_LAUNCH]        = ((gpio & (1U << JRETS_GPIO_GRADIENT_LAUNCH)) == 0);
}
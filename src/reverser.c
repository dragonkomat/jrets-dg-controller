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
#include "reverser.h"

#define JRETS_GPIO_REVERSER_FORWARD     22
#define JRETS_GPIO_REVERSER_BACKWARD    21

#define JRETS_REVERSER_DEBOUNCE_COUNT   2

typedef enum {
    JRETS_REVERSER_OFF = 0,
    JRETS_REVERSER_FORWARD,
    JRETS_REVERSER_BACKWARD
} reverser_t;

reverser_t g_reverser_state;
int g_reverser_debounce;

inline void _init_gpio(uint gpio) {
    gpio_init(gpio);
    gpio_set_oeover(gpio, GPIO_OVERRIDE_LOW);
    gpio_set_pulls(gpio, true, false);
}

void reverser_init() {
    g_reverser_state = JRETS_REVERSER_OFF;
    g_reverser_debounce = 0;

    _init_gpio(JRETS_GPIO_REVERSER_FORWARD);
    _init_gpio(JRETS_GPIO_REVERSER_BACKWARD);
}

void reverser_task(uint32_t gpio) {

    bool forward = ((gpio & (1U << JRETS_GPIO_REVERSER_FORWARD)) == 0);
    bool backward = ((gpio & (1U << JRETS_GPIO_REVERSER_BACKWARD)) == 0);

    switch(g_reverser_state) {
        default:
        case JRETS_REVERSER_OFF:
            if( forward ) {
                g_states.keys[JRETS_KEY_REVERSER_FORWARD] = true;
                g_reverser_state = JRETS_REVERSER_FORWARD;
                g_reverser_debounce = JRETS_REVERSER_DEBOUNCE_COUNT;
            }
            else if( backward ) {
                g_states.keys[JRETS_KEY_REVERSER_BACKWARD] = true;
                g_reverser_state = JRETS_REVERSER_BACKWARD;
                g_reverser_debounce = JRETS_REVERSER_DEBOUNCE_COUNT;
            }
            break;
        case JRETS_REVERSER_FORWARD:
            g_states.keys[JRETS_KEY_REVERSER_FORWARD] = false;
            if( (g_reverser_debounce == 0) && !forward && !backward ) {
                g_reverser_state = JRETS_REVERSER_OFF;
            }
            break;
        case JRETS_REVERSER_BACKWARD:
            g_states.keys[JRETS_KEY_REVERSER_BACKWARD] = false;
            if( (g_reverser_debounce == 0) && !forward && !backward ) {
                g_reverser_state = JRETS_REVERSER_OFF;
            }
            break;
    }

    if( g_reverser_debounce > 0 ) {
        g_reverser_debounce--;
    }
}

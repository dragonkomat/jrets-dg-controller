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
#include "mascon.h"

#define JRETS_GPIO_MASCON   8   //< 8:A0～10:A2
#define JRETS_GPIO_BRAKE    12  //< 12:A0～15:A3

typedef enum {
    JRETS_MASCON_IDLE,
    JRETS_MASCON_MAKE,
    JRETS_MASCON_BREAK
} mascon_state_t;

int g_mascon_mascon_level;  //< -1:無効、0:切、1～5
int g_mascon_brake_level;   //< -1:無効、0:切、1～8、9:非常

mascon_state_t g_mascon_mascon_state;
mascon_state_t g_mascon_brake_state;

int mascon_get_mascon_level(uint32_t gpio) {
    uint32_t level = (gpio >> JRETS_GPIO_MASCON) & 0x07U;
    int ret;

    switch(level) {
        default:
        case 7:
        case 0:
            ret = -1;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = 1;
            break;
        case 3:
            ret = 2;
            break;
        case 4:
            ret = 3;
            break;
        case 5:
            ret = 4;
            break;
        case 6:
            ret = 5;
            break;
    }

    return ret;
}

int mascon_get_brake_level(uint32_t gpio) {
    uint32_t level = (gpio >> JRETS_GPIO_BRAKE) & 0x0FU;
    int ret;

    switch(level) {
        default:
        case 0:
            ret = -1;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = 1;
            break;
        case 3:
            ret = 2;
            break;
        case 4:
            ret = 3;
            break;
        case 5:
            ret = 4;
            break;
        case 6:
            ret = 5;
            break;
        case 7:
            ret = 6;
            break;
        case 8:
            ret = 7;
            break;
        case 9:
            ret = 8;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            ret = 9;
            break;
    }

    return ret;    
}

inline void _init_gpio(uint gpio) {
    gpio_init(gpio);
    gpio_set_oeover(gpio, GPIO_OVERRIDE_LOW);
}

void mascon_init(void) {
    g_mascon_mascon_level = -1;
    g_mascon_brake_level = -1;
    g_mascon_mascon_state = JRETS_MASCON_IDLE;
    g_mascon_brake_state = JRETS_MASCON_IDLE;

    _init_gpio(JRETS_GPIO_BRAKE);
    _init_gpio(JRETS_GPIO_BRAKE + 1);
    _init_gpio(JRETS_GPIO_BRAKE + 2);
    _init_gpio(JRETS_GPIO_BRAKE + 3);
    _init_gpio(JRETS_GPIO_MASCON);
    _init_gpio(JRETS_GPIO_MASCON + 1);
    _init_gpio(JRETS_GPIO_MASCON + 2);
}

void mascon_task(uint32_t gpio) {
    int mascon = mascon_get_mascon_level(gpio);

    if( mascon >= 0 ) {
        if( (g_mascon_mascon_level != mascon) && (g_mascon_mascon_state == JRETS_MASCON_IDLE) ) {
            g_mascon_mascon_state = JRETS_MASCON_MAKE;
            if( mascon == 0 ) {
                g_states.keys[JRETS_KEY_MASCON_FREE] = true;
            }
            else if( g_mascon_mascon_level > mascon ) {
                g_states.keys[JRETS_KEY_MASCON_DOWN] = true;
            }
            else {
                g_states.keys[JRETS_KEY_MASCON_UP] = true;
            }
            g_mascon_mascon_level = mascon;
        }
    }

    switch(g_mascon_mascon_state) {
        case JRETS_MASCON_IDLE:
            break;
        case JRETS_MASCON_MAKE:
            g_mascon_mascon_state = JRETS_MASCON_BREAK;
            break;
        default:
        case JRETS_MASCON_BREAK:
            g_mascon_mascon_state = JRETS_MASCON_IDLE;
            g_states.keys[JRETS_KEY_MASCON_FREE] = false;
            g_states.keys[JRETS_KEY_MASCON_UP] = false;
            g_states.keys[JRETS_KEY_MASCON_DOWN] = false;
            break;
    }

    int brake = mascon_get_brake_level(gpio);

    if( brake >= 0 ) {
        if( (g_mascon_brake_level != brake) && (g_mascon_brake_state == JRETS_MASCON_IDLE) ) {
            g_mascon_brake_state = JRETS_MASCON_MAKE;
            if( brake == 0 ) {
                g_states.keys[JRETS_KEY_BRAKE_FREE] = true;
            }
            else if( brake == 9 ) {
                g_states.keys[JRETS_KEY_BRAKE_EMERGENCY] = true;
            }
            else if( g_mascon_brake_level > brake ) {
                g_states.keys[JRETS_KEY_BRAKE_DOWN] = true;
            }
            else {
                g_states.keys[JRETS_KEY_BRAKE_UP] = true;
            }
            g_mascon_brake_level = brake;
        }
    }

    switch(g_mascon_brake_state) {
        case JRETS_MASCON_IDLE:
            break;
        case JRETS_MASCON_MAKE:
            g_mascon_brake_state = JRETS_MASCON_BREAK;
            break;
        default:
        case JRETS_MASCON_BREAK:
            g_mascon_brake_state = JRETS_MASCON_IDLE;
            g_states.keys[JRETS_KEY_BRAKE_FREE] = false;
            g_states.keys[JRETS_KEY_BRAKE_UP] = false;
            g_states.keys[JRETS_KEY_BRAKE_DOWN] = false;
            g_states.keys[JRETS_KEY_BRAKE_EMERGENCY] = false;
            break;
    }
}

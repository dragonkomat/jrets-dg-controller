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

#include "hardware/adc.h"

#include "main.h"
#include "footcon.h"

#define JRETS_GPIO_FOOTCON  26

#define JRETS_FOOTCON_TH_1ST_OFF    0x980
#define JRETS_FOOTCON_TH_1ST_ON     0xa00
#define JRETS_FOOTCON_TH_2ND_OFF    0xd00
#define JRETS_FOOTCON_TH_2ND_ON     0xd80
#define JRETS_FOOTCON_TH_OPEN       0xf80

typedef enum {
    JRETS_FOOTCON_STATE_OFF,
    JRETS_FOOTCON_STATE_1ST,
    JRETS_FOOTCON_STATE_2ND,
    JRETS_FOOTCON_STATE_OPEN
} footcon_state_t;

uint16_t g_footcon_value;
footcon_state_t g_footcon_state;

void footcon_init() {
    g_footcon_value = 0x0FFF;
    g_footcon_state = JRETS_FOOTCON_STATE_OPEN;

    adc_init();
    adc_gpio_init(JRETS_GPIO_FOOTCON);
    adc_select_input(JRETS_GPIO_FOOTCON-26);
}

void footcon_task() {

    uint16_t value = g_footcon_value;

    if( value >= JRETS_FOOTCON_TH_OPEN ) {
        g_footcon_state = JRETS_FOOTCON_STATE_OPEN;
    }
    else {
        switch( g_footcon_state ) {
            default:
            case JRETS_FOOTCON_STATE_OPEN:
            case JRETS_FOOTCON_STATE_OFF:
                if( value >= JRETS_FOOTCON_TH_1ST_ON ) {
                    g_footcon_state = JRETS_FOOTCON_STATE_1ST;
                }
                break;
            case JRETS_FOOTCON_STATE_1ST:
                if( value >= JRETS_FOOTCON_TH_2ND_ON ) {
                    g_footcon_state = JRETS_FOOTCON_STATE_2ND;
                }
                else if ( value < JRETS_FOOTCON_TH_1ST_OFF ) {
                    g_footcon_state = JRETS_FOOTCON_STATE_OFF;
                }
                break;
            case JRETS_FOOTCON_STATE_2ND:
                if ( value < JRETS_FOOTCON_TH_2ND_OFF ) {
                    g_footcon_state = JRETS_FOOTCON_STATE_1ST;
                }
                break;
        }
    }

    switch( g_footcon_state ) {
        default:
        case JRETS_FOOTCON_STATE_OFF:
        case JRETS_FOOTCON_STATE_OPEN:
            g_states.keys[JRETS_KEY_HORN_1] = false;
            g_states.keys[JRETS_KEY_HORN_2] = false;
            break;
        case JRETS_FOOTCON_STATE_1ST:
            g_states.keys[JRETS_KEY_HORN_1] = true;
            g_states.keys[JRETS_KEY_HORN_2] = false;
            break;
        case JRETS_FOOTCON_STATE_2ND:
            g_states.keys[JRETS_KEY_HORN_1] = true;
            g_states.keys[JRETS_KEY_HORN_2] = true;
            break;
    }
}

void footcon_capture_task() {
    g_footcon_value = adc_read();
}

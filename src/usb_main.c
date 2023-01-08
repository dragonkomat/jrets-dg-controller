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
#include "tusb.h"

#include "usb_descriptors.h"
#include "main.h"
#include "usb_main.h"

const uint8_t g_keycodes[] = {
    HID_KEY_Z,          // JRETS_KEY_MASCON_UP                  // マスコン：加速を強める
    HID_KEY_A,          // JRETS_KEY_MASCON_DOWN                // マスコン：加速を弱める
    HID_KEY_S,          // JRETS_KEY_MASCON_FREE                // マスコン：加速ゼロ
    HID_KEY_SLASH,      // JRETS_KEY_BRAKE_EMERGENCY            // ブレーキ：非常ブレーキ
    HID_KEY_PERIOD,     // JRETS_KEY_BRAKE_UP                   // ブレーキ：ブレーキを強める
    HID_KEY_COMMA,      // JRETS_KEY_BRAKE_DOWN                 // ブレーキ：ブレーキを弱める
    HID_KEY_M,          // JRETS_KEY_BRAKE_FREE                 // ブレーキ：ブレーキゼロ
    HID_KEY_SPACE,      // JRETS_KEY_ATS_CONFIRM                // ATS確認
    HID_KEY_X,          // JRETS_KEY_ATS_PERSIST                // 警報持続
    HID_KEY_Y,          // JRETS_KEY_ATS_RECOVER_NORMAL         // ATS復帰(常用)
    HID_KEY_U,          // JRETS_KEY_ATS_RECOVER_EMERGENCY      // ATS復帰(非常)
    HID_KEY_E,          // JRETS_KEY_EB_CONFIRM                 // EB確認
    HID_KEY_B,          // JRETS_KEY_BUZZER                     // 連絡ブザー
    HID_KEY_ARROW_UP,   // JRETS_KEY_REVERSER_FORWARD           // レバーサ：前
    HID_KEY_ARROW_DOWN, // JRETS_KEY_REVERSER_BACKWARD          // レバーサ：後
    HID_KEY_D,          // JRETS_KEY_SUPPRESSION_1              // 抑速１
    HID_KEY_W,          // JRETS_KEY_SUPPRESSION_2              // 抑速２・定速
    HID_KEY_ESCAPE,     // JRETS_KEY_PAUSE                      // ポーズ
    HID_KEY_ENTER,      // JRETS_KEY_HORN_1                     // 警笛：１段目
    HID_KEY_BACKSPACE,  // JRETS_KEY_HORN_2                     // 警笛：２段目
    HID_KEY_C,          // JRETS_KEY_VIEW                       // 運転台表示切替
    HID_KEY_V,          // JRETS_KEY_HUD                        // HUD表示・非表示
    HID_KEY_I,          // JRETS_KEY_INCHING                    // インチング
    HID_KEY_T,          // JRETS_KEY_TASC_OFF                   // TASC切
    HID_KEY_K           // JRETS_KEY_GRADIENT_LAUNCH            // 勾配起動
};

uint8_t g_keycode[6];

void usb_init() {
    memset(g_keycode,0x00,sizeof(g_keycode));
    tusb_init();
}

void usb_task() {
    tud_task();
}

void usb_report_task() {
    int i,j;
    uint8_t keycode[6] = { 0 };

    for(i=0,j=0; i<JRETS_KEY_COUNT; i++) {
        if( g_keycodes[i] && g_states.keys[i] ) {
            keycode[j++] = g_keycodes[i];
            if( j>=6 ) break;
        }
    }

    if(memcmp(g_keycode,keycode,sizeof(keycode)) != 0) {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        memcpy(g_keycode,keycode,sizeof(keycode));
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
}

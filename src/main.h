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

#ifndef _MAIN_H
#define _MAIN_H

#include <sys/types.h>
#include <stdbool.h>

enum {
    JRETS_KEY_MASCON_UP,                //< マスコン：加速を強める
    JRETS_KEY_MASCON_DOWN,              //< マスコン：加速を弱める
    JRETS_KEY_MASCON_FREE,              //< マスコン：加速ゼロ
    JRETS_KEY_BRAKE_EMERGENCY,          //< ブレーキ：非常ブレーキ
    JRETS_KEY_BRAKE_UP,                 //< ブレーキ：ブレーキを強める
    JRETS_KEY_BRAKE_DOWN,               //< ブレーキ：ブレーキを弱める
    JRETS_KEY_BRAKE_FREE,               //< ブレーキ：ブレーキゼロ
    JRETS_KEY_ATS_CONFIRM,              //< ATS確認
    JRETS_KEY_ATS_PERSIST,              //< 警報持続
    JRETS_KEY_ATS_RECOVER_NORMAL,       //< ATS復帰(常用)
    JRETS_KEY_ATS_RECOVER_EMERGENCY,    //< ATS復帰(非常)
    JRETS_KEY_EB_CONFIRM,               //< EB確認
    JRETS_KEY_BUZZER,                   //< 連絡ブザー
    JRETS_KEY_REVERSER_FORWARD,         //< レバーサ：前
    JRETS_KEY_REVERSER_BACKWARD,        //< レバーサ：後
    JRETS_KEY_SUPPRESSION_1,            //< 抑速１
    JRETS_KEY_SUPPRESSION_2,            //< 抑速２・定速
    JRETS_KEY_PAUSE,                    //< ポーズ
    JRETS_KEY_HORN_1,                   //< 警笛：１段目
    JRETS_KEY_HORN_2,                   //< 警笛：２段目
    JRETS_KEY_VIEW,                     //< 運転台表示切替
    JRETS_KEY_HUD,                      //< HUD表示・非表示
    JRETS_KEY_INCHING,                  //< インチング
    JRETS_KEY_TASC_OFF,                 //< TASC切
    JRETS_KEY_GRADIENT_LAUNCH,          //< 勾配起動

    JRETS_KEY_COUNT
};

typedef struct {
    volatile bool ready;
    bool keys[JRETS_KEY_COUNT];
} states_t;

extern states_t g_states;

#endif  // _MAIN_H
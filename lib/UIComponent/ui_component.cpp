/* ui_component.cpp - Definitions of UI components of the Embedded Sentry.
 * License-Identifier: MIT
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://mit-license.org
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ui_component.h"

// Definition of buttons.
ScreenButton reset_wide = {15, 15, 205, DEFAULT_HEIGHT, set_key_started};
ScreenButton reset_key = {15, 15, 95, DEFAULT_HEIGHT, set_key_started};
ScreenButton test = {125, 15, 95, DEFAULT_HEIGHT, test_key_started};

// ScreenButton cancel = {15, 15, 95, DEFAULT_HEIGHT, recording_cancelled};
ScreenButton finish = {15, 15, 205, DEFAULT_HEIGHT, recording_finished};


// The list of screen buttons used for each view.
ScreenButton *const idle_no_key_bts[IDLE_NO_KEY_LEN] = {
    &reset_wide
};
ScreenButton *const idle_default_bts[IDLE_DEFAULT_LEN] = {
    &reset_key,
    &test
};

ScreenButton *const recording_bts[RECORDING_LEN] = {
    // &cancel,
    &finish
};
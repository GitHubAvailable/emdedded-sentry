/* ui_component Library
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
#ifndef UI_Component_h
#define UI_Component_h

#include "interrupt.h"
#include "screenbutton.h"

#define DEFAULT_HEIGHT 50

// The length of each array defined.
#define IDLE_NO_KEY_LEN 1
#define IDLE_DEFAULT_LEN 2
#define RECORDING_LEN 1

// Definition of buttons.
// Idle start view with no key stored.
extern ScreenButton reset_wide;

// Default start view.
extern ScreenButton reset_key;
extern ScreenButton test;

// Recording view.
// extern ScreenButton cancel;
extern ScreenButton finish;


// The list of screen buttons used for each view.
/*!< buttons on the idle view with no key stored */
extern ScreenButton *const idle_no_key_bts[IDLE_NO_KEY_LEN];

/*!< buttons on the default idle view */
extern ScreenButton *const idle_default_bts[IDLE_DEFAULT_LEN];

/*!< buttons on the recording view */
extern ScreenButton *const recording_bts[RECORDING_LEN];

#endif
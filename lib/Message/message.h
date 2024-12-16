/* message Library
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
#ifndef Message_h
#define Message_h

#include <mbed.h>

const uint8_t DEV_STARTING[] = "Starting...";
const uint8_t APP_TITLE[] = "Embedded Sentry";
const uint8_t DEVICE_LOCKED[] = "Device Status: Locked";
const uint8_t DEVICE_UNLOCKED[] = "Device Status: Unlocked";
const uint8_t START_RECORD_HINT[] = "Click Button to Start Recording";

const uint8_t RECORDING[] = "Recording started.";
const uint8_t SET_KEY_STATUS[] = "Current Mode: Setting Key";
const uint8_t TEST_STATUS[] = "Current Mode: Unlocking";
const uint8_t RECORDING_HINT[] = "Please finish the sequence in 10s";

const uint8_t KEY_SAVED[] = "Key Saved";
const uint8_t INCORRECT_KEY[] = "Incorrect Gesture! Please try again!";
const uint8_t CORRECT_KEY[] = "Correct Gesture! Device unlocked!";

const uint8_t RESET_BUTTON[] = "Reset";
const uint8_t UNLOCK_BUTTON[] = "Unlock";
const uint8_t SUBMIT_BUTTON[] = "Submit";

#endif
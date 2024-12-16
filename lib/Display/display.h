/* display Library
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
#ifndef Display_h
#define Display_h

#include "prevstatus.h"

/*!
    @brief Turn off all leds on the board.
*/
void clear_led();

/*!
    @brief set initial UI of the device
    @details turn on the green LED
*/
void start_view();

/*!
    @brief A view that shows the device is ready to accept sequence.
    @param key_stored A bool indicating whether the answer key
        was stored on the device, the user is required to store
        a key sequence before testing.
*/
void device_ready_view(const PrevStatus &prev);

/*!
    @brief A view that shows the device is currently recording a sequence.
    @param mode A bool that shows if the current sequence is a test sequence.
*/
void recording_view(bool is_test);  // could add time later.

// /*!
//     @brief a view that shows an new key was stored successfully and the
//         device is ready.
// */
// void key_stored_view();

// /*!
//     @brief a view that shows the previous input matched the key and the
//         device is ready.
// */
// void seq_match_view();

// /*!
//     @brief a view that shows the previous input did not match the key
//         and the device is ready.
// */
// void seq_mismatch_view();

#endif
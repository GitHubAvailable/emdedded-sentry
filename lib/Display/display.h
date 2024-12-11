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

/*!
    @brief set initial UI of the device
    @details turn on the green LED
*/
void start_view();

/*!
    @brief a view that shows the device is ready to accept sequence.
*/
void device_ready_view();

/*!
    @brief a view that shows an new key was stored successfully and the
        device is ready.
*/
void key_stored_view();

/*!
    @brief a view that shows the previous input matched the key and the
        device is ready.
*/
void seq_match_view();

/*!
    @brief a view that shows the previous input did not match the key
        and the device is ready.
*/
void seq_mismatch_view();

void recording_view();

#endif
/* backend Library
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
#ifndef Backend_h
#define Backend_h

#include <mbed.h>
#include "buffer.h"
#include "dataloader.h"
#include "screenbutton.h"

#define GYRO_READ_PERIOD 4ms  // Issue a read every 4ms
#define MAX_AXIS_DISTANCE 0.1

/*!
    @brief reset the hardware to prepare for a new sequence
    @details reset the button to raise the `STARTRECORDING` flag when pressed
*/
void reset_device();

/*!
    @brief Wait for a button to be touched.
    @param The list of buttons to be detected.
    @note This function uses polling. And it blocks 
    @returns Pointer the first pressed pressed button in the array.
*/
ScreenButton *wait_buttons(ScreenButton *const *buttons, uint8_t len);

/*!
    @brief Setup periodic gyro-read signal.
*/
void set_periodic_read();

/*!
    @brief End periodic gyro-read signal.
*/
void end_periodic_read();

/*!
    @brief evaluate two gesture sequence match each other
    @details compare sequence of each axis by Dynamic Time Warping (DTW)
        and then evaluate the result based on if the worst axis
        meets the threshold or not
    @returns bool - if the two sequence match each other
*/
bool test_match(const DataBuffer &, const DataBuffer &);

#endif
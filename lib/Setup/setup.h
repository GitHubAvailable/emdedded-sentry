/* setup Library
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
#ifndef Setup_h
#define Setup_h

#include <mbed.h>

/*!
    @brief set default configuration of GPIO
    @note should be called with IRQ disabled
*/
void setup_gpio();

/*!
    @brief set default configuration of I3G4250D gyroscope
    @note should be called with IRQ disabled
*/
void setup_gyro();

#endif
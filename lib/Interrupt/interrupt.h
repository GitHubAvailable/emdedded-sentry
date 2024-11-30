/* interrupt Library
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
#ifndef Interrput_h
#define Interrput_h

#include <mbed.h>

#define START_RECORDING 1

// The set of flags that signal each event.
EventFlags flags;

/*!
    @brief Signal the program to start recording a sequence.
    @note This function should only be called in interrupt context.
*/
void start_recording();

#endif
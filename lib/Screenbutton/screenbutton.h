/* screenbutton Library
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
#ifndef ScreenButton_h
#define ScreenButton_h

#include <mbed.h>

/*!
    @brief A button with binded action that could be displayed on the
        screen.
*/
struct ScreenButton
{
    uint16_t x_pos;  /*!< x-coordinate of the lower-left corner */
    uint16_t y_pos;  /*!< y-coordinate of the lower-left corner */
    uint16_t width;  /*!< width of the button */
    uint16_t height;  /*!< height of the button */

    void (*action)(void);  /*!< the action to be called when touched */
};

#endif
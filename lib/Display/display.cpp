/* display.cpp - Implementation of the UI display functions for
 * the Embedded Sentry.
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
#include "hardware.h"
#include "display.h"

void start_view()
{
    green_led.write(ON);
    lcd.DisplayStringAt(0, LINE(10),
        (uint8_t *) "Sentry Starting...", CENTER_MODE);
}

void device_ready_view()
{
    lcd.Clear(DEFAULT_BG);
    lcd.DisplayStringAt(0, LINE(3),
        (uint8_t *) "Embedded Sentry", CENTER_MODE);
    lcd.DisplayStringAt(2, LINE(5),
        (uint8_t *) "Device is ready! You can:", CENTER_MODE);
    lcd.DisplayStringAt(5, LINE(6),
        (uint8_t *) "1. Click the button to start a test;", CENTER_MODE);
    lcd.DisplayStringAt(5, LINE(7),
        (uint8_t *) "2. Double click the button to reset the key", CENTER_MODE);
}

void recording_view()
{
    lcd.Clear(DEFAULT_BG);

    /* Display text. */
}
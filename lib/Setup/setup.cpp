/* setup.cpp - Implementation of the setup functions for the Embedded Sentry.
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
#include "interrupt.h"

void setup_gpio()
{
    green_led.write(OFF);
    red_led.write(OFF);

    // Signal the sentry to start recording when button pressed.
    button.rise(&start_recording);
}

void setup_gyro()
{
    
}
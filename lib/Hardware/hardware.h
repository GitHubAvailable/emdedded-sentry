/* hardware Library
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
#ifndef Hardware_h
#define Hardware_h

#include <mbed.h>
#include <LCD_DISCO_F429ZI.h>

// GPIO macros.
#define BUTTON PA_0  // user button
#define GREEN_LED PG_13
#define RED_LED PG_14

#define OFF 0
#define ON 1

// SPI macros.
#define MISO PF_9
#define MOSI PF_8
#define SCK PF_7
#define GYRO_SS PC_1  // the SS line of the gyroscope

// LCD default color settings.
#define DEFAULT_BG LCD_COLOR_BLACK
#define TEXT_DEFAULT_BG LCD_COLOR_BLACK  // backgroud color of texts
#define TEXT_DEFAULT_COLOR LCD_COLOR_GREEN

// The control button of the application.
InterruptIn button(BUTTON, PullDown);

// The green LED on the board.
DigitalOut green_led(GREEN_LED);

// The red LED on the board.
DigitalOut red_led(RED_LED);

// The SPI master to communicate with I3G4250D gyroscope.
SPI gyro_spi(MISO, MOSI, SCK, GYRO_SS, use_gpio_ssel);

// The ILI9341 LCD screen.
LCD_DISCO_F429ZI lcd;

#endif
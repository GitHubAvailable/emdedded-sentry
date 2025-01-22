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

// For some reason including these two can fix compiling errors.
#include <LCD_DISCO_F429ZI.h>
#include <TS_DISCO_F429ZI.h>

#define GYRO_SPI_COM_FLAG 0x2  // SPI communication complete
#define GYRO_DATA_READY_FLAG 0x4  // Gyro data ready
#define READ_DATA_FLAG 0x8  // Time to read from gyro
#define START_SET_KEY_FLAG 0x10  // Enter set key recording.
#define START_TEST_FLAG 0x20  // Enter test key recording.
#define RECORDING_CANCELLED_FLAG 0x40  // Current recording cancelled.
#define RECORDING_FINISHED_FLAG 0x80  // Recording finished.

// The set of flags that signal each event.
extern EventFlags flags;

// /*!
//     @brief Signal the program to start recording a sequence.
//     @note This function should only be called in interrupt context.
// */
// void start_recording();

/*!
    @brief Signal the program that an SPI communication is complete.
    @note This function should only be called in interrupt context.
*/
void com_finish(int event);

/*!
    @brief Signal the program that a new gyroscope data is ready.
    @note This function should only be called in interrupt context.
*/
void data_ready();

/*!
    @brief Signal the program to read data from gyroscope.
    @note This function should only be called in interrupt context.
*/
void read_gyro();

/*!
    @brief Signal the program to start the recording a new key.
*/
void set_key_started();

/*!
    @brief Signal the program to start recording a test sequence.
*/
void test_key_started();

/*!
    @brief Signal the program that the abort the current recording.
*/
void recording_cancelled();

/*!
    @brief Signal the program that the recording has finished.
*/
void recording_finished();

#endif
/* interrupt.cpp - Implementation of the interrupt handlers for
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
#include "interrupt.h"

EventFlags flags;

void com_finish(int event)
{
    flags.set(GYRO_SPI_COM_FLAG);
}

void data_ready()
{
    flags.set(GYRO_DATA_READY_FLAG);
}

void read_gyro()
{
    flags.set(READ_DATA_FLAG);
}

void set_key_started()
{
    flags.set(START_SET_KEY_FLAG);
}

void test_key_started()
{
    flags.set(START_TEST_FLAG);
}

void recording_cancelled()
{
    flags.set(RECORDING_CANCELLED_FLAG);
}

void recording_finished()
{
    flags.set(RECORDING_FINISHED_FLAG);
}
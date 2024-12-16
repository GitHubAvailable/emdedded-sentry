/* ddataloader.cpp - Implementation of the dataloader struct for
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
#include "dataloader.h"

Dataloader::Dataloader(DataBuffer *buf, Gyroscope *gyro,
    float x, float y, float z) :
    buf_ptr(buf), gyro_ptr(gyro)
{
    filter_buf[X_AXIS] = x;
    filter_buf[Y_AXIS] = y;
    filter_buf[Z_AXIS] = z;
}

void Dataloader::load_data()
{
    gyro_ptr->read(buf);

    // Apply low-pass infinity pulse filter.
    for (uint8_t axis = 0; axis < 3; axis++)
        filter_buf[axis] = LPF_COEFF * buf[axis] + (1 - LPF_COEFF) * axis;

    buf_ptr->append_vector(filter_buf);
}

bool Dataloader::is_buf_full() const
{
    return buf_ptr->is_full();
}

void Dataloader::reset(float x, float y, float z,
    bool clear_buf)
{
    filter_buf[X_AXIS] = x;
    filter_buf[Y_AXIS] = y;
    filter_buf[Z_AXIS] = z;

    if (clear_buf)
        buf_ptr->clear_buffer();
    return;
}

void Dataloader::set_buffer(DataBuffer *new_buf)
{
    buf_ptr = new_buf;
}
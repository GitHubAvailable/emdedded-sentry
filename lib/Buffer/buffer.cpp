/* buffer.cpp - Implementation of the buffer functions for
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
#include "buffer.h"

void DataBuffer::append_vector(float x, float y, float z)
{
    ang_v[X_AXIS][index] = x;
    ang_v[Y_AXIS][index] = y;
    ang_v[Z_AXIS][index] = z;
    
    if (++index >= BUFFER_LEN)  // reset counter if buffer is full.
        index = 0;
    return;
}

void DataBuffer::append_vector(const float *buf)
{
    for (uint8_t axis = 0; axis < TOTAL_DIM; axis++)
        ang_v[axis][index] = buf[axis];
    
    if (++index >= BUFFER_LEN)  // reset counter if buffer is full.
        index = 0;
    return;
}

const float *DataBuffer::get_axis(uint8_t axis) const
{
    if (axis >= TOTAL_DIM)  // invalid axis
        return NULL;
    
    return (const float *) ang_v[axis];
}

DataBuffer &DataBuffer::operator=(const DataBuffer &buffer)
{
    index = buffer.index;

    for (uint8_t axis = 0; axis < TOTAL_DIM; axis++)
    {
        for (uint j = 0; j < BUFFER_LEN; j++)
            ang_v[axis][j] = buffer.ang_v[axis][j];
    }
    return (DataBuffer &) *this;
}
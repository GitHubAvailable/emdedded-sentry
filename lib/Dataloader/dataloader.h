/* dataloader Library
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
#ifndef Dataloader_h
#define Dataloader_h

#include "buffer.h"
#include "hardware.h"

#define LPF_COEFF 0.2f

struct Dataloader
{
    private:
        float buf[3];  /*!< the buffer to store the incoming data */
        float filter_buf[3];  /*!< the filtered data of 3-axis */

        DataBuffer *buf_ptr;  /*!< the buffer to store the data */
        Gyroscope *gyro_ptr;  /*!< the gyro used to read the data */
    
    public:
        /*!
            @brief Construct a dataloader bind to the provided buffer
                and gyroscope.
            @param buf pointr to the DataBuffer used to store the
                gyroscope data.
            @param gyro pointer to the gyroscope used to receive data.
            @param x the initial filter value on x-axis, default is `0`.
            @param y the initial filter value on y-axis, default is `0`.
            @param z the initial filter value on z-axis, default is `0`.
        */
        Dataloader(DataBuffer *, Gyroscope *,
            float = 0.0f, float = 0.0f, float = 0.0f);

        /*!
            @brief Read and store data to the buffer until it is full.
        */
        void load_data();

        /*!
            @brief Return if the attached buffer is full.
        */
        bool is_buf_full() const;

        /*!
            @brief Reset the dataloader and optionally clear
                the DataBuffer.
            @param x The initial filter value on x-axis, default is `0`.
            @param y The initial filter value on y-axis, default is `0`.
            @param z The initial filter value on z-axis, default is `0`.
            @param clear_buf Whether the buffer should be cleared.
        */
        void reset(float = 0.0f, float = 0.0f, float = 0.0f, bool = true);

        /*!
            @brief Change the associated databuffer.
            @param buf The new buffer where data will be stored.
            @warning Passing pointer to non-existent buffer could lead
                to unexpected result.
        */
        void set_buffer(DataBuffer *);
};

#endif
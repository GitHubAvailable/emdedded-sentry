/* buffer Library
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
#ifndef Buffer_h
#define Buffer_h

#include <mbed.h>

#define TOTAL_DIM 3  // total number of dimensions

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define BUFFER_LEN 200

/*!
    @brief A cicurlar buffer to store the angular data of 3-axis.
*/
struct DataBuffer
{
    private:
        uint8_t index = 0;  /*!< index of the next element */
        float ang_v[3][BUFFER_LEN];  /*!< xyz-axis angular velocity data */
    
    public:
        /*!
            @brief Append one new angular velocity vector to the end.
            @note index will increment after each appending
                and reset to `0` if buffer is full.
        */
        void append_vector(float x, float y, float z);

        /*!
            @brief Append one new angular velocity vector to the end.
            @warning The provided buffer must have at least length 3,
                or could lead to unexpected behavior.
        */
        void append_vector(const float *buf);

        /*!
            @brief Check if the buffer is full.
        */
        inline bool is_full() const { return index == BUFFER_LEN; }

        /*!
            @brief Return index position of the buffer.
        */
        inline uint8_t get_index() const { return index; }
        
        /*!
            @brief Clear the data buffer by resetting the pointer.
            @note This function only reset the buffer index to `0`.
        */
        inline void clear_buffer() { index = 0; }

        /*!
            @brief Get the array to the data for a specific axis.
        */
        const float *get_axis(uint8_t axis) const;

        /*!
            @brief Copy the buffer by value.
        */
        DataBuffer &operator=(const DataBuffer &buffer);
};

#endif
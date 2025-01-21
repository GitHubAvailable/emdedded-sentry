/* backend.cpp - Implementation of backend logic of the Embedded Sentry.
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
#include "backend.h"

#define IS_IN_CLOSED_RANGE(x, l, u) ((x >= l) && (x <= u))

ScreenButton *wait_buttons(ScreenButton *const *buttons, uint8_t len)
{
    TS_StateTypeDef ts_state;

    while (1)
    {
        ts.GetState(&ts_state);

        if (!ts_state.TouchDetected)
            continue;
        
        // Check if the touch falls on a button.
        for (uint8_t i = 0; i < len; i++)
        {
            if (!IS_IN_CLOSED_RANGE(ts_state.X, buttons[i]->x_pos,
                buttons[i]->x_pos + buttons[i]->width))
                continue;
            if (!IS_IN_CLOSED_RANGE(ts_state.Y, buttons[i]->y_pos,
                buttons[i]->y_pos + buttons[i]->height))
                continue;
            return buttons[i];
        }
    }
}

void set_periodic_read()
{
    gyro_read_ticker.attach(read_gyro, GYRO_READ_PERIOD);
}

void end_periodic_read()
{
    gyro_read_ticker.detach();
}

/*!
    @brief use DTF to compare if two gesture array match each other
*/
float cmp_seqs(const float *const seq1, const uint16_t len1,
    const float *const seq2, const uint16_t len2)
{
    if ((!len1) || (!len2))  // one sequence is empty
        return -1.0f;
    
    float costs[2][BUFFER_LEN];
    costs[0][0] = abs(seq1[0] - seq2[0]);

    // Setup edge entries.
    costs[1][0] = costs[0][0] + abs(seq1[1] - seq2[0]);
    for (uint16_t c = 1; c < len2; c++)
        costs[0][c] = costs[0][c - 1] + abs(seq1[0] - seq2[c]);
    
    // Evaluate cost matrix.
    for (uint16_t r = 1; r < len1; r++)
    {
        for (uint16_t c = 1; c < len2; c++)
        {
            // Replace % 2 with & 1 to save time.
            costs[r & 1][c] = abs(seq1[r & 1] - seq2[c])
                + min(costs[(r - 1) & 1][c - 1],
                    min(costs[(r - 1) & 1][c], costs[r & 1][c - 1])
                );
        }
    }
    
    return costs[(len1 - 1) & 1][len2 - 1];
}

bool test_match(const DataBuffer &key1, const DataBuffer &key2)
{
    // Cache the length of input arrays.
    uint16_t len1, len2;
    len1 = key1.get_index();
    len2 = key2.get_index();

    // Calculate distance for each axis.
    float dist, max_dist = 0.0f;
    for (uint8_t axis = 0; axis < TOTAL_DIM; axis++)
    {
        dist = cmp_seqs(key1.get_axis(axis), len1,
            key2.get_axis(axis), len2);
        max_dist = max(dist, max_dist);
    }

    return max_dist < MAX_AXIS_DISTANCE;
}
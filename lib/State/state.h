/* state Library
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

#ifndef State_h
#define State_h

/*!
    @brief the possible states of the Embedded Sentry.
*/
enum State
{
    IDLE,  /*!< waiting for user to start a new recording */
    RECORDING,  /*!< recording a sequence */
    PROCESSING  /*!< precessing the sequence */
};

#endif
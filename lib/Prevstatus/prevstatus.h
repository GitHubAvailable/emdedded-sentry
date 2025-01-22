/* prevstatus Library
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
#ifndef PrevStatus_h
#define PrevStatus_h

/*!
    @brief All possible previous status of a device entering idle state.
*/
enum PrevStatus
{
    START,  /*!< the device just started */
    SET_KEY,  /*!< return after creating a key */
    MATCH,  /*!< return from a test match */
    MISMATCH  /*!< return from a test mismatch */
};

#endif
/* display.cpp - Implementation of the UI display functions for
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
#include "hardware.h"
#include "display.h"
#include "message.h"
#include "ui_component.h"

void clear_led()
{
    green_led.write(OFF);
    red_led.write(OFF);
}

void start_view()
{
    green_led.write(ON);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *) APP_TITLE, CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *) DEV_STARTING, CENTER_MODE);
}

/*!
    @brief Draw buttons on the screen.
    @param buttons an array of pointers to pre-defined screen buttons.
    @param len the length of the array of buttons.
*/
void draw_buttons(ScreenButton *const *buttons, uint8_t len)
{
    ScreenButton *cur_button;  // Cache the current button to be drawn.
    for (uint8_t i = 0; i < len; i++)
    {
        cur_button = buttons[i];

        // Flip y-axis to be consistent with the texts
        lcd.DrawRect(cur_button->x_pos,
            HEIGHT - cur_button->y_pos - cur_button->height,
            cur_button->width, cur_button->height);
    }
}

void device_ready_view(const PrevStatus &prev)
{
    lcd.Clear(DEFAULT_BG);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *) APP_TITLE, CENTER_MODE);

    uint8_t *prev_status_msg;

    switch (prev)
    {
        case START:
            // Turn on green LED to indicate device ready.
            green_led.write(ON);
            lcd.DisplayStringAt(0, LINE(17),
                (uint8_t *) RESET_BUTTON, CENTER_MODE);
            draw_buttons(idle_no_key_bts, IDLE_NO_KEY_LEN);
            return;
        case SET_KEY:
            prev_status_msg = (uint8_t *) KEY_SAVED;
            break;
        case MATCH:
            prev_status_msg = (uint8_t *) CORRECT_KEY;
            break;
        case MISMATCH:
            prev_status_msg = (uint8_t *) INCORRECT_KEY;
            break;
        default: break;
    }

    // Display action result, if exists.
    if (prev == MISMATCH)
        red_led.write(ON);
    else
    {
        green_led.write(ON);
        // Ensure red led turned off as we may arrive here from blinking.
        red_led.write(OFF);
    }
    if (prev != START)  // note that prev_status_msg is set if get here
        lcd.DisplayStringAt(0, LINE(5), prev_status_msg, CENTER_MODE);

    // Use LEFT_MODE since max text width cannot be specified.
    lcd.DisplayStringAt(60, LINE(17),
        (uint8_t *) RESET_BUTTON, LEFT_MODE);
    lcd.DisplayStringAt(180, LINE(17),
        (uint8_t *) UNLOCK_BUTTON, LEFT_MODE);
    draw_buttons(idle_default_bts, IDLE_DEFAULT_LEN);
}

void recording_view(bool is_test)
{
    lcd.Clear(DEFAULT_BG);

    lcd.DisplayStringAt(0, LINE(3), (uint8_t *) RECORDING, CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(7), (uint8_t *) RECORDING_HINT, CENTER_MODE);

    // Place button labels.
    lcd.DisplayStringAt(120, LINE(18), (uint8_t *) SUBMIT_BUTTON, CENTER_MODE);
    draw_buttons(recording_bts, RECORDING_LEN);

    if (is_test)
    {
        red_led.write(ON);
        return;
    }
    green_led.write(ON);
}

// void processing_view()
// {
//     lcd.DisplayStringAt(0, LINE(3), (uint8_t *) )
// }
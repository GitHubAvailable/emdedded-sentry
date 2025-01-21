/*!
    @authors
        @author brye11
        @author GitHubAvailable
        @author DotBion
*/
#include <mbed.h>
#include <LCD_DISCO_F429ZI.h>
#include <TS_DISCO_F429ZI.h>

#include "backend.h"
#include "display.h"
#include "dataloader.h"
#include "interrupt.h"
#include "setup.h"
#include "state.h"
#include "ui_component.h"

#define WAIT_FOREVER 0xFFFFFFFF


// Parameters for `detect_touch`.
ScreenButton *const *cur_bt_group;  /*!< the button group to be monitored */
uint8_t group_len;  /*!< length of the button group */

// Parameters for `loader_perodic_read`.
Dataloader *target_loader;

// Parameter for `blink`.
DigitalOut *led;

/*!
    @brief Periodically use the provided Dataloader to read data.
*/
void loader_periodic_read()
{
    while (!target_loader->is_buf_full())
    {
        flags.wait_all(READ_DATA_FLAG);
        target_loader->load_data();
    }
    flags.set(RECORDING_FINISHED_FLAG);
}

/*!
    @brief use polling to detect if a screen button in the provided array
        was touched.
*/
void detect_touch()
{
    ScreenButton *touched_bt;  /*!< the screen button touched */
    touched_bt = wait_buttons(cur_bt_group, group_len);
    touched_bt->action();  /*!< set the specified flag to alert */
}

// Toggles LED
void blink(DigitalOut *led)
{
    // Toggle the LED every 250ms.
    while (1) {
        *led = !*led;
        ThisThread::sleep_for(250ms);
    }
}


int main()
{
    // Starting configuration.
    // Interrupt not disabled as it causes error to LCD.
    // Setup hardwares.
    setup_gpio();
    setup_lcd();

    // Show start view.
    start_view();

    // Setup gyroscope.
    Gyroscope *gyro = &Gyroscope::gyro;
    gyro->setup();

    // Setup data structures.
    DataBuffer key_buf, input_buf;

    // The dataloader to manage gyro measurement.
    Dataloader loader(&input_buf, gyro);
    // loader.set_buffer(&input_buf);  // set_buffer lead to failure

    // Setup variables.
    State state;  /*!< the current state of the device */
    // The latest recorded action before getting to idle state.
    PrevStatus prev_status;

    state = IDLE;
    prev_status = START;
    bool is_test;  /*!< whether the current recording is for testing */

    // Setup Dynamic Time Warping (DTW) variables and buffers.
    /*
        These variables were setup to fix a series of MbedOS errors
        caused by calling the original functions where they
        were originally located.
    */
    float max_dist = 0.0f;  /*!< max distance between input and key */
    float costs[2][BUFFER_LEN];  /*!< DTW cost matrix */
    /*!< pointer for accessing data of specific axis */
    float *key_ptr, *input_ptr;

    // Initialize gyro_read parameter.
    target_loader = &loader;

    while (1)
    {
        thread_sleep_for(500);

        // Set threads for screen touch detection and gyro reading.
        Thread idle_detect, record_detect, blink_led, gyro_read;

        /* -------- IDLE State -------- */
        device_ready_view(prev_status);

        // Set the button to be detected.
        if (prev_status == START)
        {
            cur_bt_group = idle_no_key_bts;
            group_len = IDLE_NO_KEY_LEN;
        }
        else
        {
            // A key was stored.
            cur_bt_group = idle_default_bts;
            group_len = IDLE_DEFAULT_LEN;
        }

        idle_detect.start(detect_touch);  // start detecting buttons
        flags.wait_any(START_SET_KEY_FLAG | START_TEST_FLAG,
            WAIT_FOREVER, false);
        
        // Button touched, prepare for a new recording.
        is_test = ((flags.get() & START_TEST_FLAG) != 0);
        
        // Clear flags used to ensure further use.
        flags.clear(START_SET_KEY_FLAG | START_TEST_FLAG);
        
        // Reset buffer.
        loader.reset();  // reset loader data and clear buffer
        state = RECORDING;

        thread_sleep_for(500);  // avoid detecting button on the next view

        /* -------- Working States -------- */
        switch (state)
        {
            case RECORDING:
                recording_view(is_test);

                // Set button to be monitored.
                cur_bt_group = recording_bts;
                group_len = RECORDING_LEN;

                gyro->enable();  // enable gyroscope measuring

                // Set button group to be monitored.
                cur_bt_group = recording_bts;
                group_len = RECORDING_LEN;

                set_periodic_read();  // start measuring signal

                // Start reading data.
                gyro_read.start(loader_periodic_read);

                // Start status light blinking.
                blink_led.start(callback(blink,
                    (is_test) ? &red_led : &green_led));

                // Start detecting thread.
                record_detect.start(detect_touch);

                flags.wait_any(RECORDING_CANCELLED_FLAG
                    | RECORDING_FINISHED_FLAG, WAIT_FOREVER, false);

                // Recording cancelled or finished.
                // Stop threads if needed.
                if (gyro_read.get_state() == rtos::Thread::State::Running)
                    gyro_read.terminate();
                if (record_detect.get_state() == rtos::Thread::State::Running)
                    record_detect.terminate();
                blink_led.terminate();
                
                // Stop hardwares.
                end_periodic_read();  // end measuring signal
                gyro->disable();

                if ((flags.get() & RECORDING_CANCELLED_FLAG) != 0)
                {
                    flags.clear(RECORDING_CANCELLED_FLAG);
                    state = IDLE;
                    break;
                }
                flags.clear(RECORDING_FINISHED_FLAG);
                state = PROCESSING;
            
            case PROCESSING:
                state = IDLE;
                if (!is_test)
                {
                    // Store the new key.
                    // This method avoid the MbedOS parameter error.
                    key_buf.index = input_buf.index;

                    for (uint8_t axis = 0; axis < TOTAL_DIM; axis++)
                    {
                        for (uint16_t j = 0; j < BUFFER_LEN; j++)
                            key_buf.ang_v[axis][j] = input_buf.ang_v[axis][j];
                    }

                    /*
                        For unknown reason, call to operator= leads
                        to MbedOS parameter error.
                        Even a call to an empty function like
                        key_buf.copy() that does no operations trigger
                        the same error.
                    */
                    // key_buf = input_buf;

                    prev_status = SET_KEY;
                    break;
                }
                
                // Compare if the input matches the key using DTW.
                prev_status = test_match(key_buf, input_buf) ?
                   MATCH : MISMATCH;
                break;

            default:
                break;  // should never arrive here
        }
    }
    
    return 0;
}
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

// note: button interrupt disabled
// not sure how to pass parameters to the threads, not working
// not sure how to use interrupt to control screen

// Parameters for `detect_touch`.
ScreenButton *const *cur_bt_group;  /*!< the button group to be monitored */
uint8_t group_len;  /*!< length of the button group */

// Parameters for `loader_perodic_read`.
Dataloader *target_loader;

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


int main()
{
    // Starting configuration.
    // Disable interrupts causes error for lcd.
    // __disable_irq(); // disable global interrupts

    // Setup hardwares.
    setup_gpio();
    setup_lcd();

    // Show start view.
    start_view();

    // Setup gyroscope.
    Gyroscope *gyro = &Gyroscope::gyro;
    gyro->setup();

    // __disable_irq();
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
    // __enable_irq();  // enable global interrupts

    while (1)
    {
        printf("--- Idle State --- \n");
        // Set threads for screen touch detection and gyro reading.
        Thread idle_detect, record_detect, gyro_read;

        /* -------- IDLE State -------- */
        device_ready_view(prev_status);

        // Prepare for polling on screen buttons.
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

        printf("idle ended, cur State is %d   ", state);

        thread_sleep_for(500);  // avoid detecting button on the next view

        /* -------- Working States -------- */
        switch (state)
        {
            case RECORDING:
                recording_view(is_test);
                printf("---- Recording Started -----");

                if ((flags.get() & RECORDING_FINISHED_FLAG) != 0)
                    printf("****Finished flag Set!!!!!!!****");
                // __disable_irq();
                // Set button to be monitored.
                cur_bt_group = recording_bts;
                group_len = RECORDING_LEN;

                gyro->enable();  // enable gyroscope measuring

                // Set button group to be monitored.
                cur_bt_group = recording_bts;
                group_len = RECORDING_LEN;

                set_periodic_read();  // start measuring signal
                // // __enable_irq();

                // Start reading data.
                gyro_read.start(loader_periodic_read);

                // // Start detecting thread.
                record_detect.start(detect_touch);

                flags.wait_any(RECORDING_CANCELLED_FLAG
                    | RECORDING_FINISHED_FLAG, WAIT_FOREVER, false);

                // Recording cancelled or finished.
                // Stop threads if needed.
                if (gyro_read.get_state() == rtos::Thread::State::Running)
                    gyro_read.terminate();
                if (record_detect.get_state() == rtos::Thread::State::Running)
                    record_detect.terminate();
                
                // Stop hardwares.
                end_periodic_read();  // end measuring signal
                gyro->disable();

                if ((flags.get() & RECORDING_CANCELLED_FLAG) != 0)
                {
                    flags.clear(RECORDING_CANCELLED_FLAG);
                    printf("Recording Cancelled !");
                    state = IDLE;
                    break;
                }
                flags.clear(RECORDING_FINISHED_FLAG);
                printf("--- Recording Ended --- \n");
                state = PROCESSING;
            
            case PROCESSING:
                // __disable_irq();  // disable irq for data processing
                state = IDLE;
                if (!is_test)
                {
                    // Store the new key.
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
                    // __enable_irq();
                    break;
                }
                
                max_dist = 0.0f;
                for (uint8_t axis = 0; axis < TOTAL_DIM; axis++)
                {
                    key_ptr = key_buf.ang_v[axis];
                    input_ptr = input_buf.ang_v[axis];

                    costs[0][0] = abs(key_buf.ang_v[axis][0]
                        - input_buf.ang_v[axis][0]);

                    // Setup edge entries. Apply rolling array to save space.
                    costs[1][0] = costs[0][0];
                    for (uint16_t c = 1; c < input_buf.index + 1; c++)
                        costs[0][c] = costs[0][c - 1]
                            + abs(key_ptr[0] - input_ptr[c]);
                    
                    /*
                        For unknown reasons, the program may stuck at the
                        the starting view if the following for loop is used.
                    */
                    // Evaluate cost matrix.
                    // for (uint16_t r = 1; r < key_buf.index + 1; r++)
                    // {
                    //     // for (uint16_t c = 1; c < input_buf.index + 1; c++)
                    //     // {
                    //     //     // Replace % 2 with & 1 to save time.
                    //     //     costs[r & 1][c] = abs(key_ptr[r] - input_ptr[c])
                    //     //         + min(
                    //     //         costs[(r - 1) & 1][c - 1],
                    //     //         min(costs[(r - 1) & 1][c], costs[r & 1][c - 1]));
                    //     // }
                    // }
                    // max_dist = max(max_dist,
                    //     costs[(key_buf.index - 1) & 2][input_buf.index - 1]);

                    /*
                        For unknown reason, call to `cmp_seqs` trigger
                        MbedOS Fault exception.
                        The error still remains even after all operations
                        in the function were removed except a default
                        return value `0.0f`.
                        `cmp_seqs` was not public and was added to the
                        `backend.h` later in an attempt to fix the error
                        caused by calling `test_match`.
                    */
                    max_dist = max(max_dist,
                        cmp_seqs(key_buf.ang_v[axis], key_buf.index + 1,
                        input_buf.ang_v[axis], input_buf.index + 1));
                }
                prev_status = (max_dist < MAX_AXIS_DISTANCE) ?
                    MATCH : MISMATCH;
                
                /*
                    For unknown reason, call to `test_match` leads
                    to MbedOS parameter error.
                    Removing all operations in `test_match`
                    while keeping a default return value `false` trigger
                    the same error.
                */
                // prev_status = test_match(key_buf, input_buf) ?
                //    MATCH : MISMATCH;
                // __enable_irq();
                break;

            default:
                break;
        }

        printf("------ Recording and comparison ended! ----- \n");
    }
    
    return 0;
}
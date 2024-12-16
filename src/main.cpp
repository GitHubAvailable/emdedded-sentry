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
    while (target_loader->is_buf_full())
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

    // Initialize gyro_read parameter.
    target_loader = &loader;
    // __enable_irq();  // enable global interrupts

    while (1)
    {
        // Set threads for screen touch detection and gyro reading.
        Thread bt_detect, gyro_read;

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

        bt_detect.start(detect_touch);  // start detecting buttons
        flags.wait_any(START_SET_KEY_FLAG | START_TEST_FLAG,
            WAIT_FOREVER, false);
        
        // Button touched, prepare for a new recording.
        is_test = ((flags.get() & START_TEST_FLAG) != 0);
        
        // Clear flags used to ensure further use.
        flags.clear(START_SET_KEY_FLAG | START_TEST_FLAG);
        
        // Reset buffer.
        loader.reset();  // reset loader data and clear buffer
        state = RECORDING;

        /* -------- Working States -------- */
        switch (state)
        {
            case RECORDING:
                recording_view(is_test);
                __disable_irq();
                gyro->enable();  // enable gyroscope measuring

                // Set button group to be monitored.
                cur_bt_group = recording_bts;
                group_len = RECORDING_LEN;

                set_periodic_read();  // start measuring signal
                __enable_irq();

                // Start reading data.
                gyro_read.start(loader_periodic_read);

                // Start detecting thread.
                bt_detect.start(detect_touch);

                flags.wait_any(RECORDING_CANCELLED_FLAG
                    | RECORDING_FINISHED_FLAG, WAIT_FOREVER, false);

                // Recording cancelled or finished.
                // Stop threads if needed.
                if (gyro_read.get_state() == rtos::Thread::State::Running)
                    gyro_read.terminate();
                if (bt_detect.get_state() == rtos::Thread::State::Running)
                    bt_detect.terminate();
                
                // Stop hardwares.
                end_periodic_read();  // end measuring signal
                gyro->disable();

                if (flags.get() | RECORDING_CANCELLED_FLAG)
                {
                    state = IDLE;
                    break;
                }
                state = PROCESSING;
            
            case PROCESSING:
                __disable_irq();  // disable irq for data processing
                state = IDLE;
                if (!is_test)
                {
                    key_buf = input_buf;  // store the new key
                    prev_status = SET_KEY;
                    __enable_irq();
                    break;
                }
                
                prev_status = test_match(key_buf, input_buf) ?
                    MATCH : MISMATCH;
                __enable_irq();
                break;

            default:
                break;
        }
    }
    
    return 0;
}
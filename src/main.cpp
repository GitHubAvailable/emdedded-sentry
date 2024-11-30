#include <mbed.h>
#include "backend.h"
#include "display.h"
#include "interrupt.h"
#include "setup.h"
#include "state.h"

// The current state of the device.
State state;

// The current recording mode of the device.
bool mode;

int main()
{
    // Configuration.
    __disable_irq(); // disable global interrupts
    setup_gpio();

    state = State::IDLE;
    __enable_irq();  // enable global interrupts

    while (1)
    {
        switch (state)
        {
            case RECORDING:
                /* code */
                break;
            
            case PROCESSING:
                /* process data */
                /* display results */
                break;

            default:
                reset_device();
                idle_view();
                flags.wait_all(START_RECORDING);
                break;
        }
    }
    
    return 0;
}
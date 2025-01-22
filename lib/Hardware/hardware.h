/* hardware Library
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
#ifndef Hardware_h
#define Hardware_h

#include <mbed.h>
#include <LCD_DISCO_F429ZI.h>
#include <TS_DISCO_F429ZI.h>

// GPIO macros.
#define BUTTON PA_0  // user button
#define GREEN_LED PG_13
#define RED_LED PG_14

#define OFF 0
#define ON 1

// Interrupt pin.
#define DATA_READY PA_2

// SPI macros.
#define MISO PF_9
#define MOSI PF_8
#define SCK PF_7
#define GYRO_SS PC_1  // the SS line of the gyroscope

// Gyroscope settings.
#define CLK_FREQ 1'000'000  // default frequence 1MHz
#define ENABLE 3

#define GYRO_CTRL1_ADDR 0x20
#define GYRO_CTRL1_CONFIG 0b00'11'0'0'0'0  // ODR 100Hz, 25 cutoff, xyz enable
#define GYRO_ENABLE (GYRO_CTRL1_CONFIG | (0b1111))  // enable device
#define GYRO_DISABLE (GYRO_CTRL1_CONFIG & (~(1 << ENABLE)))
#define GYRO_SLEEP (GYRO_ENABLE & (~(0b111)))  // turn off all axis, sleep

#define GYRO_CTRL3_ADDR 0x22
#define GYRO_CTRL3_CONFIG 0b0'0'0'0'1'000  // enable INT2

#define GYRO_CTRL4_ADDR 0x23
#define GYRO_CTRL4_CONFIG 0b0'0'01'0'00'0  // full-scale selection, 500dps

#define DATA_ADDR 0x28
#define READ 0x80
#define AUTO_INCREMENT 0x40
#define READ_CMD (DATA_ADDR | READ | AUTO_INCREMENT)

// LCD Screen Size
#define WIDTH 240
#define HEIGHT 320

// LCD available font sizes.
#define FONT24 &Font24
#define FONT20 &Font20
#define FONT16 &Font16
#define FONT12 &Font12
#define FONT8 &Font8

// LCD default color settings.
#define DEFAULT_BG LCD_COLOR_BLACK
#define TEXT_DEFAULT_BG LCD_COLOR_BLACK  // backgroud color of texts
#define TEXT_DEFAULT_COLOR LCD_COLOR_GREEN

// The gyroscope data ready interrupt pin.
extern InterruptIn data_ready_int;

// The green LED on the board.
extern DigitalOut green_led;

// The red LED on the board.
extern DigitalOut red_led;

// The ticker used to repeatedly load data.
extern Ticker gyro_read_ticker;

/*!
    @brief The SPI master to communicate with I3G4250D gyroscope.
*/
struct Gyroscope
{
    private:
        uint8_t write_buf[8];
        uint8_t read_buf[8];
        SPI gyro_spi;

        /*!
            @brief Create a gyroscop with pre-defined pin configuration.
        */
        Gyroscope();

        /*!
            @brief Set a specific register on the gyroscope.
            @param addr the address of the register to be set.
            @param val the value to be assigned.
        */
        void set_reg(uint8_t addr, uint8_t val);
    
    public:
        static Gyroscope gyro;  /*!< a gyroscope singleton for use */

        /*!
            @brief Set default configuration of I3G4250D gyroscope.
            @note Should be called with IRQ disabled. This function
                only loads the default configuration to the gyroscope.
                `enable()` should be called before using it.
        */
        void setup();

        /*!
            @brief Enable the gyroscope.
            @warning This function waits and should not be called from
                interrupt context.
        */
        void enable();

        /*!
            @brief Disable the gyroscope.
            @warning This function waits and should not be called from
                interrupt context.
        */
        void disable();

        /*!
            @brief Set the gyroscope to sleep mode.
            @warning This function waits and should not be called from
                interrupt context.
        */
        void sleep();

        /*!
            @brief Read angular speed of all 3 axis from the gyroscope.
            @param buf should at least have length 3, or could cause
                unexpected behavior
            @warning This function waits and should not be called from
                interrupt context.
        */
        void read(float *buf);
};

// The ILI9341 LCD screen for display.
extern LCD_DISCO_F429ZI lcd;

// The ILI9341 LCD touch screen module.
extern TS_DISCO_F429ZI ts;

#endif
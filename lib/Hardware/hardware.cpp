/* hardware.cpp - Implementation of the hardware utilities for
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
#include "interrupt.h"

#define SCALING_FACTOR (17.5f * 0.0174532925199432957692236907684886f / 1000.0f)

// The gyroscope data ready interrupt pin.
InterruptIn data_ready_int(DATA_READY, PullDown);

// The green LED on the board.
DigitalOut green_led(GREEN_LED);

// The red LED on the board.
DigitalOut red_led(RED_LED);

// The ticker used to repeatedly load data.
Ticker gyro_read_ticker;

Gyroscope Gyroscope::gyro;

Gyroscope::Gyroscope() : gyro_spi(MISO, MOSI, SCK, GYRO_SS, use_gpio_ssel)
{
    data_ready_int.rise(&data_ready);
}

void Gyroscope::set_reg(uint8_t addr, uint8_t val)
{
    write_buf[0] = addr;
    write_buf[1] = val;
    gyro_spi.transfer(write_buf, 2, read_buf, 2, com_finish);
    flags.wait_all(GYRO_SPI_COM_FLAG, 5, true);
}

void Gyroscope::setup()
{
    // 8-bit data, idle HIGH, send data at rising edge.
    gyro_spi.format(8, 3);
    gyro_spi.frequency(1'000'000);  // clock frequency 1MHz

    // Configure gyroscope.
    set_reg(GYRO_CTRL1_ADDR, GYRO_CTRL1_CONFIG);
    set_reg(GYRO_CTRL3_ADDR, GYRO_CTRL3_CONFIG);
    set_reg(GYRO_CTRL4_ADDR, GYRO_CTRL4_CONFIG);
}

void Gyroscope::enable()
{
    set_reg(GYRO_CTRL1_ADDR, GYRO_ENABLE);
}

void Gyroscope::disable()
{
    set_reg(GYRO_CTRL1_ADDR, GYRO_DISABLE);
}

void Gyroscope::sleep()
{
    set_reg(GYRO_CTRL1_ADDR, GYRO_SLEEP);
}

void Gyroscope::read(float *buf)
{
    write_buf[0] = READ_CMD;

    // Read data.
    flags.wait_all(GYRO_DATA_READY_FLAG, 20, true);
    gyro_spi.transfer(write_buf, 7, read_buf, 7, com_finish);

    // Wait for read to complete.
    // Timeout is set to make it nonblocking. Timeout should not happen.
    flags.wait_all(GYRO_SPI_COM_FLAG, 80, true);

    uint16_t raw_gx, raw_gy, raw_gz;

    // Extract 16-bit raw data.
    raw_gx = (read_buf[2] << 8) | read_buf[1];
    raw_gy = (read_buf[4] << 8) | read_buf[3];
    raw_gz = (read_buf[6] << 8) | read_buf[5];

    // Load data to the provided buffer.
    buf[0] = raw_gx * SCALING_FACTOR;
    buf[1] = raw_gy * SCALING_FACTOR;
    buf[2] = raw_gz * SCALING_FACTOR;

    return;
}

// The ILI9341 LCD screen for display.
LCD_DISCO_F429ZI lcd;

// The ILI9341 LCD touch screen module.
TS_DISCO_F429ZI ts;
/*
 *  Copyright (c) 2017, Intel Corporation
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of the Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL CORPORATION OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Inter-Integrated Circuit (I2C)
 *
 * This example uses a Microchip 24FC256-I/P I2C EEPROM to demonstrate
 * both polled, interrupt and DMA based master I2C transfers on I2C_0.
 *
 * EEPROM pins 2, 3, 4 and 7 are connected to ground.
 * EEPROM pins 1 and 8 are connected to 3.3V.
 * EEPROM pin 5 is connected to I2C_0 SDA pin with pull-up resistor.
 * EEPROM pin 6 is connected to I2C_0 SCL pin with pull-up resistor.
 */

#include <unistd.h>
#include <stdbool.h>
#include "clk.h"
#include "qm_common.h"
#include "qm_i2c.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "U8glib.h"


static void pin_mux_setup(void)
{
	qm_pmux_select(QM_PIN_ID_6, QM_PIN_6_FN_I2C0_SCL);
	qm_pmux_select(QM_PIN_ID_7, QM_PIN_7_FN_I2C0_SDA);
}

int main(void)
{
	qm_i2c_config_t cfg;

	QM_PUTS("Starting: I2C");

	/* Enable I2C 0. */
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_I2C_M0_REGISTER);

	pin_mux_setup();

	/* Configure I2C. */
	cfg.address_mode = QM_I2C_7_BIT;
	cfg.mode = QM_I2C_SLAVE;
	cfg.speed = QM_I2C_SPEED_FAST; // QM_I2C_SPEED_STD;
	cfg.slave_addr = 0x3C;

	if (qm_i2c_set_config(QM_I2C_0, &cfg)) {
		QM_PUTS("Error: I2C_0 config failed");
		return 1;
	}
	if (U8begin(SH1106,I2C,128,64))
		QM_PUTS("setup succeed\r\n");
	else
		QM_PUTS("setup failed\r\n");

	QM_PUTS("Finished: I2C");

	return 0;
}

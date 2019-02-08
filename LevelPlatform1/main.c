/*
 * Copyright (c) 2017, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL CORPORATION OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *   Combined Accelerometer, RTC, A4988
 *   Sample Accel at 8 Hz
 *   If down pitch wheels forward, else backward
 *
 */

#include "clk.h"
#include "qm_gpio.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "clk.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_rtc.h"

/* The following defines the pin and pin mux details for each SoC. */
#define PIN_OUT 24
#define LED_PIN_ID (QM_PIN_ID_24)
#define PIN_MUX_FN (QM_PIN_24_FN_GPIO_24)

#define INTERVAL (QM_RTC_ALARM_SECOND(CLK_RTC_DIV_8)) /* 125 milliseconds. */
#define NUM_SAMPLES (500)

static volatile bool complete = false;
static volatile int samples = 0;
static volatile bool ledstate = false;

static void pin_mux_setup(void)
{
	qm_pmux_select(LED_PIN_ID, PIN_MUX_FN);
}

/* rtc callback will run every time the RTC alarm triggers. */
static void rtc_callback(void *data)
{
	if (ledstate)
		qm_gpio_set_pin(QM_GPIO_0, PIN_OUT);
	else
		qm_gpio_clear_pin(QM_GPIO_0, PIN_OUT);
	ledstate = !ledstate;
	/* Reset the RTC alarm to fire again if necessary. */
	if (samples < NUM_SAMPLES) {
		qm_rtc_set_alarm(QM_RTC_0,
				 (QM_RTC[QM_RTC_0]->rtc_ccvr + INTERVAL));
		samples++;
	} else {
		complete = true;
	}
}

int main(void)
{
	static qm_gpio_port_config_t cfg;

	/* Set the GPIO pin muxing. */
	pin_mux_setup();

	/* Set the GPIO pin direction to out and write the config. */
	cfg.direction = BIT(PIN_OUT);
	qm_gpio_set_config(QM_GPIO_0, &cfg);

	qm_rtc_config_t rtc;

	QM_PUTS("Starting app");

	/* Configure the RTC and request the IRQ. */
	rtc.init_val = 0;
	rtc.alarm_en = true;
	rtc.alarm_val = INTERVAL;
	rtc.callback = rtc_callback;
	rtc.callback_data = NULL;
	rtc.prescaler = CLK_RTC_DIV_1;

	QM_IR_UNMASK_INT(QM_IRQ_RTC_0_INT);

	QM_IRQ_REQUEST(QM_IRQ_RTC_0_INT, qm_rtc_0_isr);

	/* Enable the RTC. */
	clk_periph_enable(CLK_PERIPH_RTC_REGISTER | CLK_PERIPH_CLK);

	/* Start the RTC. */
	qm_rtc_set_config(QM_RTC_0, &rtc);

	/* Wait for the correct number of samples to be read. */
	while (!complete) {
	}
	QM_PUTS("All done");

}

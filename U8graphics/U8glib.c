/*
 * U8glib.c
 *
 *  Created on: Jul 24, 2017
 *      Author: henryd
 */


#include <unistd.h>
#include <stdbool.h>
#include "U8glib.h"
#include "qm_common.h"
#include "qm_i2c.h"

static struct q8Device dev;

bool U8begin(enum oledChipSet chip,enum serialInterface si,uint8_t width,uint8_t height)
{
	bool rc = true;
	switch (chip)
	{
		case SH1106:
			dev.chip = SH1106;
			dev.slaveAddress = 0x3c;
			break;
		default:
			rc = false;
			break;
	}
	if (rc)
	{
		switch (si)
		{
			case I2C:
				dev.si = I2C;
				break;
			default:
				rc = false;
		}
	}
	if (rc)
	{
		dev.width = width;
		dev.height = height;
		dev.x = 0;
		dev.y = 0;
	}
	return rc;
}

/*
 * U8glib.h
 *
 *  Created on: Jul 24, 2017
 *      Author: henryd
 *  Quick & dirty reformat of the Arduino U8glib
 *  Only I2C supported so far
 *  Only SH1106 chipset supported so far
 */

#ifndef U8GLIB_H_
#define U8GLIB_H_

enum oledChipSet {SH1106};
enum serialInterface {I2C};

struct q8Device {
	enum oledChipSet chip;
	enum serialInterface si;
	uint8_t slaveAddress;
	uint8_t width,height;	// screen dimensions
	uint8_t x,y;	// last pixel written
};

/*
 * U8begin sets up basic data structures and polls device for slaveID
 * responds true or false if responds ok
 */
bool U8begin(enum oledChipSet chip,enum serialInterface si,uint8_t width,uint8_t height);

// Supported graphic chips
// SH1106
#define SH1106_SLAVE_ID 0x3c
#endif /* U8GLIB_H_ */

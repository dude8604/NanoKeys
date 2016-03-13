/*
 * Keyboard.h
 *
 *  Created on: Feb 16, 2016
 *      Author: Pat Deegan
 *      Part of the NanoKeys Project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
 *      
 * http://flyingcarsandstuff.com/projects/NanoKeys/
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further information on licensing
 * terms.
 *
 *
 * This is the implementation of the HID keyboard service.  It is
 * not meant to be used directly by library user code.
 *
 * It provides everything needed by the specs, including a suitable
 * report map, control point (that actually is only a stub and does nothing,
 * for the moment), and a key code map for all ascii values.
 *      
 */

#ifndef NANOKEYS_SRC_INCLUDES_SERVICES_KEYBOARD_H_
#define NANOKEYS_SRC_INCLUDES_SERVICES_KEYBOARD_H_


#include "../NanoKeysIncludes.h"
#include "../NanoKeysConfig.h"


//define NANOKEYS_KBD_USE_TICKER_TO_CLEAR

namespace NanoKeys {
namespace Service {

#define KB_VALUE_NUM_BYTES    2

class HIDKeyboardValueBytes {

public:
	HIDKeyboardValueBytes() {
		clear();
	}

	uint8_t *getPointer(void) {
		return theBytes;
	}

	const uint8_t *getPointer(void) const {
		return theBytes;
	}

	uint8_t numBytes() {
		return KB_VALUE_NUM_BYTES;
	}


	void clear() {

		memset(theBytes, 0, KB_VALUE_NUM_BYTES);

	}
public:
	uint8_t theBytes[KB_VALUE_NUM_BYTES + 1];
};

class HIDKeyReportRefDescBytes {
public:
	HIDKeyReportRefDescBytes() {

		theBytes[0] = 0x01;
		theBytes[1] = 0x01;

	}

	uint8_t *getPointer(void) {
		return theBytes;
	}

	const uint8_t *getPointer(void) const {
		return theBytes;
	}

	uint8_t numBytes() {
		return 2;
	}


private:
	uint8_t theBytes[2];

};


class Keyboard {

	typedef enum{
		KBD_IDLE=0,
		KBD_SENDING_KEYCODE,
		KBD_SEND_REFACTORY,
		KBD_CLEARING_KEYCODE,
		KBD_CLEAR_REFACTORY
	} State;

public:
	Keyboard(BLE &_ble);
	virtual ~Keyboard();

	bool isReady();
	virtual void onDataWritten(const GattWriteCallbackParams *params);
	void dataSent(unsigned int count);

	bool send(uint8_t c);

	volatile State state() { return current_state;}


	void keyUp();

protected:

	void clearCode();
	void setState(State setTo);

	BLE &ble;

	HIDKeyboardValueBytes valueBytes;
	HIDKeyReportRefDescBytes descBytes;
	uint8_t controlPointValue;

	GattCharacteristic hidInfo;
	WriteOnlyGattCharacteristic<uint8_t> controlPoint;
	GattCharacteristic reportMap;
	GattCharacteristic report;
	GattAttribute repDesc;
	GattAttribute * repDescList[1];
	GattService * kbserv;
#ifdef NANOKEYS_KBD_USE_TICKER_TO_CLEAR
	Ticker	 codeClearTicker;
#endif
	uint32_t last_send_time_ms;


	volatile State current_state;




};


}
}


#endif /* NANOKEYS_SRC_INCLUDES_SERVICES_KEYBOARD_H_ */

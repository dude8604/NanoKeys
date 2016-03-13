/*
 * Keyboard.cpp
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
 *      
 */

#include "includes/services/Keyboard.h"
#include "includes/NanoKeysDebug.h"

#define KB_LED_NUM      0x01
#define KB_LED_CAPS     0x02
#define KB_LED_SCROLL   0x04

#define KEY_CODE_MOD_LEFT_CONTROL	0x01
#define KEY_CODE_MOD_LEFT_SHIFT		0x02
#define KEY_CODE_MOD_LEFT_ALT		0x04
#define KEY_CODE_MOD_LEFT_GUI		0x08
#define KEY_CODE_MOD_RIGHT_CONTROL	0x10
#define KEY_CODE_MOD_RIGHT_SHIFT	0x20
#define KEY_CODE_MOD_RIGHT_ALT		0x40
#define KEY_CODE_MOD_RIGHT_GUI		0x80

#define KEY_CODE_LEFT_CONTROL		0xE0
#define KEY_CODE_LEFT_SHIFT			0xE1
#define KEY_CODE_LEFT_ALT			0xE2
#define KEY_CODE_LEFT_GUI			0xE3
#define KEY_CODE_RIGHT_CONTROL		0xE4
#define KEY_CODE_RIGHT_SHIFT		0xE5
#define KEY_CODE_RIGHT_ALT			0xE6
#define KEY_CODE_RIGHT_GUI   		0xE7
#define KEY_CODE_1					0x1E
#define KEY_CODE_2					0x1F
#define KEY_CODE_3					0x20
#define KEY_CODE_4					0x21
#define KEY_CODE_5					0x22
#define KEY_CODE_6					0x23
#define KEY_CODE_7					0x24
#define KEY_CODE_8					0x25
#define KEY_CODE_9					0x26
#define KEY_CODE_0					0x27
#define KEY_CODE_A					0x04
#define KEY_CODE_B					0x05
#define KEY_CODE_C					0x06
#define KEY_CODE_D					0x07
#define KEY_CODE_E					0x08
#define KEY_CODE_F					0x09
#define KEY_CODE_G					0x0A
#define KEY_CODE_H					0x0B
#define KEY_CODE_I					0x0C
#define KEY_CODE_J					0x0D
#define KEY_CODE_K					0x0E
#define KEY_CODE_L					0x0F
#define KEY_CODE_M					0x10
#define KEY_CODE_N					0x11
#define KEY_CODE_O					0x12
#define KEY_CODE_P					0x13
#define KEY_CODE_Q					0x14
#define KEY_CODE_R					0x15
#define KEY_CODE_S					0x16
#define KEY_CODE_T					0x17
#define KEY_CODE_U					0x18
#define KEY_CODE_V					0x19
#define KEY_CODE_W					0x1A
#define KEY_CODE_X					0x1B
#define KEY_CODE_Y					0x1C
#define KEY_CODE_Z					0x1D
#define KEY_CODE_MINUS				0x2D
#define KEY_CODE_EQUAL				0x2E
#define KEY_CODE_SQBRAK_LEFT		0x2F
#define KEY_CODE_BACKSLASH			0x31
#define KEY_CODE_SEMICOLON			0x33
#define KEY_CODE_SQUOTE				0x34
#define KEY_CODE_BACKTICK			0x35
#define KEY_CODE_COMMA				0x36
#define KEY_CODE_PERIOD				0x37
#define KEY_CODE_SQBRAK_RIGHT		0x30
#define KEY_CODE_SLASH				0x38
#define KEY_CODE_F1 				0x3A
#define KEY_CODE_F2 				0x3B
#define KEY_CODE_F3 				0x3C
#define KEY_CODE_F4 				0x3D
#define KEY_CODE_F5 				0x3E
#define KEY_CODE_F6 				0x3F
#define KEY_CODE_F7 				0x40
#define KEY_CODE_F8 				0x41
#define KEY_CODE_F9 				0x42
#define KEY_CODE_F10				0x43
#define KEY_CODE_F11				0x44
#define KEY_CODE_F12				0x45
#define KEY_CODE_APP				0x65
#define KEY_CODE_ENTER     			0x28
#define KEY_CODE_BACKSPACE   		0x2A
#define KEY_CODE_ESC				0x29
#define KEY_CODE_TAB				0x2B
#define KEY_CODE_SPACE				0x2C
#define KEY_CODE_INSERT				0x49
#define KEY_CODE_HOME				0x4A
#define KEY_CODE_PAGE_UP			0x4B
#define KEY_CODE_DELETE				0x4C
#define KEY_CODE_END				0x4D
#define KEY_CODE_PAGE_DOWN			0x4E
#define KEY_CODE_PRINTSCREEN		0x46
#define KEY_CODE_ARROW_RIGHT		0x4F
#define KEY_CODE_ARROW_LEFT			0x50
#define KEY_CODE_ARROW_DOWN			0x51
#define KEY_CODE_ARROW_UP			0x52




// "Partial" key codes--require a SHIFT as well
#define KEY_CODE_BANG				0x1E
#define KEY_CODE_AT					0x1F
#define KEY_CODE_POUND				0x20
#define KEY_CODE_DOLLAR				0x21
#define KEY_CODE_PERCENT			0x22
#define KEY_CODE_AMPERSAND			0x24
#define KEY_CODE_STAR				0x25
#define KEY_CODE_PAREN_OPEN			0x26
#define KEY_CODE_PAREN_CLOSE		0x27
#define KEY_CODE_UNDERSCORE			0x2D
#define KEY_CODE_PLUS				0x2E
#define KEY_CODE_COLON				0x33
#define KEY_CODE_DQUOTE				0x34
#define KEY_CODE_LESSTHAN			0x36
#define KEY_CODE_GREATERTHAN		0x37
#define KEY_CODE_QUESTIONMARK		0x38











namespace NanoKeys {
namespace Service {


static uint8_t reportMapValue[] = {  // USB report descriptor
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    // REPORT_ID
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};




#define SHIFT 0x80

static const unsigned char asciiMap[] = {
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  KEY_CODE_BACKSPACE,          // '\b'
  KEY_CODE_TAB,                // '\t'
  KEY_CODE_ENTER,              // '\n'
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  KEY_CODE_SPACE,                              // ' '
  KEY_CODE_BANG 		| SHIFT,               // '!'
  KEY_CODE_DQUOTE 		| SHIFT,               // '"'
  KEY_CODE_POUND 		| SHIFT,               // '#'
  KEY_CODE_DOLLAR 		| SHIFT,               // '$'
  KEY_CODE_PERCENT 		| SHIFT,               // '%'
  KEY_CODE_AMPERSAND 	| SHIFT,               // '&'
  KEY_CODE_SQUOTE,                             // '''
  KEY_CODE_PAREN_OPEN 	| SHIFT,               // '('
  KEY_CODE_PAREN_CLOSE 	| SHIFT,               // ')'
  KEY_CODE_STAR 		| SHIFT,               // '*'
  KEY_CODE_PLUS 		| SHIFT,               // '+'
  KEY_CODE_COMMA,              // ','
  KEY_CODE_MINUS,              // '-'
  KEY_CODE_PERIOD,             // '.'
  KEY_CODE_SLASH,              // '/'
  KEY_CODE_0,                  // '0'
  KEY_CODE_1,                  // '1'
  KEY_CODE_2,                  // '2'
  KEY_CODE_3,                  // '3'
  KEY_CODE_4,                  // '4'
  KEY_CODE_5,                  // '5'
  KEY_CODE_6,                  // '6'
  KEY_CODE_7,                  // '7'
  KEY_CODE_8,                  // '8'
  KEY_CODE_9,                  // '9'

  KEY_CODE_COLON 		| SHIFT,				// ':'
  KEY_CODE_SEMICOLON, 							// ';'
  KEY_CODE_LESSTHAN 	| SHIFT,				// '<'
  KEY_CODE_EQUAL,              					// '='
  KEY_CODE_GREATERTHAN	| SHIFT,				// '>'
  KEY_CODE_QUESTIONMARK | SHIFT,				// '?'
  KEY_CODE_AT 			| SHIFT,				// '@'
  KEY_CODE_A | SHIFT,          // 'A'
  KEY_CODE_B | SHIFT,          // 'B'
  KEY_CODE_C | SHIFT,          // 'C'
  KEY_CODE_D | SHIFT,          // 'D'
  KEY_CODE_E | SHIFT,          // 'E'
  KEY_CODE_F | SHIFT,          // 'F'
  KEY_CODE_G | SHIFT,          // 'G'
  KEY_CODE_H | SHIFT,          // 'H'
  KEY_CODE_I | SHIFT,          // 'I'
  KEY_CODE_J | SHIFT,          // 'J'
  KEY_CODE_K | SHIFT,          // 'K'
  KEY_CODE_L | SHIFT,          // 'L'
  KEY_CODE_M | SHIFT,          // 'M'
  KEY_CODE_N | SHIFT,          // 'N'
  KEY_CODE_O | SHIFT,          // 'O'
  KEY_CODE_P | SHIFT,          // 'P'
  KEY_CODE_Q | SHIFT,          // 'Q'
  KEY_CODE_R | SHIFT,          // 'R'
  KEY_CODE_S | SHIFT,          // 'S'
  KEY_CODE_T | SHIFT,          // 'T'
  KEY_CODE_U | SHIFT,          // 'U'
  KEY_CODE_V | SHIFT,          // 'V'
  KEY_CODE_W | SHIFT,          // 'W'
  KEY_CODE_X | SHIFT,          // 'X'
  KEY_CODE_Y | SHIFT,          // 'Y'
  KEY_CODE_Z | SHIFT,          // 'Z'
  KEY_CODE_SQBRAK_LEFT,        // '['
  KEY_CODE_BACKSLASH,          // '\'
  KEY_CODE_SQBRAK_RIGHT,       // ']'
  KEY_CODE_6 | SHIFT,          // '^'
  KEY_CODE_UNDERSCORE | SHIFT, // '_'
  KEY_CODE_BACKTICK,           // '`'
  KEY_CODE_A,                  // 'a'
  KEY_CODE_B,                  // 'b'
  KEY_CODE_C,                  // 'c'
  KEY_CODE_D,                  // 'd'
  KEY_CODE_E,                  // 'e'
  KEY_CODE_F,                  // 'f'
  KEY_CODE_G,                  // 'g'
  KEY_CODE_H,                  // 'h'
  KEY_CODE_I,                  // 'i'
  KEY_CODE_J,                  // 'j'
  KEY_CODE_K,                  // 'k'
  KEY_CODE_L,                  // 'l'
  KEY_CODE_M,                  // 'm'
  KEY_CODE_N,                  // 'n'
  KEY_CODE_O,                  // 'o'
  KEY_CODE_P,                  // 'p'
  KEY_CODE_Q,                  // 'q'
  KEY_CODE_R,                  // 'r'
  KEY_CODE_S,                  // 's'
  KEY_CODE_T,                  // 't'
  KEY_CODE_U,                  // 'u'
  KEY_CODE_V,                  // 'v'
  KEY_CODE_W,                  // 'w'
  KEY_CODE_X,                  // 'x'
  KEY_CODE_Y,                  // 'y'
  KEY_CODE_Z,                  // 'z'
  0x2f | SHIFT,               // '{'
  0x31 | SHIFT,               // '|'
  0x30 | SHIFT,               // '}'
  0x35 | SHIFT,               // '~'
  0x0
};





//define NANOKEY_USE_CLEARCODE_DELAY_MS		40


static uint8_t hidInformationCharacteriticValue[] = { 0x00, 0x01, 0x00, 0x02 };
Keyboard::Keyboard(BLE &_ble) :
		ble(_ble), valueBytes(), hidInfo(
				GattCharacteristic::UUID_HID_INFORMATION_CHAR,
				hidInformationCharacteriticValue,
				sizeof(hidInformationCharacteriticValue),
				sizeof(hidInformationCharacteriticValue),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),

		controlPoint(GattCharacteristic::UUID_HID_CONTROL_POINT_CHAR,
				&controlPointValue),

		reportMap(GattCharacteristic::UUID_REPORT_MAP_CHAR, reportMapValue,
				sizeof(reportMapValue), sizeof(reportMapValue),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),

		descBytes(), repDesc(BLE_UUID_REPORT_REF_DESCR, descBytes.getPointer(),
				descBytes.numBytes(), descBytes.numBytes()), repDescList {
				&repDesc }, report(GattCharacteristic::UUID_REPORT_CHAR,
				(valueBytes.getPointer()), valueBytes.numBytes(),
				valueBytes.numBytes(),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
						| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
				repDescList, 1),
				current_state(KBD_IDLE),
				last_send_time_ms(0)

{

	GattCharacteristic *charTable[] = { &hidInfo, &controlPoint, &reportMap, &report };

	kbserv = new GattService(GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
			charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

	ble.addService(*kbserv);
	// ble.onDataWritten(this, &Keyboard::onDataWritten);

#ifndef NANOKEY_USE_CLEARCODE_DELAY_MS
	ble.gattServer().onDataSent(this, &Keyboard::dataSent);
#endif


}

void Keyboard::keyUp() {
	NANOK_DEBUG_OUTPUT('K');
	if (current_state == Keyboard::KBD_CLEARING_KEYCODE || current_state == Keyboard::KBD_CLEAR_REFACTORY) {

#ifdef NANOKEYS_KBD_USE_TICKER_TO_CLEAR
		codeClearTicker.detach();
#endif
		clearCode();

	}

}

Keyboard::~Keyboard() {}

void Keyboard::onDataWritten(const GattWriteCallbackParams *params) {

	if (params->handle == controlPoint.getValueAttribute().getHandle()) {

		return;

	}
}

void Keyboard::setState(Keyboard::State setTo)
{

	current_state = setTo;
	NANOK_DEBUG_OUTPUT("St set to:");
	NANOK_DEBUG_OUTPUTLN((int)setTo);


}

void Keyboard::dataSent(unsigned int count)
{


	switch (current_state)
	{
	case Keyboard::KBD_IDLE:
		// little weird, but nothing to do.
		NANOK_DEBUG_VERBOSE_OUTPUT('i');
		break;
	case Keyboard::KBD_SENDING_KEYCODE:
		/* fall-through */
	case Keyboard::KBD_SEND_REFACTORY:
		NANOK_DEBUG_VERBOSE_OUTPUT('s');
#ifndef NANOKEYS_KBD_USE_TICKER_TO_CLEAR
		clearCode();
#else

		// don't call clearCode() here in this callback... instead we set our state,
		// and let the ticker handle it
#endif
		setState(Keyboard::KBD_CLEARING_KEYCODE);
		break;

	case Keyboard::KBD_CLEARING_KEYCODE:
		/* fall-through */
	case Keyboard::KBD_CLEAR_REFACTORY:
		NANOK_DEBUG_VERBOSE_OUTPUT('R');
		// ok, all steps for char are done.
		setState(Keyboard::KBD_IDLE);
		break;

	}
	return;


}

bool Keyboard::isReady() {
	if (current_state == KBD_IDLE)
		return true;

	if (! ble.gap().getState().connected )
	{
		// we lost the conn while doing something...
		// fugget about it
		setState(KBD_IDLE);
		return true;
	}

	// seems we're not idle but connected...
	// either we're in the process of sending/clearing, or
	// something odd has happenned on the connection side, e.g.
	// we started send while pairing was still in process...
	// check for timeout
	if (millis() - last_send_time_ms > NANOKEYS_HUNGSEND_TIMEOUT_MS)
	{

		NANOK_DEBUG_OUTPUTLN(F("Hung send? Resetting"));
		setState(KBD_IDLE);
		return true;
	}


	return false;
}

bool Keyboard::send(uint8_t c)
{
	if (c > 127) {
		NANOK_DEBUG_OUTPUT("Bad char");
		return false;
	}



	if ( ! ble.gap().getState().connected )
	{

		NANOK_DEBUG_VERBOSE_OUTPUT("!Cn");
		return true;
	}


#ifdef NANOKEYS_SENDS_FORCE_WAITFORREADY
	if (! isReady()) {
		NANOK_DEBUG_VERBOSE_OUTPUT("!Rdy");
		return false;

	}
#endif


	uint8_t val = asciiMap[c];
	if (val & SHIFT)
	{
		valueBytes.theBytes[0] = KEY_CODE_MOD_LEFT_SHIFT;

		val &= 0x7f; // remove the shift
	} else {

		valueBytes.theBytes[0] = 0;
	}

	valueBytes.theBytes[1] = val;

	NANOK_DEBUG_VERBOSE_OUTPUT("Sending '");
	NANOK_DEBUG_VERBOSE_OUTPUT((char)c);
	NANOK_DEBUG_VERBOSE_OUTPUT("' (key code 0x");
	NANOK_DEBUG_VERBOSE_OUTPUT((int)val, HEX);
	NANOK_DEBUG_VERBOSE_OUTPUTLN(')');


	last_send_time_ms = millis();

#ifndef NANOKEY_USE_CLEARCODE_DELAY_MS

	setState(Keyboard::KBD_SENDING_KEYCODE);
#endif


#ifdef NANOKEYS_KBD_USE_TICKER_TO_CLEAR
	codeClearTicker.attach_us(this, &Keyboard::keyUp, 30000);
#endif

	ble.gattServer().write(report.getValueHandle(), valueBytes.getPointer(),
			valueBytes.numBytes());


#ifdef NANOKEY_USE_CLEARCODE_DELAY_MS
	delay(5);
	ble.waitForEvent();
	delay(NANOKEY_USE_CLEARCODE_DELAY_MS);
	clearCode();

#endif


	return true;

}

void Keyboard::clearCode() {

	valueBytes.clear();


	ble.gattServer().write(report.getValueHandle(), valueBytes.getPointer(),
			valueBytes.numBytes());


#ifdef NANOKEY_USE_CLEARCODE_DELAY_MS
	delay(5);
	ble.waitForEvent();
	delay(NANOKEY_USE_CLEARCODE_DELAY_MS);
#endif

}

} /* namespace Service */
} /* namespace NanoKeys */


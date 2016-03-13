/*
 * autokeys.ino
 *
 *  Created on: Feb 16, 2016
 *      Author: Pat Deegan
 *      Part of the NanoKeys Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 *
 * http://flyingcarsandstuff.com/projects/NanoKeys/
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
 * ******************* USAGE/INSTRUCTIONS ***********************
 *
 * This is a little sample to demonstrate using the BLE Nano
 * (http://redbearlab.com/blenano/) as a Human Interface
 * Device (HID) keyboard, over Bluetooth Low Energy.
 *
 * The code itself does nothing other than occasionally repeat
 * some test strings.
 *
 * To use:
 *   * build and install on a BLE Nano device
 *
 *   * get a mobile device, open a notepad-type app to see
 *     the eventual text arrive.
 *
 *   * go to bluetooth settings, scan for and pair with the
 *     device, to set it up as a wireless keyboard.
 *
 *   * hurry up and go to the note entry app you previously
 *     openned before the phone gets weird because of the
 *     incoming text :)
 *
 * For troubleshooting and more info, see the project page at
 * http://flyingcarsandstuff.com/projects/NanoKeys/
 *
 */

// we need to include the BLE lib and NanoKeys
#include <BLE_API.h>
#include <NanoKeys.h>


// **** CONFIG ***
// DEVICE_NAME -- will be advertised.
// Note, there's stuff in the packet already, so
// don't try to make this too long...
#define DEVICE_NAME						"Auto"

// DELAY_BETWEEN_SENDS_MS -- millis between
// auto "typing" of text
#define DELAY_BETWEEN_SENDS_MS			5000ul

// PERIODIC_WAKEUP_INTERVAL_MS -- our wake-ups.
// NOTE: Too large: slow.  Too short: excessive
// power consumption.
// 50ms works well.
#define PERIODIC_WAKEUP_INTERVAL_MS		50





// DEBUG_SERIAL -- uncomment/define this to output a
// little debug info through the USB serial.
// You can run this program while the BLE Nano is
// on the MK20 and plugged into the computer, so you
// can use the serial monitor to see the output.

// #define DEBUG_SERIAL    Serial

// DEBUG_SERIAL_BAUDRATE -- only applies if DEBUG_SERIAL defined
#define DEBUG_SERIAL_BAUDRATE		115200


// We declare our keyboard, as a global.
// It is used in much the same way as good ol' Serial.
NanoKeys::Keyboard TheKeyboard;


// We use a ticker, to ensure we'll wake-up regularly
// and actually output stuff
Ticker ticker_task;




// defines for the debugging output on the serial port.
#ifdef DEBUG_SERIAL
#define DEBUG_INIT(baud)    DEBUG_SERIAL.begin(baud)
#define DEBUG_OUTPUT(...)   DEBUG_SERIAL.print(__VA_ARGS__)
#define DEBUG_OUTPUTLN(...) DEBUG_SERIAL.println(__VA_ARGS__)
#else
#define DEBUG_INIT(baud)    
#define DEBUG_OUTPUT(...)   
#define DEBUG_OUTPUTLN(...) 
#endif


// periodicWakeup -- does nothing but wakeup.
void periodicWakeup() {
	DEBUG_OUTPUT('.');
}


// setup -- begin the debug serial, setup our keyboard etc
void setup() {

	DEBUG_INIT(DEBUG_SERIAL_BAUDRATE);

	// NanoKeys::Manager handles "meta" stuff, like setup and waiting for events.
	// It's a singleton.
	NanoKeys::Manager::getInstance()->begin(DEVICE_NAME);

	// also attach the ticker, so we'll wake up once in a while
	ticker_task.attach_us(periodicWakeup, PERIODIC_WAKEUP_INTERVAL_MS * 1000ul);
}


// globas used to track state in main loop
uint32_t nextEmit = 0;
int emitCount = 0;

// main loop
void loop() {

	// check if it's time to print...
	if (millis() > nextEmit) {

		// yep, it is.
		DEBUG_OUTPUTLN(F("sending magic string!"));

		// The keyboard is used just like Serial, so most things that
		// work with Serial, namely print/println(), will work.
		if (emitCount % 10 == 0)
		{
			TheKeyboard.print("We luv "); // string
			TheKeyboard.println(3.1415926f); // float
		} else if (emitCount % 5 == 0) {

			TheKeyboard.println(F("The quick brown fox jumps over the lazy dog"));
		}

		// printing integers and strings
		TheKeyboard.print(emitCount++); // int
		TheKeyboard.println(": Booya!");


		nextEmit = millis() + DELAY_BETWEEN_SENDS_MS;


	}

	// this is where we sleep and check for new BLE stuff, so DO call it.
	NanoKeys::Manager::getInstance()->waitForEvent();

}

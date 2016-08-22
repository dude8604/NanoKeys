/*
 * blenano_keyboard.ino
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
 * ******************* USAGE/INSTRUCTIONS ***********************
 *
 * This is a little sample to demonstrate using the BLE Nano
 * (http://redbearlab.com/blenano/) as a Human Interface
 * Device (HID) keyboard, over Bluetooth Low Energy.
 *
 * The code presses different keys in response to different commands
 * sent via the serial port.
 *
 * To use:
 *   * build and install on a BLE Nano device
 *
 *   * go to bluetooth settings, scan for and pair with the
 *     device, to set it up as a wireless keyboard.
 *
 *   * Try sending different commands and watch it act like 
 *     a keyboard.
 *
 * For troubleshooting and more info, see the original project page at
 * http://flyingcarsandstuff.com/projects/NanoKeys/
 *
 */

// we need to include the BLE lib and NanoKeys
#include <BLE_API.h>
#include <NanoKeys.h>

//for now, copying keycode defines from Keyboard.cpp
#define KEY_CODE_T          0x17

#define KEY_CODE_ENTER           0x28
#define KEY_CODE_ARROW_RIGHT    0x4F
#define KEY_CODE_ARROW_LEFT     0x50
#define KEY_CODE_ARROW_DOWN     0x51
#define KEY_CODE_ARROW_UP     0x52

// **** CONFIG ***
// DEVICE_NAME -- will be advertised.
// Note, there's stuff in the packet already, so
// don't try to make this too long...
// Limit seems to be 4 characters
#define DEVICE_NAME						"Wave" 

// PERIODIC_WAKEUP_INTERVAL_MS -- our wake-ups.
// NOTE: Too large: slow.  Too short: excessive
// power consumption.
// 50ms works well.
#define PERIODIC_WAKEUP_INTERVAL_MS		50

// We declare our keyboard, as a global.
// It is used in much the same way as good ol' Serial.
NanoKeys::Keyboard TheKeyboard;

// We use a ticker, to ensure we'll wake-up regularly
// and actually output stuff
Ticker ticker_task;

// periodicWakeup -- does nothing but wakeup.
void periodicWakeup() {

}

// setup -- begin the debug serial, setup our keyboard etc
void setup() {

  Serial.begin(9600);

  // NanoKeys::Manager handles "meta" stuff, like setup and waiting for events.
  // It's a singleton.
  NanoKeys::Manager::getInstance()->begin(DEVICE_NAME);

  ticker_task.attach_us(periodicWakeup, PERIODIC_WAKEUP_INTERVAL_MS * 1000ul);
}


// main loop
void loop() {

  if (Serial.available()) {
    char dir = Serial.read(); //get input from serial

    switch (dir) {
      case 'u': //if the character 'u' was sent
        TheKeyboard.pressKey(KEY_CODE_ARROW_UP); //press up arrow key
        break;
      case 'l': //if the character 'l' was sent
        TheKeyboard.pressKey(KEY_CODE_ARROW_LEFT); //press left arrow key
        break;
      case 'd': //if the character 'd' was sent
        TheKeyboard.pressKey(KEY_CODE_ARROW_DOWN); //press down arrow key
        break;
      case 'r': //if the character 'r' was sent
        TheKeyboard.pressKey(KEY_CODE_ARROW_RIGHT); //press right arrow key
        break;
      case 'e': //if the character 'e' was sent
        TheKeyboard.pressKey(KEY_CODE_ENTER); //press enter key
        break;
      case 't' //if the character 't' was sent
        TheKeyboard.pressKey(KEY_CODE_T); //press T key so we can see text being written
        break;
    }

    delay(500); //hold key down for 500ms
    TheKeyboard.releaseKey(); //release the key
  }

  // this is where we sleep and check for new BLE stuff, so DO call it.
  NanoKeys::Manager::getInstance()->waitForEvent();

}

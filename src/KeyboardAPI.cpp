/*
 * KeyboardAPI.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Pat Deegan
 *      Part of the NanoKeys Project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
 *      
 * http://flyingcarsandstuff.com/projects/NanoKeys/
 *
 *
 * 	Modified on: Aug 21, 2016
 * 		Author: Philip Weiss (dude8604)
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

#include "includes/KeyboardAPI.h"
#include "includes/Manager.h"
#include "includes/NanoKeysDebug.h"


#ifdef NANOKEYS_SENDS_FORCE_WAITFORREADY
#define NANOKEYS_KEYBOARDAPI_WAITFORREADY
#endif

namespace NanoKeys {

Keyboard::Keyboard() {


}

Keyboard::~Keyboard() {}

size_t Keyboard::write(uint8_t c)
{
	Manager * man = Manager::getInstance();

	Service::Keyboard * kdbServ = man->keyboardService();

#ifdef NANOKEYS_KEYBOARDAPI_WAITFORREADY

	uint8_t waitCount = 0; // just a sanity check, so we don't get stuck
	while ( (! kdbServ->isReady()) && waitCount++ < 150)
	{
		man->ble()->waitForEvent();
	}

#endif

	if (kdbServ->send(c)) {
#ifndef NANOKEYS_KEYBOARDAPI_WAITFORREADY
		man->ble()->waitForEvent();
		man->ble()->waitForEvent();
#endif

		NANOK_DEBUG_OUTPUT("!");
		return 1;
	}
	NANOK_DEBUG_OUTPUT("~");

	return 0;
}

/* PTW */

bool Keyboard::pressKey(uint8_t keyCode) {
	
	Manager * man = Manager::getInstance();

	return man->keyboardService()->keyDown(keyCode);

	
}

void Keyboard::releaseKey() {
	
	Manager * man = Manager::getInstance();

	man->keyboardService()->keyUp();
	
}

/* end PTW */

} /* namespace NanoKeys */

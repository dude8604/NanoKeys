/*
 * Manager.h
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

#ifndef NANOKEYS_SRC_INCLUDES_MANAGER_H_
#define NANOKEYS_SRC_INCLUDES_MANAGER_H_
#include "NanoKeysIncludes.h"
#include "services/Battery.h"
#include "services/DeviceInfo.h"
#include "services/Keyboard.h"

namespace NanoKeys {

/*
 * Manager class
 *
 * The Manager is your interface to the BLE instance, and is used to handle
 * the "meta" stuff--namely, doing the setup through it's begin() call.
 *
 * Otherwise, it's main use is to provide a few utility methods like
 * isConnected() and waitForEvent().
 *
 */

class Manager {
public:

	typedef void(*DisconnCallback)(Gap::Handle_t handle, Gap::DisconnectionReason_t reason);
	static Manager * getInstance(BLE * ble=NULL);


	/*
	 * begin DEVICE_NAME [AUTOADVERT]
	 *
	 * Init and setup, should be called in your setup() function.
	 *
	 * @param DEVICE_NAME: name to advertise, const char *
	 * @param AUTOADVERT: optional boolean for setting up advert contents, defaults to true.
	 *
	 * If you pass false for AUTOADVERT, you are responsible for setting up the
	 * advert-related stuff--so, the type, payload, interval etc and must actually
	 * startAdvertising() at some point too.
	 */
	bool begin(const char * deviceName, bool autoAdvert=true);



	/*
	 * ble
	 *
	 * @return: Returns a pointer to the BLE instance
	 */
	inline BLE * ble() { return theBLE;}

	/*
	 * setOnDisconnection CALLBACK
	 *
	 * @param CALLBACK: a BLE-compatible disconnection callback.
	 *
	 * Set your own disconnection callback.  If not used, a default will be
	 * set that re-enables advertising.
	 */
	void setOnDisconnection(DisconnCallback setTo);

	/*
	 * isConnected
	 *
	 * Tell you whether a client is currently connected.
	 *
	 * @return: boolean
	 *
	 */
	inline bool isConnected() { return theBLE->gap().getState().connected ;}

	/*
	 * waitForEvent
	 *
	 * Shortcut for Manager::getInstance()->ble()->waitForEvent()
	 *
	 */
	inline void waitForEvent() { theBLE->waitForEvent();}



	inline Service::DeviceInfo * devInfoService() { return devInfo;}
	inline Service::Battery	* batteryService() { return battery;}
	inline Service::Keyboard   * keyboardService() { return keyboard;}


private:

	Manager(BLE * ble=NULL);
	static Manager * _manager_singleton;

	BLE * theBLE;

	Service::DeviceInfo * devInfo;
	Service::Battery	* battery;
	Service::Keyboard   * keyboard;

	bool setup_complete;
	DisconnCallback 	on_disconn_cb;





};

} /* namespace NanoKeys */

#endif /* NANOKEYS_SRC_INCLUDES_MANAGER_H_ */

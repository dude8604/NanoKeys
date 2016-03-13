/*
 * Manager.cpp
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

#include "includes/Manager.h"
#include "includes/NanoKeysConfig.h"

namespace NanoKeys {
Manager * Manager::_manager_singleton = NULL;



static const uint16_t uuid16_list[] = {GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE,
		GattService::UUID_DEVICE_INFORMATION_SERVICE,
		GattService::UUID_BATTERY_SERVICE};




static void defaultDisconnCb(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{

    Manager::getInstance()->ble()->startAdvertising();
}

Manager * Manager::getInstance(BLE * ble)
{
	if (_manager_singleton)
		return _manager_singleton;

	_manager_singleton = new Manager(ble);

	return _manager_singleton;
}
Manager::Manager(BLE * blePtr) : theBLE(blePtr),
		devInfo(NULL),
				battery(NULL),
				keyboard(NULL),
		setup_complete(false),
		on_disconn_cb(NULL)

{


}


void Manager::setOnDisconnection(DisconnCallback setTo)
{
	on_disconn_cb = setTo;

	if (theBLE)
	{
		theBLE->onDisconnection(on_disconn_cb);
	}

}

bool Manager::begin(const char * deviceName, bool autoAdvert)
{
	if (setup_complete)
		return false;

	setup_complete = true;

	// in the BLE instance
	if (! theBLE)
	{
		theBLE = new BLE();

		if (! theBLE)
		{
			return false;
		}
		delay(200);
	}

    theBLE->init();


    // a ref to the ble, for convenience
    BLE & device(*theBLE);


    // our services
    devInfo = new Service::DeviceInfo(device,
    					"psychogenic.com",
    					NANOKEYS_MODELNUMBER_STR,
    					NANOKEYS_SERIALNUMBER_STR,
    					NANOKEYS_HARDWARE_REVISION_STR,
    					NANOKEYS_FIRMWARE_REVISION_STR,
    					NANOKEYS_SOFTWARE_REVISION_STR,
    					Service::PNPDetails(1, 0x1915, 0xEEEE, 1)
    				);

    battery = new Service::Battery(device);
    keyboard = new Service::Keyboard(device);

    if (! (devInfo && battery && keyboard))
    {
    	return false;
    }


    // if we have a disconn cb, set it,
    // otherwise, use default
    if (on_disconn_cb)
    {

        theBLE->onDisconnection(on_disconn_cb);
    } else {
    	theBLE->onDisconnection(defaultDisconnCb);
    }

    // set device name
    theBLE->gap().setDeviceName((const uint8_t *)deviceName);
    // set tx power,valid values are -40, -20, -16, -12, -8, -4, 0, 4
    theBLE->gap().setTxPower(4);


	if (autoAdvert) {
		// setup adv_data and srp_data
		theBLE->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
		theBLE->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t*)uuid16_list, sizeof(uuid16_list));
		theBLE->accumulateAdvertisingPayload(GapAdvertisingData::KEYBOARD);
		theBLE->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)deviceName, sizeof(deviceName));

		// set adv_type
		theBLE->setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);


		// set adv_interval, in ms
		theBLE->gap().setAdvertisingInterval(100);
		// set adv_timeout, in seconds
		theBLE->setAdvertisingTimeout(0);
		// start advertising
		theBLE->startAdvertising();
    }

	theBLE->securityManager().init(true, true, SecurityManager::IO_CAPS_NONE); // IO_CAPS_KEYBOARD_ONLY) ; // ... passkey );

    return true;

}

} /* namespace NanoKeys */

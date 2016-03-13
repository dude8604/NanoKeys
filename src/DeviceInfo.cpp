/*
 * DeviceInfo.cpp
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

#include "includes/services/DeviceInfo.h"

namespace NanoKeys {
namespace Service {


DeviceInfo::DeviceInfo(BLE &_ble,
                         const char *manufacturersName ,
                         const char *modelNumber       ,
                         const char *serialNumber      ,
                         const char *hardwareRevision  ,
                         const char *firmwareRevision  ,
                         const char *softwareRevision  ,
                         PNPDetails pnp) :
    ble(_ble),
    manufacturersNameStringCharacteristic(GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR,
                                          (uint8_t *)manufacturersName,
                                          (manufacturersName != NULL) ? strlen(manufacturersName) : 0, /* minLength */
                                          (manufacturersName != NULL) ? strlen(manufacturersName) : 0, /* maxLength */
                                          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    modelNumberStringCharacteristic(GattCharacteristic::UUID_MODEL_NUMBER_STRING_CHAR,
                                    (uint8_t *)modelNumber,
                                    (modelNumber != NULL) ? strlen(modelNumber) : 0, /* minLength */
                                    (modelNumber != NULL) ? strlen(modelNumber) : 0, /* maxLength */
                                    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    serialNumberStringCharacteristic(GattCharacteristic::UUID_SERIAL_NUMBER_STRING_CHAR,
                                     (uint8_t *)serialNumber,
                                     (serialNumber != NULL) ? strlen(serialNumber) : 0, /* minLength */
                                     (serialNumber != NULL) ? strlen(serialNumber) : 0, /* maxLength */
                                     GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    hardwareRevisionStringCharacteristic(GattCharacteristic::UUID_HARDWARE_REVISION_STRING_CHAR,
                                         (uint8_t *)hardwareRevision,
                                         (hardwareRevision != NULL) ? strlen(hardwareRevision) : 0, /* minLength */
                                         (hardwareRevision != NULL) ? strlen(hardwareRevision) : 0, /* maxLength */
                                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    firmwareRevisionStringCharacteristic(GattCharacteristic::UUID_FIRMWARE_REVISION_STRING_CHAR,
                                         (uint8_t *)firmwareRevision,
                                         (firmwareRevision != NULL) ? strlen(firmwareRevision) : 0, /* minLength */
                                         (firmwareRevision != NULL) ? strlen(firmwareRevision) : 0, /* maxLength */
                                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),

    softwareRevisionStringCharacteristic(GattCharacteristic::UUID_SOFTWARE_REVISION_STRING_CHAR,
                                         (uint8_t *)softwareRevision,
                                         (softwareRevision != NULL) ? strlen(softwareRevision) : 0, /* minLength */
                                         (softwareRevision != NULL) ? strlen(softwareRevision) : 0, /* maxLength */
                                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    _pnpDets(pnp),
    pnpDetailsCharacteristic(GattCharacteristic::UUID_PNP_ID_CHAR,
                              (uint8_t*)&(_pnpDets),
                              _pnpDets.isSetup() ? sizeof(_pnpDets) : 0,
                              _pnpDets.isSetup() ? sizeof(_pnpDets) : 0,
                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)



{
    static bool serviceAdded = false; /* We should only ever need to add the heart rate service once. */
    if (serviceAdded) {
        return;
    }

    GattCharacteristic *charTable[] = {&manufacturersNameStringCharacteristic,
                                       &modelNumberStringCharacteristic,
                                       &serialNumberStringCharacteristic,
                                       &hardwareRevisionStringCharacteristic,
                                       &firmwareRevisionStringCharacteristic,
                                       &softwareRevisionStringCharacteristic,
                                       &pnpDetailsCharacteristic};
    GattService         deviceInformationService(GattService::UUID_DEVICE_INFORMATION_SERVICE, charTable,
                                                 sizeof(charTable) / sizeof(GattCharacteristic *));

    ble.addService(deviceInformationService);
    serviceAdded = true;
}

DeviceInfo::~DeviceInfo()
{

}
} /* namespace Service */
} /* namespace NanoKeys */

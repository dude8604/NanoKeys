/*
 * DeviceInfo.h
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

#ifndef NANOKEYS_SRC_INCLUDES_SERVICES_DEVICEINFO_H_
#define NANOKEYS_SRC_INCLUDES_SERVICES_DEVICEINFO_H_

#include "../NanoKeysIncludes.h"
namespace NanoKeys {
namespace Service {



typedef struct  __attribute__ ((__packed__)) PNPDetailsStruct{
  uint8_t assigned_by;
  uint16_t vendor_id;
  uint16_t product_id;
  uint16_t product_version;

  PNPDetailsStruct() : assigned_by(255) {}
  PNPDetailsStruct(uint8_t pnpType, uint16_t companyId, uint16_t prodId, uint16_t vers) :
    assigned_by(pnpType), vendor_id(companyId), product_id(prodId), product_version(vers)
    {

    }

  bool isSetup() {
    return true;
  }



} PNPDetails;


class DeviceInfo {
public:

    DeviceInfo(BLE &_ble,
                             const char *manufacturersName = NULL,
                             const char *modelNumber       = NULL,
                             const char *serialNumber      = NULL,
                             const char *hardwareRevision  = NULL,
                             const char *firmwareRevision  = NULL,
                             const char *softwareRevision  = NULL,
                             PNPDetails pnp=PNPDetails()) ;
    virtual ~DeviceInfo();

protected:
    BLE                &ble;
    GattCharacteristic  manufacturersNameStringCharacteristic;
    GattCharacteristic  modelNumberStringCharacteristic;
    GattCharacteristic  serialNumberStringCharacteristic;
    GattCharacteristic  hardwareRevisionStringCharacteristic;
    GattCharacteristic  firmwareRevisionStringCharacteristic;

    PNPDetails _pnpDets;

    GattCharacteristic  softwareRevisionStringCharacteristic;

    GattCharacteristic  pnpDetailsCharacteristic;
};



} /* namespace Service */
} /* namespace NanoKeys */

#endif /* NANOKEYS_SRC_INCLUDES_SERVICES_DEVICEINFO_H_ */

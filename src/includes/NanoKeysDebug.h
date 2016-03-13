/*
 * NanoKeysDebug.h
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

#ifndef NANOKEYS_SRC_INCLUDES_NANOKEYSNANOK_DEBUG_H_
#define NANOKEYS_SRC_INCLUDES_NANOKEYSNANOK_DEBUG_H_


// #define NANOK_DEBUG_SERIAL		Serial
//#define NANOK_DEBUG_VERBOSE_ENABLE

#ifdef NANOK_DEBUG_SERIAL
#define NANOK_DEBUG_OUTPUT(...)   NANOK_DEBUG_SERIAL.print(__VA_ARGS__)
#define NANOK_DEBUG_OUTPUTLN(...) NANOK_DEBUG_SERIAL.println(__VA_ARGS__)
#else
#define NANOK_DEBUG_OUTPUT(...)
#define NANOK_DEBUG_OUTPUTLN(...)
#endif

#ifdef NANOK_DEBUG_VERBOSE_ENABLE
#define NANOK_DEBUG_VERBOSE_OUTPUT(...)			NANOK_DEBUG_OUTPUT(__VA_ARGS__)
#define NANOK_DEBUG_VERBOSE_OUTPUTLN(...)			NANOK_DEBUG_OUTPUTLN(__VA_ARGS__)
#else
#define NANOK_DEBUG_VERBOSE_OUTPUT(...)
#define NANOK_DEBUG_VERBOSE_OUTPUTLN(...)
#endif



#endif /* NANOKEYS_SRC_INCLUDES_NANOKEYSNANOK_DEBUG_H_ */

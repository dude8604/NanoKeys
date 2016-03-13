/*
 * KeyboardAPI.h
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

#ifndef NANOKEYS_SRC_INCLUDES_KEYBOARDAPI_H_
#define NANOKEYS_SRC_INCLUDES_KEYBOARDAPI_H_

#include "NanoKeysIncludes.h"

namespace NanoKeys {

/*
 * The NanoKeys::Keyboard is the main user interface (along
 * with NanoKeys::Manager) used by library client code.
 *
 * The Keyboard is just a standard Arduino Print interface
 * implementation, meaning that everything supported by Print
 * (like printing strings and numbers in user-selected bases)
 * will just work, with one caveat:
 *
 *  * only values between [0, 127] -- i.e. good ol ascii -- are actually
 *    mapped by the underlying service.
 *
 * For the most part, use the Keyboard instance like you would use
 * Serial, for writing.
 *
 *
 */
class Keyboard : public Print {
public:
	Keyboard();
	virtual ~Keyboard();

	virtual size_t write(uint8_t c);

};

} /* namespace NanoKeys */

#endif /* NANOKEYS_SRC_INCLUDES_KEYBOARDAPI_H_ */

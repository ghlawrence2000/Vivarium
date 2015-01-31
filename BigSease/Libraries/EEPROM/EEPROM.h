/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/eeprom.h>
#include <inttypes.h>

class EEPROMClass
{
public:
	uint8_t read (uint32_t);
	uint8_t readByte (uint32_t);
	uint16_t readWord (uint32_t);
	uint32_t readDWord (uint32_t);
	uint64_t readQWord (uint32_t);
	float readFloat (uint32_t);
	double readDouble (uint32_t);
	template <class T> T readAll (uint32_t, T &);

	uint8_t write (uint32_t, uint8_t);
	uint8_t writeByte (uint32_t, uint8_t);
	uint8_t writeWord (uint32_t, uint16_t);
	uint8_t writeDWord (uint32_t, uint32_t);
	uint8_t writeQWord (uint32_t, uint64_t);
	uint8_t writeFloat (uint32_t, float);
	uint8_t writeDouble (uint32_t, double);
	template <class T> T writeAll (uint32_t, const T &);
};

extern EEPROMClass EEPROM;

#endif


/*
 * EEPROM.cpp - EEPROM library
 * Copyright (c) 2006 David A. Mellis.  All right reserved.
 * Copyright (c) 2013 Roger A. Krupski.
 *
 * Free software - use, modify, distribute freely.
 */

#include "EEPROM.h"

uint8_t EEPROMClass::read (uint32_t address)
{
	uint8_t value;
	return EEPROM.readAll (address, value);
}

uint8_t EEPROMClass::readByte (uint32_t address)
{
	uint8_t value;
	return EEPROM.readAll (address, value);
}

uint16_t EEPROMClass::readWord (uint32_t address)
{
	uint16_t value;
	return EEPROM.readAll (address, value);
}

uint32_t EEPROMClass::readDWord (uint32_t address)
{
	uint32_t value;
	return EEPROM.readAll (address, value);
}

uint64_t EEPROMClass::readQWord (uint32_t address)
{
	uint64_t value;
	return EEPROM.readAll (address, value);
}

float EEPROMClass::readFloat (uint32_t address)
{
	float value;
	return EEPROM.readAll (address, value);
}

double EEPROMClass::readDouble (uint32_t address)
{
	double value;
	return EEPROM.readAll (address, value);
}

template <class T> T EEPROMClass::readAll (uint32_t address, T &value)
{
	uint8_t *ptr = (uint8_t *) (void *) &value;
	uint8_t x;

	for (x = 0; x < sizeof (value); x++) {
		*ptr++ = eeprom_read_byte ( (uint8_t *) address++);
	}

	return value;
}

uint8_t EEPROMClass::write (uint32_t address, uint8_t value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeByte (uint32_t address, uint8_t value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeWord (uint32_t address, uint16_t value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeDWord (uint32_t address, uint32_t value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeQWord (uint32_t address, uint64_t value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeFloat (uint32_t address, float value)
{
	return EEPROM.writeAll (address, value);
}

uint8_t EEPROMClass::writeDouble (uint32_t address, double value)
{
	return EEPROM.writeAll (address, value);
}

template <class T> T EEPROMClass::writeAll (uint32_t address, const T &value)
{
	const uint8_t *ptr = (const uint8_t *) (const void *) &value;
	uint8_t x;

	for (x = 0; x < sizeof (value); x++) {
		eeprom_write_byte ( (uint8_t *) address++, *ptr++);
	}

	return sizeof (value);
}

EEPROMClass EEPROM;

/*
* Copyright (C) 2012 Southern Storm Software, Pty Ltd.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#include "EEPROM24.h"
#include "I2CMaster.h"

EEPROM24::EEPROM24(I2CMaster &bus, unsigned long type, uint8_t bank)
    : _bus(&bus)
    , _size((type & 0xFFFF) * ((type >> 16) & 0x0FFF))
    , _pageSize((type >> 16) & 0x0FFF)
    , _mode((uint8_t)((type >> 28) & 0x0F))
    , i2cAddress(0x50)
{
    // Adjust the I2C address for the memory bank of the chip.
    switch (_mode) {
    case EE_BSEL_NONE:
        i2cAddress += (bank & 0x07);
        break;
    case EE_BSEL_8BIT_ADDR: {
        uint8_t addrBits = 8;
        unsigned long size = 0x0100;
        while (size < _size) {
            ++addrBits;
            size <<= 1;
        }
        if (addrBits < 11)
            i2cAddress += ((bank << (addrBits - 8)) & 0x07);
        break; }
    case EE_BSEL_17BIT_ADDR:
        i2cAddress += ((bank << 1) & 0x06);
        break;
    case EE_BSEL_17BIT_ADDR_ALT:
        i2cAddress += bank & 0x03;
        break;
    }
}

bool EEPROM24::available()
{
    // Perform a "Current Address Read" on the EEPROM.  We don't care about
    // the returned byte.  We only care if the read request was ACK'ed or not.
    if (!_bus->startRead(i2cAddress, 1))
        return false;
    _bus->read();
    return true;
}

uint8_t EEPROM24::read(unsigned long address)
{
    if (address >= _size)
        return 0;
    writeAddress(address);
    if (!_bus->startRead(i2cAddress, 1))
        return 0;
    return _bus->read();
}

size_t EEPROM24::read(unsigned long address, void *data, size_t length)
{
    if (address >= _size || !length)
        return 0;
    if ((address + length) > _size)
        length = (size_t)(_size - address);
    writeAddress(address);
    if (!_bus->startRead(i2cAddress, length))
        return 0;
    uint8_t *d = (uint8_t *)data;
    unsigned int count = 0;
    while (_bus->available()) {
        *d++ = _bus->read();
        ++count;
    }
    return count;
}

bool EEPROM24::write(unsigned long address, uint8_t value)
{
    if (address >= _size)
        return false;
    writeAddress(address);
    _bus->write(value);
    return waitForWrite();
}

size_t EEPROM24::write(unsigned long address, const void *data, size_t length)
{
    if (address >= _size)
        return 0;
    if ((address + length) > _size)
        length = (size_t)(_size - address);
    bool needAddress = true;
    size_t result = 0;
    size_t page = 0;
    const uint8_t *d = (const uint8_t *)data;
    while (length > 0) {
        if (needAddress) {
            writeAddress(address);
            needAddress = false;
        }
        _bus->write(*d++);
        ++address;
        ++page;
        if ((address & (_pageSize - 1)) == 0) {
            // At the end of a page, so perform a flush.
            if (!waitForWrite())
                return result;  // Could not write this page.
            needAddress = true;
            result += page;
            page = 0;
        }
        --length;
    }
    if (!needAddress) {
        if (!waitForWrite())
            return result;  // Could not write the final page.
    }
    return result + page;
}

void EEPROM24::writeAddress(unsigned long address)
{
    switch (_mode) {
    case EE_BSEL_NONE:
        _bus->startWrite(i2cAddress);
        _bus->write((uint8_t)(address >> 8));
        _bus->write((uint8_t)address);
        break;
    case EE_BSEL_8BIT_ADDR:
        _bus->startWrite(i2cAddress | (((uint8_t)(address >> 8)) & 0x07));
        _bus->write((uint8_t)address);
        break;
    case EE_BSEL_17BIT_ADDR:
        _bus->startWrite(i2cAddress | (((uint8_t)(address >> 16)) & 0x01));
        _bus->write((uint8_t)(address >> 8));
        _bus->write((uint8_t)address);
        break;
    case EE_BSEL_17BIT_ADDR_ALT:
        _bus->startWrite(i2cAddress | (((uint8_t)(address >> 14)) & 0x04));
        _bus->write((uint8_t)(address >> 8));
        _bus->write((uint8_t)address);
        break;
    }
}

bool EEPROM24::waitForWrite()
{
    // 1000 iterations is going to be approximately 100ms when the I2C
    // clock is 100 kHz.  If there has been no response in that time
    // then we assume that the write has failed and timeout.
    if (!_bus->endWrite())
        return false;
    unsigned count = 1000;
    while (count > 0) {
        _bus->startWrite(i2cAddress);
        if (_bus->endWrite())
            return true;
        --count;
    }
    return false;
}


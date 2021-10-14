//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006, 2007 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
// See http://www.joegeluso.com/software/articles/ccitt.htm for a great
// write-up on the CRC16 calculation.
//

#ifndef INCLUDE_CRC16_H
#define INCLUDE_CRC16_H


#include <stddef.h>

/**
  * The crc16 class is used to represent the running value of a 16-bit
  * cyclic redundancy check of series of bytes.
  */
class crc16
{
public:
    /**
      * The destructor.
      */
    virtual ~crc16();

    /**
      * The default constructor.
      *
      * @param ccitt
      *     This is true for a CCITT calculation, or false for the
      *     XModem calculation.
      * @param augment
      *     This is true if the 16-zero-bit augmentation is desired.
      *     This is the default.  False if no augmentation is desired.
      */
    crc16(bool ccitt = true, bool augment = true);

    /**
      * The copy constructor.
      */
    crc16(const crc16 &);

    /**
      * The assignment operator.
      */
    crc16 &operator=(const crc16 &);

    /**
      * The get method is used to obtain the running value of the cyclic
      * redundancy check.
      */
    unsigned short get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(unsigned char);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      */
    void nextbuf(const void *, size_t);

private:
    /**
      * The state instance variable is used to remember the running
      * value of the 16-bit cyclic redundancy check.
      */
    unsigned short state;

    /**
      * The augment instance variable is used to remember whether or
      * not the 16 zero bits of augmentation are to be processed before
      * reporting the result.
      */
    bool augment;
};

#endif // INCLUDE_CRC16_H

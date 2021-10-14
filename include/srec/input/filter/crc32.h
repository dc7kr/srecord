//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2003 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for include/srec/input/filter/crc32.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_CRC32_H
#define INCLUDE_SREC_INPUT_FILTER_CRC32_H

#pragma interface "srec_input_filter_crc32"

#include <srec/input/filter.h>

class srec_input_filter_crc32:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_crc32();

    /**
      * The constructor.
      */
    srec_input_filter_crc32(srec_input *deeper, unsigned long address,
	    int order);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The address instance variable is used to remember where to place
      * the CRC in memory.
      */
    unsigned long address;

    /**
      * The order instance variable is used to remember whether the byte
      * order is big-endian (0) or little-endian (1).
      */
    int order;

    /**
      * The buffer instance variable is used to remember the contents
      * of the deeper file.  The deeper file must be read completely in
      * order to calculate the CRC, and the input may be out of address
      * order, necessitating this buffer.
      */
    class srec_memory *buffer;

    /**
      * The buffer_pos instance variable is used to remember where we
      * are up to in processing 'buffer'.
      */
    unsigned long buffer_pos;

    /**
      * The have_forwarded_header instance variable is used to remember
      * whether we have returned the file header to our reader yet.
      */
    bool have_forwarded_header;

    /**
      * The have_given_crc instance variable is used to remember
      * whether we have returned the CRC to our reader yet.
      */
    bool have_given_crc;

    /**
      * The have_forwarded_start_address instance variable is used
      * to remember whether we have returned the start address to our
      * reader yet.
      */
    bool have_forwarded_start_address;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_crc32();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_crc32(const srec_input_filter_crc32 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_crc32 &operator=(const srec_input_filter_crc32 &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_CRC32_H

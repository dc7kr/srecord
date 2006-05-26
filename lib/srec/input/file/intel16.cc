//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2005 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_intel16 class
//

#pragma implementation "srec_input_file_intel16"

#include <srec/input/file/intel16.h>
#include <srec/record.h>


srec_input_file_intel16::srec_input_file_intel16(const char *filename) :
    srec_input_file(filename),
    data_record_count(0),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false),
    mode(linear),
    address_base(0),
    pushback(0),
    end_seen(false)
{
}


srec_input_file_intel16::~srec_input_file_intel16()
{
    if (pushback)
       	delete pushback;
}


int
srec_input_file_intel16::read_inner(srec_record &record)
{
    if (pushback)
    {
	record = *pushback;
	delete pushback;
	pushback = 0;
	return 1;
    }

    //
    // keep reading lines until something returnable comes along
    //
    for (;;)
    {
	//
	// grab the first character of the line
	//
	int c = get_char();

	//
	// end of file means we are done
	//
	if (c < 0)
	    return 0;

	//
	// quietly ignore blank lines
	//
	if (c == '\n')
	    continue;

	//
	// If it doesn't start with a colon, it's a garbage line.
	// Warn, and then ignore it.
	//
	if (c != ':')
	{
	    if (!garbage_warning)
	    {
		warning("ignoring garbage lines");
		garbage_warning = true;
	    }
	    for (;;)
	    {
		c = get_char();
		if (c < 0)
		    return 0;
		if (c == '\n')
		    break;
	    }
	    continue;
	}

	//
	// Looks like a real Intel-hex line.
	//
	unsigned char buffer[255*2+5];
	checksum_reset();
	buffer[0] = get_byte();
	unsigned nbytes = buffer[0] << 1;
	buffer[1] = get_byte();
	buffer[2] = get_byte();
	buffer[3] = get_byte();
	for (unsigned j = 0; j <= nbytes; ++j)
	{
	    // Note: the bytes are HI,LO and we need the other way around
	    buffer[4 + (j ^ 1)] = get_byte();
	}
	if (use_checksums())
	{
	    int csum = checksum_get();
	    if (csum != 0x00)
		fatal_error("checksum mismatch (%02X != 00)", csum);
	}
	if (get_char() != '\n')
	    fatal_error("end-of-line expected");
    
	srec_record::address_t address_field =
	     2 * srec_record::decode_big_endian(buffer + 1, 2);
    
	srec_record::type_t type = srec_record::type_unknown;
	switch (buffer[3])
	{
	case 0:
	    //
	    // data
	    //
	    if (mode == linear)
	    {
		//
		// linear addressing model
		//
		if
		(
		    (
			(long long)address_base + address_field
		    <
			((long long)1 << 32)
		    )
		&&
		    (
			(long long)address_base + address_field + nbytes
		    >
			((long long)1 << 32)
		    )
		)
		{
		    int split =
			((long long)1 << 32) - address_base - address_field;
		    pushback =
			new srec_record
			(
			    srec_record::type_data,
			    0L,
			    buffer + 4 + split,
			    nbytes - split
			);
		    nbytes = split;
		}
	    }
	    else
	    {
		//
		// segmented addressing model
		// (Does this make sense for INHX16?)
		//
		if (address_field + nbytes > (1L << 16))
		{
		    int split = (1L << 16) - address_field;
		    pushback =
			new srec_record
			(
			    srec_record::type_data,
			    address_base + ((address_field + split) & 0xFFFF),
			    buffer + 4 + split,
			    nbytes - split
			);
		    nbytes = split;
		}
	    }
	    type = srec_record::type_data;
	    break;
    
	case 1:
	    //
	    // end-of-file record
	    //
	    if (nbytes != 0)
		fatal_error("length field must be zero");
	    if (address_field != 0)
		fatal_error("address field must be zero");
	    end_seen = true;
	    seek_to_end();
	    return 0;
    
	case 2:
	    //
	    // extended segment address record
	    //
	    // Set the base address and addressing mode,
	    // and then loop for another record, this one
	    // isn't visable to the caller.
	    //
	    // (Does this make sense for INHX16?)
	    //
	    if (nbytes != 2)
		fatal_error("length field must be 1");
	    if (address_field != 0)
		fatal_error("address field must be zero");
	    address_base = srec_record::decode_big_endian(buffer + 4, 2) << 5;
	    mode = segmented;
	    continue;
    
	case 3:
	    //
	    // start segment address record
	    //
	    if (nbytes != 4)
		fatal_error("length field must be 4");
	    if (address_field != 0)
		fatal_error("address field must be zero");
	    address_field =
		srec_record::decode_big_endian(buffer + 4, 2) * 16 +
		srec_record::decode_big_endian(buffer + 6, 2);
	    record =
		srec_record
		(
		    srec_record::type_start_address,
		    address_field << 1,
		    0,
		    0
		);
	    return 1;
    
	case 4:
	    //
	    // extended linear address record
	    //
	    // Set the base address and addressing mode,
	    // and then loop for another record, this one
	    // isn't visable to the caller.
	    //
	    // (Does this make sense for INHX16?)
	    //
	    if (nbytes != 2)
		fatal_error("length field must be 1");
	    if (address_field != 0)
		fatal_error("address field must be zero");
	    address_base = srec_record::decode_big_endian(buffer + 4, 2) << 17;
	    mode = linear;
	    continue;
    
	case 5:
	    //
	    // start linear address record
	    //
	    if (nbytes != 4)
		fatal_error("length field must be 2");
	    if (address_field != 0)
		fatal_error("address field must be zero");
	    address_field = srec_record::decode_big_endian(buffer + 4, 4);
	    record =
		srec_record
		(
		    srec_record::type_start_address,
		    address_field << 1,
		    0,
		    0
		);
	    return 1;
	}

	//
	// data record or unknown
	//
	record =
	    srec_record
	    (
		type,
		address_base + address_field,
		buffer + 4,
		nbytes
	    );
	return 1;
    }
}


int
srec_input_file_intel16::read(srec_record &record)
{
    for (;;)
    {
	if (!read_inner(record))
	{
	    if (!seen_some_input && garbage_warning)
		fatal_error("file contains no data");
	    if (data_record_count <= 0)
		fatal_error("file contains no data");
	    if (!end_seen)
	    {
		warning("no end-of-file record");
		termination_seen = true;
	    }
	    return 0;
	}
	seen_some_input = true;
	switch (record.get_type())
	{
	case srec_record::type_unknown:
	    fatal_error("record type not recognised");
	    break;

	default:
	    // impossible
	    continue;

	case srec_record::type_data:
	    ++data_record_count;
	    if (record.get_length() == 0)
	    {
		warning("empty data record ignored");
		continue;
	    }
	    break;

	case srec_record::type_start_address:
	    if (termination_seen)
		warning("redundant start address record");
	    termination_seen = true;
	    break;
	}
	break;
    }
    return 1;
}


const char *
srec_input_file_intel16::get_file_format_name()
    const
{
    return "Intel Hexadecimal 16 (INHX16)";
}
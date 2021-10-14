//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_formatted_binary class
//

#pragma implementation "srec_output_file_formatted_binary"

#include <srec/output/file/formatted_binary.h>
#include <srec/record.h>


srec_output_file_formatted_binary::~srec_output_file_formatted_binary()
{
}


srec_output_file_formatted_binary::srec_output_file_formatted_binary(
	const char *filename) :
    srec_output_file(filename),
    upper_bound(0),
    address(0),
    check_sum(0)
{
}


void
srec_output_file_formatted_binary::notify_upper_bound(unsigned long arg)
{
    upper_bound = arg;
    if (upper_bound == 0)
	fatal_error("can't write file with no data");
}


void
srec_output_file_formatted_binary::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
	// assert(upper bound != 0);
	if (upper_bound == 0)
	    fatal_error("must call srec_output::notify_upper_bound first");
	if (upper_bound < (1uL << 16))
	{
	    put_char(0x08); //     *
	    put_char(0x1C); //    ***
	    put_char(0x2A); //   * * *
	    put_char(0x49); //  *  *  *
	    put_char(0x08); //     *
	    put_char(0x00);
	    put_char((upper_bound >> 12) & 0x0F);
	    put_char((upper_bound >>  8) & 0x0F);
	    put_char((upper_bound >>  4) & 0x0F);
	    put_char( upper_bound        & 0x0F);
	}
	else
	{
	    put_char(0x08); //     *
	    put_char(0x1C); //    ***
	    put_char(0x3E); //   *****
	    put_char(0x6B); //  ** * **
	    put_char(0x08); //     *
	    put_char(0x00);
	    put_char((upper_bound >> 28) & 0x0F);
	    put_char((upper_bound >> 24) & 0x0F);
	    put_char((upper_bound >> 20) & 0x0F);
	    put_char((upper_bound >> 16) & 0x0F);
	    put_char((upper_bound >> 12) & 0x0F);
	    put_char((upper_bound >>  8) & 0x0F);
	    put_char((upper_bound >>  4) & 0x0F);
	    put_char( upper_bound        & 0x0F);
	}
	put_char(0xFF);
	break;

    case srec_record::type_data:
	{
	    while (address < record.get_address())
	    {
		put_char(0xFF);
		++address;
	    }
	    const unsigned char *data = record.get_data();
	    int length = record.get_length();
	    while (length-- > 0)
	    {
		// assert(address < upper_bound);
		if (address >= upper_bound)
		    fatal_error("upper bound stuffed");

		check_sum += *data;
		put_char(*data++);
		++address;
	    }
	}
	break;

    case srec_record::type_start_address:
	// assert(address == upper_bound);
	if (address != upper_bound)
	    fatal_error("upper bound stuffed");

	put_char(0);
	put_char(0);
	put_char(check_sum >> 8);
	put_char(check_sum);
	break;
    }
}


const char *
srec_output_file_formatted_binary::mode()
    const
{
    return "wb";
}


void
srec_output_file_formatted_binary::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srec_output_file_formatted_binary::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


int
srec_output_file_formatted_binary::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}

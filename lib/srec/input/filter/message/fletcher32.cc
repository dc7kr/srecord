//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009 Peter Miller
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

#include <lib/srec/arglex.h>
#include <lib/srec/input/filter/message/fletcher32.h>
#include <lib/srec/memory/walker/fletcher32.h>
#include <lib/srec/record.h>


srec_input_filter_message_fletcher32::~srec_input_filter_message_fletcher32()
{
}


srec_input_filter_message_fletcher32::srec_input_filter_message_fletcher32(
    const srec_input::pointer &a_deeper,
    unsigned long a_address,
    endian_t a_end
) :
    srec_input_filter_message(a_deeper),
    address(a_address),
    end(a_end)
{
}


srec_input::pointer
srec_input_filter_message_fletcher32::create(
    const srec_input::pointer &a_deeper, unsigned long a_address,
    endian_t a_end)
{
    return
        pointer
        (
            new srec_input_filter_message_fletcher32(a_deeper, a_address, a_end)
        );
}


void
srec_input_filter_message_fletcher32::process(const srec_memory &input,
    srec_record &output)
{
    //
    // Now calculate the Fletcher 32 checksum the bytes in order from
    // lowest address to highest.  (Holes are ignored, not filled,
    // warning issued already.)
    //
    srec_memory_walker_fletcher32::pointer w =
        srec_memory_walker_fletcher32::create();
    input.walk(w);
    unsigned long fletcher = w->get();

    //
    // Turn the CRC into the first data record.
    //
    unsigned char chunk[4];
    srec_record::encode(chunk, fletcher, sizeof(chunk), end);
    output = srec_record(srec_record::type_data, address, chunk, sizeof(chunk));
}


const char *
srec_input_filter_message_fletcher32::get_algorithm_name()
    const
{
    return "Fletcher-32";
}


// vim:ts=8:sw=4:et

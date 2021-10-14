//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/srec/input/filter/split.h>
#include <lib/srec/record.h>


srec_input_filter_split::~srec_input_filter_split()
{
}


srec_input_filter_split::srec_input_filter_split(const srec_input::pointer &a1,
        int a2, int a3, int a4) :
    srec_input_filter(a1),
    modulus(a2),
    offset(a2 - a3),
    width(a4),
    buffer(),
    buffer_pos(0)
{
    //
    // The offset is expressed as a positive number (by subtracting
    // the inherently negative offset from the modulus).  This avoids
    // modulo operator problems near zero.
    //
}


srec_input::pointer
srec_input_filter_split::create(const srec_input::pointer &a_deeper,
    int a2, int a3, int a4)
{
    return pointer(new srec_input_filter_split(a_deeper, a2, a3, a4));
}


int
srec_input_filter_split::read(srec_record &record)
{
    for (;;)
    {
        while
        (
            buffer.get_type() != srec_record::type_data
        ||
            buffer_pos >= buffer.get_length()
        )
        {
            if (!srec_input_filter::read(buffer))
                return 0;
            if (buffer.get_type() != srec_record::type_data)
            {
                record = buffer;
                return 1;
            }
            buffer_pos = 0;
        }

        unsigned long addr =
            (buffer.get_address() + offset + buffer_pos);
        int phase = addr % modulus;
        if (phase < width)
        {
            unsigned char c = buffer.get_data(buffer_pos++);
            //
            // Because the offset was made positive by
            // subtracting it from the modulus (to avoid
            // problems with the C modulo operator (%) near
            // zero) this means the address is actually one
            // multiple of modulus too high.  Subtract it
            // off before the width multiplication.
            //
            addr = (addr / modulus - 1) * width + phase;
            record = srec_record(srec_record::type_data, addr, &c, 1);
            return 1;
        }
        ++buffer_pos;
    }
}

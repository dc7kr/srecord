//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006-2008 Peter Miller
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

#include <lib/srec/output/file/cosmac.h>
#include <lib/srec/record.h>

srec_output_file_cosmac::~srec_output_file_cosmac()
{
}


srec_output_file_cosmac::srec_output_file_cosmac(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(0),
    address_length(4),
    line_length(80),
    column(0),
    header_required(true)
{
}


srec_output::pointer
srec_output_file_cosmac::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_cosmac(a_file_name));
}


void
srec_output_file_cosmac::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
    case srec_record::type_unknown:
    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_data:
        if (record.get_address() >= (1uL << 24) && address_length < 8)
            address_length = 8;
        else if (record.get_address() >= (1uL << 16) && address_length < 6)
            address_length = 6;

        if (header_required)
        {
            address = record.get_address();
            put_stringf("!M%.*lX ", (int)address_length, address);
            column = address_length + 3;
            header_required = false;
        }
        if (address != record.get_address())
        {
            address = record.get_address();
            put_stringf(";\n%.*lX ", (int)address_length, address);
            column = address_length + 1;
        }
        for (int j = 0; j < record.get_length(); ++j)
        {
            if (column + 3 > line_length)
            {
                put_string(",\n");
                column = 0;
            }
            put_byte(record.get_data(j));
            column += 2;
            ++address;
        }
        break;

    case srec_record::type_start_address:
        if (column != 0)
        {
            put_char('\n');
            column = 0;
            header_required = true;
        }
        break;
    }
}


void
srec_output_file_cosmac::line_length_set(int x)
{
    line_length = (x < 4 ? 4 : x);
}


void
srec_output_file_cosmac::address_length_set(int x)
{
    x *= 2;
    if (x < 4)
        x = 4;
    if (x > 8)
        x = 8;
    address_length = x;
}


int
srec_output_file_cosmac::preferred_block_size_get()
    const
{
    return ((line_length - 1) / 2);
}


const char *
srec_output_file_cosmac::format_name()
    const
{
    return "cosmac";
}

//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007, 2008 Peter Miller
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

#include <lib/srec/output/file/ti_txt.h>
#include <lib/srec/record.h>


srec_output_file_ti_txt::~srec_output_file_ti_txt()
{
    if (column > 0)
        put_char('\n');
    put_stringf("q\n");
    column = 0;
}


srec_output_file_ti_txt::srec_output_file_ti_txt(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(),
    address_set(false),
    address_length(2),
    pref_block_size(16),
    column(0),
    line_length(pref_block_size * 3 - 1)
{
}


srec_output::pointer
srec_output_file_ti_txt::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_ti_txt(a_file_name));
}


void
srec_output_file_ti_txt::put_byte_wrap(unsigned char c)
{
    if (column && column + 3 > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (column)
    {
        put_char(' ');
        ++column;
    }
    put_byte(c);
    ++address;
    column += 2;
}


void
srec_output_file_ti_txt::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        // All header data is discarded
        break;

    case srec_record::type_data:
        if (!address_set || address != record.get_address())
        {
            if (column > 0)
            {
                put_char('\n');
                column = 0;
            }

            address = record.get_address();
            address_set = true;

            int width = 2;
            if (address >= 0x1000000)
                width = 4;
            if (address >= 0x10000)
                width = 3;
            if (width < address_length)
                width = address_length;
            width *= 2;
            put_stringf("@%0*lX\n", width, address);
        }
        for (int j = 0; j < record.get_length(); ++j)
        {
            put_byte_wrap(record.get_data(j));
        }
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_start_address:
        // ignore
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_ti_txt::line_length_set(int linlen)
{
    int n = (linlen + 1) / 3;
    if (n < 1)
        n = 1;
    if (n > srec_record::max_data_length)
        n = srec_record::max_data_length;
    pref_block_size = n;
    line_length = pref_block_size * 3 - 1;
}


void
srec_output_file_ti_txt::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    if (n > 4)
        n = 4;
    address_length = n;
}


int
srec_output_file_ti_txt::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srec_output_file_ti_txt::format_name()
    const
{
    return "TI-Txt";
}

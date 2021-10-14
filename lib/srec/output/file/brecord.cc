//
//      srecord - The "srecord" program.
//      Copyright (C) 2007-2009 Peter Miller
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

#include <lib/srec/output/file/brecord.h>
#include <lib/srec/record.h>


srec_output_file_brecord::~srec_output_file_brecord()
{
    flush();
}


srec_output_file_brecord::srec_output_file_brecord(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    buffer_address(0),
    buffer_length(0),
    buffer_maximum(BUFFER_MAXIMUM_MAXIMUM)
{
}


srec_output::pointer
srec_output_file_brecord::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_brecord(a_file_name));
}


void
srec_output_file_brecord::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_execution_start_address:
        flush();
        put_4bytes(record.get_address());
        put_byte(0);
        put_char('\n');
        break;

    default:
        // This format can't do any other record types
        break;

    case srec_record::type_data:
        for (size_t j = 0; j < record.get_length(); ++j)
            buffer_stash(record.get_address() + j, record.get_data(j));
        break;
    }
}


void
srec_output_file_brecord::buffer_stash(unsigned long addr, unsigned char data)
{
    if (buffer_length > 0 && addr != buffer_address + buffer_length)
        flush();
    if (buffer_length == 0)
        buffer_address = addr;
    buffer[buffer_length++] = data;
    if (buffer_length >= buffer_maximum)
        flush();
}


void
srec_output_file_brecord::flush()
{
    if (buffer_length > 0)
    {
        put_4bytes(buffer_address);
        // assert(buffer_length <= BUFFER_MAXIMUM_MAXIMUM);
        put_byte(buffer_length);
        for (unsigned j = 0; j < buffer_length; ++j)
            put_byte(buffer[j]);
        put_char('\n');

        buffer_address = 0;
        buffer_length = 0;
    }
}


void
srec_output_file_brecord::line_length_set(int w)
{
    int x = (w - 10) / 2;
    if (x < 2)
        x = 2;
    else if (x > BUFFER_MAXIMUM_MAXIMUM)
        x = BUFFER_MAXIMUM_MAXIMUM;
    buffer_maximum = x;
}


void
srec_output_file_brecord::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


int
srec_output_file_brecord::preferred_block_size_get()
    const
{
    return BUFFER_MAXIMUM_MAXIMUM;
}


const char *
srec_output_file_brecord::format_name()
    const
{
    return "B-Record";
}

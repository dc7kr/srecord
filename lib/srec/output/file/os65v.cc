//
// srecord - manipulate eprom load files
// Copyright (C) 2002, 2006-2008 Peter Miller
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

#include <lib/srec/output/file/os65v.h>
#include <lib/srec/record.h>


srec_output_file_os65v::~srec_output_file_os65v()
{
    //
    // The OS65 audio tape loader seems to have had two ways of
    // stopping.  The first is to goto the address given by the 'G'
    // command.  The second is to poke into the monitor's RAM into the
    // variable which controls whether or not to keep loading from the
    // audio tape.  If we have seen (and emitted) a goto address, don't
    // bother with the poke to stop looping.
    //
    if (!(seen_start_address && enable_goto_addr_flag) && enable_footer_flag)
    {
        //
        // Note: we output a newline, even though the original OS65
        // used CR line termination.  The newline simply triggers the
        // behaviour controlled by the line_termination class variable.
        //
        put_string(".00FD/00\n");
    }
}


srec_output_file_os65v::srec_output_file_os65v(const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(0),
    state(0),
    seen_start_address(false)
{
    if (line_termination == line_termination_native)
    {
        //
        // Unless over-ridden on the command line, use CR line
        // termination.  This probably isn't ideal, but it is backwards
        // compatible.
        //
        line_termination = line_termination_cr;
    }
}


srec_output::pointer
srec_output_file_os65v::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_os65v(a_file_name));
}


void
srec_output_file_os65v::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        // All header data is discarded
        break;

    case srec_record::type_data:
        if (seen_start_address)
            fatal_error("more data following execution start address (bug)");
        if (address != record.get_address() || state == 0)
        {
            address = record.get_address();
            put_stringf(".%04lX/", address);
            state = '/';
        }
        for (int j = 0; j < record.get_length(); ++j)
        {
            unsigned char n = record.get_data(j);
            if (address == 0x00FD && n == 0)
            {
                // Actually, it's probably a bad idea to write on any of
                // the OS65 operating system's RAM.
                fatal_error("you may not write 00 to 00FD in this format");
            }
            put_byte(n);

            //
            // Note: we output a newline, even though the original OS65
            // used CR line termination.  The newline simply triggers
            // the behaviour controlled by the line_termination class
            // variable.
            //
            put_char('\n');
            ++address;
        }
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            if (address != record.get_address() || state == 0)
            {
                address = record.get_address();
                put_stringf(".%04lX", address);
                state = '.';
            }
            put_char('G');

            // The line termination will not be read, but it makes the
            // automated test script easier to write.
            put_char('\n');
        }
        seen_start_address = true;
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_os65v::line_length_set(int)
{
    // Irrelevant.  Ignore.
}


void
srec_output_file_os65v::address_length_set(int)
{
    // Irrelevant.  Ignore.
}


int
srec_output_file_os65v::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}


const char *
srec_output_file_os65v::mode(void)
    const
{
    return "wb";
}


const char *
srec_output_file_os65v::format_name()
    const
{
    return "OS65V";
}

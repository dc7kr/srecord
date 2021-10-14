//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2006-2009 Peter Miller
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

#include <cctype>
#include <lib/srec/input/file/ti_tagged_16.h>
#include <lib/srec/record.h>


srec_input_file_ti_tagged_16::~srec_input_file_ti_tagged_16()
{
}


srec_input_file_ti_tagged_16::srec_input_file_ti_tagged_16(
        const std::string &a_file_name) :
    srec_input_file(a_file_name),
    address(0),
    csum(0)
{
}


srec_input::pointer
srec_input_file_ti_tagged_16::create(const std::string &a_file_name)
{
    return pointer(new srec_input_file_ti_tagged_16(a_file_name));
}


int
srec_input_file_ti_tagged_16::get_char()
{
    int c = inherited::get_char();
    if (c < 0 || c == '\n')
        csum = 0;
    else
        csum += c;
    return c;
}


bool
srec_input_file_ti_tagged_16::read(srec_record &record)
{
    for (;;)
    {
        int c = get_char();
        switch (c)
        {
        default:
            fatal_error
            (
                (isprint(c) ? "unknown tag '%c'" : "unknown tag (%02X)"),
                c
            );

        case -1:
            return false;

        case '*':
            {
                // followed by 2 data characters (1 data byte)
                unsigned char data[1];
                data[0] = get_byte();
                record = srec_record(srec_record::type_data, address, data, 1);
                ++address;
                return true;
            }

        case ':':
            // end of file
            while (get_char() >= 0)
                ;
            return false;

        case '0':
            {
                // file header
                // 4 char: word count of the rest of the file (hex)
                // 8 char: file name (ascii)
                // we will ignore
                get_word();
                for (int n = 0; n < 8; ++n)
                    get_char();
            }
            break;

        case '7':
            {
                // checksum which the programmer verifies
                int n = (-csum) & 0xFFFF;
                int n2 = get_word();
                if (n != n2)
                {
                    fatal_error
                    (
                        "checksum mismatch (calculated %04X, file has %04X)",
                        n,
                        n2
                    );
                }
            }
            break;

        case '8':
            // a checksum which the programmer ignores.
            get_word();
            break;

        case '9':
            // load address which represents a word location.
            address = get_word() * 2;
            break;

        case 'B':
            {
                unsigned char data[2];
                data[0] = get_byte();
                data[1] = get_byte();
                record = srec_record(srec_record::type_data, address, data, 2);
                address += 2;
                return true;
            }

        case 'F':
            // denotes the end of a data record.
            if (get_char() != '\n')
                fatal_error("end of line expected");
            break;

        case 'K':
            {
                // file description
                int n = get_word();
                if (n < 5)
                {
                    bad_desc:
                    fatal_error("broken description");
                }
                n -= 5;
                int max = 250;
                unsigned char *buffer = new unsigned char [max];
                for (int j = 0; j < n; ++j)
                {
                    c = get_char();
                    if (c < 0 || c == '\n')
                            goto bad_desc;
                    if (j < max)
                            buffer[j] = c;
                }
                if (n > max)
                    n = max;
                record = srec_record(srec_record::type_header, 0, buffer, n);
                delete buffer;
            }
            return true;
        }
    }
}


const char *
srec_input_file_ti_tagged_16::get_file_format_name()
    const
{
    return "Texas Instruments SDSMAC (320)";
}

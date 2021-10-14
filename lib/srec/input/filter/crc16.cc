//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2006, 2007 Peter Miller
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


#include <lib/srec/arglex.h>
#include <lib/srec/input/filter/crc16.h>
#include <lib/srec/memory.h>
#include <lib/srec/memory/walker/crc16.h>
#include <lib/srec/record.h>

srec_input_filter_crc16::~srec_input_filter_crc16()
{
    delete buffer;
    buffer = 0;
}


srec_input_filter_crc16::srec_input_filter_crc16(srec_input *deeper_arg,
        unsigned long address_arg, int order_arg) :
    srec_input_filter(deeper_arg),
    address(address_arg),
    order(order_arg),
    ccitt_flag(true),
    augment_flag(true),
    buffer(0),
    buffer_pos(0),
    have_forwarded_header(false),
    have_given_crc(false),
    have_forwarded_start_address(false)
{
}


void
srec_input_filter_crc16::command_line(srec_arglex *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srec_arglex::token_crc16_xmodem:
            ccitt_flag = false;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_ccitt:
            ccitt_flag = true;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_augment:
            augment_flag = true;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_augment_not:
            augment_flag = false;
            cmdln->token_next();
            break;

        default:
            return;
        }
    }
}


int
srec_input_filter_crc16::read(srec_record &record)
{
    //
    // If we haven't read the deeper input yet, read all of it into
    // a memory buffer, then crc16 the bytes.
    //
    if (!buffer)
    {
        buffer = new srec_memory();
        buffer->reader(ifp, true);

        if (buffer->has_holes())
        {
            warning
            (
                "The data presented for CRC calculation has at least "
                "one hole in it.  This is bad.  It means that the "
                "in-memory calculation performed by your embedded "
                "system will be different than the calculation "
                "performed here.  You are strongly advised to use the "
                "--fill 0xFF filter *before* this CRC filter to ensure "
                "both calculations are using the same byte values.  "
                "See srec_info(1) for how to see the holes."
            );
        }
    }

    //
    // Pass on the header of the deeper file.
    //
    if (!have_forwarded_header)
    {
        have_forwarded_header = true;
        srec_record *rp = buffer->get_header();
        if (rp)
        {
            record = *rp;
            return 1;
        }
    }

    if (!have_given_crc)
    {
        have_given_crc = true;

        //
        // Now CRC16 the bytes in order from lowest address to
        // highest.  (Holes are ignored, not filled.)
        //
        srec_memory_walker_crc16 walker(ccitt_flag, augment_flag);
        buffer->walk(&walker);
        unsigned crc = walker.get();

        //
        // Turn the CRC into the first data record.
        //
        unsigned char chunk[2];
        if (order)
            srec_record::encode_little_endian(chunk, crc, sizeof(chunk));
        else
            srec_record::encode_big_endian(chunk, crc, sizeof(chunk));
        record =
            srec_record(srec_record::type_data, address, chunk, sizeof(chunk));
        return 1;
    }

    //
    // Now resend the rest of the data.
    //
    unsigned long ret_address = buffer_pos;
    unsigned char data[64];
    size_t nbytes = sizeof(data);
    if (buffer->find_next_data(ret_address, data, nbytes))
    {
        record = srec_record(srec_record::type_data, ret_address, data, nbytes);
        buffer_pos = ret_address + nbytes;
        return 1;
    }

    //
    // Pass on the header of the deeper file.
    //
    if (!have_forwarded_start_address)
    {
        have_forwarded_start_address = true;
        srec_record *rp = buffer->get_start_address();
        if (rp)
        {
            record = *rp;
            return 1;
        }
    }

    //
    // All done.
    //
    return 0;
}

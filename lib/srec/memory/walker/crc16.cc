//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2006-2008 Peter Miller
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

#include <lib/srec/memory/walker/crc16.h>
#include <lib/srec/output.h>


srec_memory_walker_crc16::~srec_memory_walker_crc16()
{
    delete checksum;
}


srec_memory_walker_crc16::srec_memory_walker_crc16(crc16::seed_mode_t seed_mode,
    bool augment_flag)
{
    checksum = new crc16(seed_mode, augment_flag);
}


srec_memory_walker_crc16::pointer
srec_memory_walker_crc16::create(crc16::seed_mode_t arg1, bool arg2)
{
    return pointer(new srec_memory_walker_crc16(arg1, arg2));
}


void
srec_memory_walker_crc16::observe(unsigned long, const void *data, int length)
{
    checksum->nextbuf(data, length);
}


unsigned
srec_memory_walker_crc16::get()
    const
{
    return checksum->get();
}

//
//      srecord - manipulate eprom load files
//      Copyright (C) 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_memory_walker_continuity class
//


#include <lib/srec/memory/walker/continuity.h>


srec_memory_walker_continuity::~srec_memory_walker_continuity()
{
}


srec_memory_walker_continuity::srec_memory_walker_continuity() :
    current_address(0),
    data_seen(false),
    nholes(0)
{
}


void
srec_memory_walker_continuity::observe(unsigned long addr, const void *data,
    int nbytes)
{
    if (data_seen)
    {
        if (current_address != addr)
            ++nholes;
    }
    else
    {
        data_seen = true;
    }
    current_address = addr + nbytes;
}


bool
srec_memory_walker_continuity::is_continuous()
    const
{
    return (data_seen && nholes == 0);
}

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


#include <lib/srec/input/filter/checksum/bitnot.h>


srec_input_filter_checksum_bitnot::~srec_input_filter_checksum_bitnot()
{
}


srec_input_filter_checksum_bitnot::srec_input_filter_checksum_bitnot(
        srec_input::pointer a1, int a2, int a3, int a4, int a5) :
    srec_input_filter_checksum(a1, a2, a3, a4, a5)
{
}


srec_input::pointer
srec_input_filter_checksum_bitnot::create(const srec_input::pointer &a_deeper,
    int a2, int a3, int a4, int a5)
{
    return
        pointer
        (
            new srec_input_filter_checksum_bitnot(a_deeper, a2, a3, a4, a5)
        );
}


srec_input_filter_checksum::sum_t
srec_input_filter_checksum_bitnot::calculate()
{
    return ~sum;
}

#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2006, 2007 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: Test the srec_output_file_asm functionality
#
here=`pwd`
if test $? -ne 0 ; then exit 2; fi
work=${TMP_DIR-/tmp}/$$

pass()
{
        cd $here
        rm -rf $work
        echo PASSED
        exit 0
}

fail()
{
        cd $here
        rm -rf $work
        echo 'FAILED test of the srec_output_file_asm functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the srec_output_file_asm functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
; HDR
; To avoid this next ORG directive, use the --offset -0x64 filter.
        ORG     100
        DB      72,101,108,108,111,44,32,87,111,114,108,100,33,10
; To avoid this next ORG directive, use the --fill filter.
        ORG     150
        DB      72,101,108,108,111,44,32,87,111,114,108,100,33,10
; upper bound = 0x00A4
; lower bound = 0x0064
; length =      0x0040
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -offset 100 test.in -offset 150 \
        -o test.out -asm > log 2>&1
if test $? -ne 0; then cat log; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 2003 Peter Miller;
#	All rights reserved.
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: Test the ASM functionality
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
	echo 'FAILED test of the ASM functionality'
	exit 1
}

no_result()
{
	cd $here
	rm -rf $work
	echo 'NO RESULT for test of the ASM functionality'
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
S12300004E6F77206973207468652074696D6520666F7220616C6C20676F6F64206D656E93
S123002020746F20636F6D6520746F2074686520616964206F6620746865697220706172AF
S107004074792E0A93
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
; HDR
	DB	78,111,119,32,105,115,32,116,104,101,32,116,105,109,101,32
	DB	102,111,114,32,97,108,108,32,103,111,111,100,32,109,101,110
	DB	32,116,111,32,99,111,109,101,32,116,111,32,116,104,101,32
	DB	97,105,100,32,111,102,32,116,104,101,105,114,32,112,97,114
	DB	116,121,46,10
; termination = 0
; start = 0
; finish = 68
; length = 68
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -o test.out -asm
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

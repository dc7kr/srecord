#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2006, 2007 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
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
        echo 'FAILED test of the memory chunk iterator functionality'
        exit 1
}

no_result()
{
        cd $here
        rm -rf $work
        echo 'NO RESULT for test of the memory chunk iterator functionality'
        exit 2
}

trap "no_result" 1 2 3 15

bin=$here/${1-.}/bin
mkdir $work
if test $? -ne 0; then no_result; fi
cd $work
if test $? -ne 0; then no_result; fi

cat > test.in.srec << 'fubar'
S00A0000746573742E696E30
S1230000D46EBDDE679162F728C97423D8B8ACD103E3E275D6383B02A05DBFA2F9F475B021
S1230020421C58D8C68B45162F288CBFF07FEB22E184550747074F93FB1C32DE00169D3F5A
S123004069EB95183E0AFCF7C59C6ECF3E1425664AA9324C43FF7EE9A3AAE11A466229882A
S1230060E1B1D00B068995A22340314D5363D7B5883D464362A99C67CCE2BA035ECFF5BD80
S12300800D9FA8D055F00CA046A175A3AA06DFA3232EF2CDCB3DD4CC4814838914F4834625
S12300A012E0306409C153A71892CAAE6BBE28D94F9469349B461636ACE8891F047C9B5D45
S12300C0B9F62AFD38EEFC127AD925F2156274BE0443900DBE4E74BE5EDE3843D35DDE9781
S12300E080A2D28BB22F67A6AC364D997BB2C680F43A37704548C9D0039E689E63CAE3DFC3
S12301002C6BD9B1863463E0359AFAE53BC18B3AAE6335FBD15E67A0F4456032ED4B8BA3A6
S12301203261893DAF1688419614BA7F18054B6AA9714D01C367BBE5B42F4C0266803DE6B3
S1230140AE87D6A56A446F9B26D059048ACDE643320A111D9F5A15F279EEDD8CFBC7976865
S12301608550C61ADC78E3EE58DF4BFB3752930EDA44DAA3AD36D2D6A1A0538A6E7274AEB4
S12301806B2D9D85D4723A3ED39460251CAF52E60529A6E73C82723FBA105FF62D795F376F
S12301A0B541DC43ABE6773867C232E55AFF44099D124A8BDABE28B580FD8BE2AB21D5D7A5
S12301C02B7DE36C620D1247E4A8B03067FE5D636F2AF1A0B58C450E4F7721FEE2291BED15
S12301E0FAA657F6BF9CCC2B9FC719DF0BB9BF7A92C72DF9B895F442137387C32171D049E4
S5030010EC
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Different:      0x40 - 0x43
fubar
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in.srec -o test.in -bin
if test $? -ne 0; then no_result; fi

$bin/srec_cat test.in -bin -exclude 0x40 0x44 -becrc32 0x40 -o test.out -bin \
    > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

$bin/srec_cmp -v test.in -bin test.out -bin > log
if test $? -ne 2; then fail; fi

diff test.ok log
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

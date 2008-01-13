//
//        srecord - manipulate eprom load files
//        Copyright (C) 1998-2008 Peter Miller
//
//        This program is free software; you can redistribute it and/or modify
//        it under the terms of the GNU General Public License as published by
//        the Free Software Foundation; either version 3 of the License, or
//        (at your option) any later version.
//
//        This program is distributed in the hope that it will be useful,
//        but WITHOUT ANY WARRANTY; without even the implied warranty of
//        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//        GNU General Public License for more details.
//
//        You should have received a copy of the GNU General Public License
//        along with this program. If not, see
//        <http://www.gnu.org/licenses/>.
//

#include <iostream>
using namespace std;

#include <lib/srec/arglex.h>
#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>


srec_arglex::srec_arglex(int argc, char **argv) :
    arglex(argc, argv),
    stdin_used(false),
    stdout_used(false),
    issue_sequence_warnings(-1)
{
    static const table_ty table[] =
    {
        { "(", token_paren_begin, },
        { "{", token_paren_begin, },
        { "[", token_paren_begin, },
        { ")", token_paren_end, },
        { "}", token_paren_end, },
        { "]", token_paren_end, },
        { "+", token_union, },
        { "-A430", token_a430, },
        { "-Absolute_Object_Module_Format", token_aomf, },
        { "-AND", token_and, },
        { "-Ascii_Hexadecimal", token_ascii_hex, },
        { "-Ascii_Space_Hexadecimal", token_ascii_hex, },
        { "-ASseMbler", token_assembler, },
        { "-Atmel_Generic", token_atmel_generic_le, },
        { "-Atmel_Generic_BigEndian", token_atmel_generic_be, },
        { "-Atmel_Generic_LittleEndian", token_atmel_generic_le, },
        { "-AUGment", token_crc16_augment },
        { "-Not_AUGment", token_crc16_augment_not },
        { "-BAsic", token_basic_data, },
        { "-Big_Endian_Checksum", token_checksum_be_bitnot, },
        { "-Big_Endian_Checksum_BitNot", token_checksum_be_bitnot, },
        { "-Big_Endian_Checksum_Negative", token_checksum_be_negative, },
        { "-Big_Endian_Checksum_Positive", token_checksum_be_positive, },
        { "-Big_Endian_Cyclic_Redundancy_Check_16", token_crc16_be, },
        { "-Big_Endian_Cyclic_Redundancy_Check_32", token_crc32_be, },
        { "-Big_Endian_Length", token_length_be, },
        { "-Big_Endian_MAximum", token_maximum_be, },
        { "-Big_Endian_MInimum",token_minimum_be, },
        { "-BINary", token_binary, },
        { "-B_Record", token_brecord, },
        { "-BROken", token_crc16_broken, },
        { "-Byte_Swap", token_byte_swap, },
        { "-C_COMpressed", token_c_compressed, },
        { "-CL430", token_cl430, },
        { "-CONSTant", token_constant, },
        { "-COsmac", token_cosmac, },
        { "-CRop", token_crop, },
        { "-Cyclic_Redundancy_Check_16_XMODEM", token_crc16_xmodem,},
        { "-Comite_Consultatif_International_Telephonique_et_Telegraphique",
            token_crc16_ccitt,},
        { "-International_Telecommunication_Union", token_crc16_ccitt,},
        { "-C_Array", token_c_array, },
        { "-Dec_Binary", token_dec_binary, },
        { "-DECimal_STyle", token_style_hexadecimal_not, },
        { "-DIFference", token_minus, },
        { "-Disable_Sequence_Warnings", token_sequence_warnings_disable, },
        { "-Dot_STyle", token_style_dot, },
        { "-Elektor_Monitor52", token_emon52, },
        { "-Enable_Sequence_Warnings", token_sequence_warnings_enable, },
        { "-Exclude", token_exclude, },
        { "-FAIrchild", token_fairchild, },
        { "-Fast_Load", token_fast_load, },
        { "-Fill", token_fill, },
        { "-Formatted_Binary", token_formatted_binary, },
        { "-Four_Packed_Code", token_four_packed_code, },
        { "-GENerate", token_generator },
        { "-GENerator", token_generator },
        { "-GUess", token_guess, },
        { "-HEXadecimal_Dump", token_hexdump, },
        { "-HEXadecimal_STyle", token_style_hexadecimal, },
        { "-IGnore_Checksums", token_ignore_checksums, },
        { "-INCLude", token_include, },
        { "-Intel", token_intel, },
        { "-INtel_HeXadecimal_16", token_intel16, },
        { "-INTERSection", token_intersection, },
        { "-Length", token_length, },
        { "-Little_Endian_Checksum", token_checksum_le_bitnot, },
        { "-Little_Endian_Checksum_BitNot", token_checksum_le_bitnot, },
        { "-Little_Endian_Checksum_Negative", token_checksum_le_negative, },
        { "-Little_Endian_Checksum_Positive", token_checksum_le_positive, },
        { "-Little_Endian_Cyclic_Redundancy_Check_16", token_crc16_le,},
        { "-Little_Endian_Cyclic_Redundancy_Check_32", token_crc32_le,},
        { "-Little_Endian_Length", token_length_le, },
        { "-Little_Endian_MAximum", token_maximum_le, },
        { "-Little_Endian_MInimum", token_minimum_le, },
        { "-MAximum", token_maximum, },
        { "-MInimum", token_minimum, },
        { "-MINUs", token_minus, },
        { "-MOS_Technologies", token_mos_tech, },
        { "-Motorola", token_motorola, },
        { "-MULTiple", token_multiple, },
        { "-Needham_Hexadecimal", token_needham_hex, },
        { "-NOT", token_not, },
        { "-Not_CONSTant", token_constant_not, },
        { "-Not_INCLude", token_include_not, },
        { "-OFfset", token_offset, },
        { "-Ohio_Scientific", token_ohio_scientific, },
        { "-Ohio_Scientific65v", token_ohio_scientific, },
        { "-OR", token_or, },
        { "-Output", token_output, },
        { "-Output_Words", token_output_word, },
        { "-OVer", token_over, },
        { "-RANDom", token_random, },
        { "-Random_Fill", token_random_fill, },
        { "-RAnge_PADding", token_range_padding, },
        { "-RAw", token_binary, },
        { "-REPeat_Data", token_repeat_data, },
        { "-REPeat_String", token_repeat_string, },
        { "-Round", token_round_nearest, },
        { "-Round_Down", token_round_down, },
        { "-Round_Nearest", token_round_nearest, },
        { "-Round_Off", token_round_nearest, },
        { "-Round_Up", token_round_up, },
        { "-Section_STyle", token_style_section, },
        { "-SIGnetics", token_signetics, },
        { "-SPAsm", token_spasm_be, }, // is this right?
        { "-SPAsm_BigEndian", token_spasm_be, },
        { "-SPAsm_LittleEndian", token_spasm_le, },
        { "-SPEctrum", token_spectrum, },
        { "-SPlit", token_split, },
        { "-STewie", token_stewie, },
        { "-SUBtract", token_minus, },
        { "-S_record", token_motorola, },
        { "-Tektronix", token_tektronix, },
        { "-Tektronix_Extended", token_tektronix_extended, },
        { "-Texas_Instruments_Tagged", token_ti_tagged, },
        { "-Texas_Instruments_Tagged_16", token_ti_tagged_16, },
        { "-Texas_Instruments_TeXT", token_ti_txt, },
        { "-Un_Fill", token_unfill, },
        { "-UNIon", token_union, },
        { "-Un_SPlit", token_unsplit, },
        { "-VHdl", token_vhdl, },
        { "-VMem", token_vmem, },
        { "-WILson", token_wilson, },
        { "-Within", token_within, },
        { "-X_MODEM", token_crc16_xmodem,},
        { "-XOR", token_xor, },
        ARGLEX_END_MARKER
    };

    table_set(table);
}


srec_arglex::~srec_arglex()
{
}


bool
srec_arglex::can_get_number()
    const
{
    switch (token_cur())
    {
    case token_number:
    case token_minimum:
    case token_maximum:
    case token_length:
        return true;

    default:
        return false;
    }
}


void
srec_arglex::get_address(const char *name, unsigned long &address)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address", name);
        // NOTREACHED
    }
    address = get_number("address");
}


void
srec_arglex::get_address_and_nbytes(const char *name, unsigned long &address,
    int &nbytes)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address and a byte count", name);
        // NOTREACHED
    }
    address = get_number("address");
    nbytes = 4;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srec_arglex::get_address_nbytes_width(const char *name, unsigned long &address,
    int &nbytes, int &width)
{
    address = get_number("address");
    nbytes = 4;
    width = 1;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
        if (can_get_number())
        {
            width = get_number("width", 1, nbytes);
        }
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srec_arglex::default_command_line_processing()
{
    switch (token_cur())
    {
    default:
        arglex::default_command_line_processing();
        break;

    case srec_arglex::token_ignore_checksums:
        srec_input_file::ignore_all_checksums();
        token_next();
        break;

    case srec_arglex::token_sequence_warnings_enable:
        issue_sequence_warnings = 1;
        token_next();
        break;

    case srec_arglex::token_sequence_warnings_disable:
        issue_sequence_warnings = 0;
        token_next();
        break;

    case srec_arglex::token_multiple:
        srec_memory::allow_overwriting();
        token_next();
        break;
    }
}

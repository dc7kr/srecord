//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001-2004, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_arglex_output class
//

#include <lib/srec/arglex.h>
#include <lib/srec/output/file/aomf.h>
#include <lib/srec/output/file/ascii_hex.h>
#include <lib/srec/output/file/asm.h>
#include <lib/srec/output/file/atmel_generic.h>
#include <lib/srec/output/file/basic.h>
#include <lib/srec/output/file/binary.h>
#include <lib/srec/output/file/c.h>
#include <lib/srec/output/file/cosmac.h>
#include <lib/srec/output/file/dec_binary.h>
#include <lib/srec/output/file/emon52.h>
#include <lib/srec/output/file/fairchild.h>
#include <lib/srec/output/file/fastload.h>
#include <lib/srec/output/file/formatted_binary.h>
#include <lib/srec/output/file/four_packed_code.h>
#include <lib/srec/output/file/intel.h>
#include <lib/srec/output/file/intel16.h>
#include <lib/srec/output/file/mos_tech.h>
#include <lib/srec/output/file/needham.h>
#include <lib/srec/output/file/os65v.h>
#include <lib/srec/output/file/signetics.h>
#include <lib/srec/output/file/spasm.h>
#include <lib/srec/output/file/spectrum.h>
#include <lib/srec/output/file/srecord.h>
#include <lib/srec/output/file/stewie.h>
#include <lib/srec/output/file/tektronix_extended.h>
#include <lib/srec/output/file/tektronix.h>
#include <lib/srec/output/file/ti_tagged.h>
#include <lib/srec/output/file/vhdl.h>
#include <lib/srec/output/file/vmem.h>
#include <lib/srec/output/file/wilson.h>


srec_output *
srec_arglex::get_output()
{
    //
    // skip the -output token
    //
    if (token_cur() == token_output)
	token_next();

    //
    // determine the file name
    //
    std::string fn = "-";
    switch (token_cur())
    {
    case token_stdio:
	token_next();
	// fall through...

    default:
	if (stdout_used)
	{
	    cerr << "the standard output may only be named once on the command "
		"line" << endl;
	    exit(1);
	}
	stdout_used = true;
	break;

    case token_string:
	fn = value_string();
	token_next();
	break;
    }

    //
    // determine the file format
    //
    srec_output *ofp = 0;
    switch (token_cur())
    {
    case token_motorola:
	token_next();
	// fall through...

    default:
	ofp = new srec_output_file_srecord(fn.c_str());
	break;

    case token_aomf:
	token_next();
	ofp = new srec_output_file_aomf(fn.c_str());
	break;

    case token_ascii_hex:
	token_next();
	ofp = new srec_output_file_ascii_hex(fn.c_str());
	break;

    case token_assembler:
	token_next();
	ofp = new srec_output_file_asm(fn.c_str());
	break;

    case token_atmel_generic_be:
	token_next();
	ofp = new srec_output_file_atmel_generic(fn.c_str(), true);
	break;

    case token_atmel_generic_le:
	token_next();
	ofp = new srec_output_file_atmel_generic(fn.c_str(), false);
	break;

    case token_basic_data:
	token_next();
	ofp = new srec_output_file_basic(fn.c_str());
	break;

    case token_binary:
	token_next();
	ofp = new srec_output_file_binary(fn.c_str());
	break;

    case token_c_array:
	token_next();
	ofp = new srec_output_file_c(fn.c_str());
	break;

    case token_cosmac:
	token_next();
	ofp = new srec_output_file_cosmac(fn.c_str());
	break;

    case token_dec_binary:
	token_next();
	ofp = new srec_output_file_dec_binary(fn.c_str());
	break;

    case token_emon52:
	token_next();
	ofp = new srec_output_file_emon52(fn.c_str());
	break;

    case token_fairchild:
	token_next();
	ofp = new srec_output_file_fairchild(fn.c_str());
	break;

    case token_fast_load:
	token_next();
	ofp = new srec_output_file_fastload(fn.c_str());
	break;

    case token_formatted_binary:
	token_next();
	ofp = new srec_output_file_formatted_binary(fn.c_str());
	break;

    case token_four_packed_code:
	token_next();
	ofp = new srec_output_file_four_packed_code(fn.c_str());
	break;

    case token_intel:
	token_next();
	ofp = new srec_output_file_intel(fn.c_str());
	break;

    case token_intel16:
	token_next();
	ofp = new srec_output_file_intel16(fn.c_str());
	break;

    case token_mos_tech:
	token_next();
	ofp = new srec_output_file_mos_tech(fn.c_str());
	break;

    case token_needham_hex:
	token_next();
	ofp = new srec_output_file_needham(fn.c_str());
	break;

    case token_ohio_scientific:
	token_next();
	ofp = new srec_output_file_os65v(fn.c_str());
	break;

    case token_signetics:
	token_next();
	ofp = new srec_output_file_signetics(fn.c_str());
	break;

    case token_spasm_be:
	token_next();
	ofp = new srec_output_file_spasm(fn.c_str(), true);
	break;

    case token_spasm_le:
	token_next();
	ofp = new srec_output_file_spasm(fn.c_str(), false);
	break;

    case token_spectrum:
	token_next();
	ofp = new srec_output_file_spectrum(fn.c_str());
	break;

    case token_stewie:
	token_next();
	ofp = new srec_output_file_stewie(fn.c_str());
	break;

    case token_tektronix:
	token_next();
	ofp = new srec_output_file_tektronix(fn.c_str());
	break;

    case token_tektronix_extended:
	token_next();
	ofp = new srec_output_file_tektronix_extended(fn.c_str());
	break;

    case token_ti_tagged:
	token_next();
	ofp = new srec_output_file_ti_tagged(fn.c_str());
	break;

    case token_vhdl:
	token_next();
	ofp = new srec_output_file_vhdl(fn.c_str());
	break;

    case token_vmem:
	token_next();
	ofp = new srec_output_file_vmem(fn.c_str());
	break;

    case token_wilson:
	token_next();
	ofp = new srec_output_file_wilson(fn.c_str());
	break;
    }

    //
    // Parse extra command line options for this format, if any.
    //
    //assert(ofp);
    ofp->command_line(this);

    //
    // return the stream determined
    //
    return ofp;
}

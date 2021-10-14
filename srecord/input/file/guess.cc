//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <cctype>

#include <srecord/quit/exception.h>
#include <srecord/input/file/aomf.h>
#include <srecord/input/file/ascii_hex.h>
#include <srecord/input/file/atmel_generic.h>
#include <srecord/input/file/binary.h>
#include <srecord/input/file/brecord.h>
#include <srecord/input/file/dec_binary.h>
#include <srecord/input/file/emon52.h>
#include <srecord/input/file/fairchild.h>
#include <srecord/input/file/fastload.h>
#include <srecord/input/file/formatted_binary.h>
#include <srecord/input/file/four_packed_code.h>
#include <srecord/input/file/hexdump.h>
#include <srecord/input/file/idt.h>
#include <srecord/input/file/intel.h>
#include <srecord/input/file/intel16.h>
#include <srecord/input/file/mif.h>
#include <srecord/input/file/mips_flash.h>
#include <srecord/input/file/mos_tech.h>
#include <srecord/input/file/motorola.h>
#include <srecord/input/file/msbin.h>
#include <srecord/input/file/needham.h>
#include <srecord/input/file/os65v.h>
#include <srecord/input/file/ppb.h>
#include <srecord/input/file/ppx.h>
#include <srecord/input/file/signetics.h>
#include <srecord/input/file/spasm.h>
#include <srecord/input/file/spectrum.h>
#include <srecord/input/file/stewie.h>
#include <srecord/input/file/tektronix.h>
#include <srecord/input/file/tektronix_extended.h>
#include <srecord/input/file/ti_tagged.h>
#include <srecord/input/file/ti_txt.h>
#include <srecord/input/file/vmem.h>
#include <srecord/input/file/wilson.h>
#include <srecord/record.h>


typedef srecord::input_file::pointer (*func_p)(const std::string &file_name);

static func_p table[] =
{
    srecord::input_file_aomf::create,
    srecord::input_file_ascii_hex::create,
    srecord::input_file_atmel_generic::create_be,
    srecord::input_file_brecord::create,
    srecord::input_file_dec_binary::create,
    srecord::input_file_emon52::create,
    srecord::input_file_fairchild::create,
    srecord::input_file_fastload::create,
    srecord::input_file_formatted_binary::create,
    srecord::input_file_four_packed_code::create,
    srecord::input_file_idt::create,
    srecord::input_file_intel::create,
    srecord::input_file_intel16::create,
    srecord::input_file_mif::create,
    srecord::input_file_mips_flash::create_be,
    srecord::input_file_mos_tech::create,
    srecord::input_file_motorola::create,
    srecord::input_file_msbin::create,
    srecord::input_file_needham::create,
    srecord::input_file_os65v::create,
    srecord::input_file_ppb::create,
    srecord::input_file_ppx::create,
    srecord::input_file_signetics::create,
    srecord::input_file_spasm::create_be,
    srecord::input_file_spectrum::create,
    srecord::input_file_stewie::create,
    srecord::input_file_tektronix::create,
    srecord::input_file_tektronix_extended::create,
    srecord::input_file_ti_tagged::create,
    srecord::input_file_ti_txt::create,
    srecord::input_file_vmem::create,
    srecord::input_file_wilson::create,
    srecord::input_file_hexdump::create,
};

#define SIZEOF(a) (sizeof(a) / sizeof(a[0]))
#define ENDOF(a) ((a) + SIZEOF(a))


static std::string
abbreviate(const char *s)
{
    std::string result;
    for (;;)
    {
        unsigned char c = *s++;
        switch (c)
        {
        case '\0':
            return result;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z': case '_':
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            c = tolower(c);
            result += c;
            break;

        default:
            result += c;
            break;
        }
    }
}


srecord::input_file::pointer
srecord::input_file::guess(const std::string &fn)
{
    if (fn.empty() || fn == "-")
    {
        quit_default.fatal_error
        (
            "the file format of the standard input must be specified explicitly"
        );
    }

    //
    // Try each file format in turn.
    //
    quit_exception quitter;
    for (func_p *tp = table; tp < ENDOF(table); ++tp)
    {
        //
        // Create a new file reader
        //
        func_p func = *tp;
        srecord::input_file::pointer ifp = func(fn);
        try
        {
            //
            // Set the exception-throwing quitter
            // (it also silences all error and warning messages).
            //
            ifp->set_quit(quitter);
            srecord::record record;

            //
            // Try to read something from the file.  If it is
            // the wrong format, it will throw an exception.
            //
            if (ifp->read(record))
            {
                const char *option = ifp->format_option_name();
                quit_default.message
                (
                    "%s: the file format can be accessed directly with "
                        "the %s command line option (%s)",
                    fn.c_str(),
                    option,
                    abbreviate(option).c_str()
                );

                //
                // It is necessary to nuke the old file reader.
                // (a) Because it has the wrong quitter, but more importantly
                // (b) because it is no longer positioned at the start of the
                //     file, and the user *will* miss some data, also
                // (c) we need a chance to use the input::command_line()
                //     method.
                //
                ifp.reset();

                //
                // Return a brand-new file reader.
                //
                ifp = func(fn);
                return ifp;
            }
        }
        catch (quit_exception::vomit)
        {
        }

        //
        // Wrong format.
        // Toss this one, and try another.
        //
        ifp.reset();
    }

    //
    // If nothing else works, assume the file is binary.
    //
    quit_default.warning
    (
        "%s: unable to determine the file format, assuming -Binary",
        fn.c_str()
    );
    return srecord::input_file_binary::create(fn);
}


// vim: set ts=8 sw=4 et :

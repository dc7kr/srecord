//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2003, 2006-2008 Peter Miller
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

#include <cstdio>
#include <cstring>

#include <lib/interval.h>
#include <lib/srec/arglex.h>
#include <lib/srec/output/file/c.h>
#include <lib/srec/record.h>


static string
toupper(const string &s)
{
    char *buffer = new char[s.size() + 1];
    char *bp = buffer;
    const char *cp = s.c_str();
    while (*cp)
    {
        unsigned char c = *cp++;
        if (islower(c))
            *bp++ = toupper(c);
        else
            *bp++ = c;
    }
    string result(buffer, bp - buffer);
    delete [] buffer;
    return result;
}


static string
identifier(const string &s)
{
    char *buffer = new char[s.size() + 1];
    char *bp = buffer;
    const char *cp = s.c_str();
    while (*cp)
    {
        unsigned char c = *cp++;
        if (islower(c))
            *bp++ = toupper(c);
        else if (isalnum(c))
            *bp++ = c;
        else
            *bp++ = '_';
    }
    string result(buffer, bp - buffer);
    delete [] buffer;
    return result;
}


srec_output_file_c::~srec_output_file_c()
{
    //
    // Finish initializing the data array.
    //
    emit_header();
    if (range.empty())
    {
        if (output_word)
            emit_word(0xFFFF);
        else
            emit_byte(0xFF);
    }
    if (column)
    {
        put_char('\n');
        column = 0;
    }
    put_string("};\n");

    int nsections = 0;
    if (section_style)
    {
        //
        // emit list of section addresses
        //
        put_string("\n");
        if (constant)
            put_string("const ");
        put_stringf("unsigned long %s_address[] =\n{\n", prefix.c_str());
        interval x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;
            unsigned long address = x2.get_lowest();

            string s = format_address(address);
            int len = s.size();

            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column)
            {
                put_char(' ');
                ++column;
            }
            put_string(s);
            column += len;
            put_char(',');
            ++column;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }
        put_stringf("};\n");

        //
        // emit list of section lengths
        //
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_length_of_sections[] =\n{\n",
            prefix.c_str()
        );
        x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;
            unsigned long length = x2.get_highest() - x2.get_lowest();
            ++nsections;

            if (output_word)
                length /= 2;
            string s = format_address(length);
            int len = s.size();

            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column)
            {
                put_char(' ');
                ++column;
            }
            put_string(s);
            column += len;
            put_char(',');
            ++column;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }
        put_string("};\n");

        //
        // emit the number of sections
        //
        if (constant)
            put_string("const ");
        put_string("unsigned long ");
        put_string(prefix.c_str());
        put_string("_sections    = ");
        put_string(format_address(nsections));
        put_string(";\n");
    }

    if (!data_only_flag)
    {
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_termination = %s;\n",
            prefix.c_str(),
            format_address(taddr).c_str()
        );
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_start       = %s;\n",
            prefix.c_str(),
            format_address(range.get_lowest()).c_str()
        );
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_finish      = %s;\n",
            prefix.c_str(),
            format_address(range.get_highest()).c_str()
        );
    }

    if (constant)
        put_string("const ");
    put_stringf
    (
        "unsigned long %s_length      = %s;\n",
        prefix.c_str(),
        format_address(range.get_highest() - range.get_lowest()).c_str()
    );

    //
    // Some folks prefer #define instead
    //
    put_char('\n');

    string PREFIX = toupper(prefix);
    put_stringf
    (
        "#define %s_TERMINATION %s\n",
        PREFIX.c_str(),
        format_address(taddr).c_str()
    );
    put_stringf
    (
        "#define %s_START       %s\n",
        PREFIX.c_str(),
        format_address(range.get_lowest()).c_str()
    );
    put_stringf
    (
        "#define %s_FINISH      %s\n",
        PREFIX.c_str(),
        format_address(range.get_highest()).c_str()
    );
    put_stringf
    (
        "#define %s_LENGTH      %s\n",
        PREFIX.c_str(),
        format_address(range.get_highest() - range.get_lowest()).c_str()
    );
    if (section_style)
    {
        put_stringf
        (
            "#define %s_SECTIONS    %s\n",
            PREFIX.c_str(),
            format_address(nsections).c_str()
        );
    }

    if (include)
    {
        string insulation = identifier(include_file_name);
        FILE *fp = fopen(include_file_name.c_str(), "w");
        if (!fp)
            fatal_error_errno("open %s", include_file_name.c_str());
        fprintf(fp, "#ifndef %s\n", insulation.c_str());
        fprintf(fp, "#define %s\n", insulation.c_str());
        fprintf(fp, "\n");
        if (!data_only_flag)
        {
            if (constant)
                fprintf(fp, "const ");
            fprintf
            (
                fp,
                "extern unsigned long %s_termination;\n",
                prefix.c_str()
            );
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_start;\n", prefix.c_str());
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_finish;\n", prefix.c_str());
        }
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "extern unsigned long %s_length;\n", prefix.c_str());
        if (section_style)
        {
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_sections;\n", prefix.c_str());
        }
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "extern unsigned char %s[];\n", prefix.c_str());
        fprintf(fp, "\n");
        fprintf(fp, "#endif /* %s */\n", insulation.c_str());

        if (fclose(fp))
            fatal_error_errno("write %s", include_file_name.c_str());
    }
}


static const char *
memrchr(const char *data, char c, size_t len)
{
    if (!data)
        return 0;
    const char *result = 0;
    while (len > 0)
    {
        const char *p = (const char *)memchr(data, c, len);
        if (!p)
            break;
        result = p;
        size_t chunk = p - data + 1;
        data += chunk;
        len -= chunk;
    }
    return result;
}


static string
build_include_file_name(const string &filename)
{
    const char *fn = filename.c_str();
    // Watch out for out base class adding a line number.
    const char *colon = strstr(fn, ": ");
    if (!colon)
        colon = fn + strlen(fn);
    const char *slash = memrchr(fn, '/', colon - fn);
    if (!slash)
        slash = memrchr(fn, '\\', colon - fn);
    if (slash)
        slash++;
    else
        slash = fn;
    const char *ep = memrchr(slash, '.', colon - slash);
    if (!ep)
        ep = colon;
    return (string(fn, ep - fn) + ".h");
}


srec_output_file_c::srec_output_file_c(const string &a_file_name) :
    srec_output_file(a_file_name),
    prefix("eprom"),
    taddr(0),
    header_done(false),
    column(0),
    current_address(0),
    line_length(75),
    address_length(4),
    constant(true),
    include(false),
    include_file_name(build_include_file_name(a_file_name)),
    output_word(false),
    hex_style(true),
    section_style(false)
{
}


srec_output::pointer
srec_output_file_c::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_c(a_file_name));
}


void
srec_output_file_c::command_line(srec_arglex *cmdln)
{
    if (cmdln->token_cur() == arglex::token_string)
    {
        prefix = cmdln->value_string();
        cmdln->token_next();
    }
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srec_arglex::token_constant:
            cmdln->token_next();
            constant = true;
            break;

        case srec_arglex::token_constant_not:
            cmdln->token_next();
            constant = false;
            break;

        case srec_arglex::token_include:
            cmdln->token_next();
            include = true;
            break;

        case srec_arglex::token_include_not:
            cmdln->token_next();
            include = false;
            break;

        case srec_arglex::token_c_compressed:
            cmdln->token_next();
            hex_style = true;
            section_style = true;
            break;

        case srec_arglex::token_output_word:
            cmdln->token_next();
            output_word = true;
            break;

        case srec_arglex::token_style_hexadecimal:
            cmdln->token_next();
            hex_style = true;
            break;

        case srec_arglex::token_style_hexadecimal_not:
            cmdln->token_next();
            hex_style = false;
            break;

        case srec_arglex::token_style_section:
        case srec_arglex::token_a430:
        case srec_arglex::token_cl430:
            cmdln->token_next();
            section_style = true;
            break;

        default:
            return;
        }
    }
}


void
srec_output_file_c::emit_header()
{
    if (header_done)
        return;
    if (constant)
        put_stringf("const ");
    if (output_word)
        put_string("unsigned short");
    else
        put_string("unsigned char");
    put_char(' ');
    put_string(prefix.c_str());
    put_string("[] =\n{\n");
    header_done = true;
    column = 0;
}


void
srec_output_file_c::emit_byte(int n)
{
    char buffer[30];
    if (hex_style)
        snprintf(buffer, sizeof(buffer), "0x%2.2X", (unsigned char)n);
    else
        snprintf(buffer, sizeof(buffer), "%u", (unsigned char)n);
    int len = strlen(buffer);

    if (column && column + 2 + len > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (column)
    {
        put_char(' ');
        ++column;
    }
    put_string(buffer);
    column += len;
    put_char(',');
    ++column;
}


void
srec_output_file_c::emit_word(unsigned int n)
{
    char buffer[30];
    if (hex_style)
        snprintf(buffer, sizeof(buffer), "0x%4.4X", (unsigned short)n);
    else
        snprintf(buffer, sizeof(buffer), "%u", (unsigned short)n);
    int len = strlen(buffer);

    if (column && column + 2 + len > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (column)
    {
        put_char(' ');
        ++column;
    }
    put_string(buffer);
    column += len;
    put_char(',');
    ++column;
}


string
srec_output_file_c::format_address(unsigned long addr)
{
    char buffer[30];
    if (hex_style)
        snprintf(buffer, sizeof(buffer), "0x%0*lX", address_length * 2, addr);
    else
        snprintf(buffer, sizeof(buffer), "%lu", addr);
    return buffer;
}


void
srec_output_file_c::write(const srec_record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srec_record::type_header:
        // emit header records as comments in the file
        {
            put_string("/* ");
            if (record.get_address() != 0)
                put_stringf("%08lX: ", record.get_address());
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (isprint(c) || isspace(c))
                    put_char(c);
                else
                    put_stringf("\\%o", c);
                // make sure we don't end the comment
                if (c == '*' && cp < ep && *cp == '/')
                    put_char(' ');
            }
            put_string(" */\n");
        }
        break;

    case srec_record::type_data:
        emit_header();
        if (output_word)
        {
            if ((record.get_address() & 1) || (record.get_length() & 1))
                fatal_alignment_error(2);

            unsigned long min = record.get_address();
            unsigned long max = record.get_address() + record.get_length();
            if (!section_style && !range.empty())
            {
                // assert(current_address <= min);
                while (current_address < min)
                {
                    emit_word(0xFFFF);
                    current_address += 2;
                }
            }

            range += interval(min, max);

            for (int j = 0; j < record.get_length(); j += 2)
            {
                unsigned char n1 = record.get_data(j);
                unsigned char n2 = record.get_data(j + 1);
                // little endian
                unsigned short n = n1 + (n2 << 8);
                emit_word(n);
            }
            current_address = max;
        }
        else
        {
            unsigned long min = record.get_address();
            unsigned long max = record.get_address() + record.get_length();
            if (!section_style && !range.empty())
            {
                // assert(current_address <= min);
                while (current_address < min)
                {
                    emit_byte(0xFF);
                    ++current_address;
                }
            }

            range += interval(min, max);
            for (int j = 0; j < record.get_length(); ++j)
            {
                emit_byte(record.get_data(j));
            }
            current_address = max;
        }
        break;

    case srec_record::type_start_address:
        taddr = record.get_address();
        break;
    }
}


void
srec_output_file_c::line_length_set(int n)
{
    line_length = n;
}


void
srec_output_file_c::address_length_set(int n)
{
    switch (n)
    {
    default:
        address_length = 4;
        break;

    case 1:
    case 2:
    case 3:
    case 4:
        address_length = n;
        break;

    case 16:
        address_length = 2;
        break;

    case 32:
        address_length = 4;
        break;
    }
}


int
srec_output_file_c::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get,
    // but be careful about words.
    //
    if (output_word)
        return (srec_record::max_data_length & ~1);
    return srec_record::max_data_length;
}


const char *
srec_output_file_c::format_name()
    const
{
    return (output_word ? "C-Array (16-bit)" : "C-Array (8-bit)");
}

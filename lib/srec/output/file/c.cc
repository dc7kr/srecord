//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2003 Peter Miller;
//	All rights reserved.
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
// MANIFEST: functions to impliment the srec_output_file_c class
//

#pragma implementation "srec_output_file_c"

#include <interval.h>
#include <srec/output/file/c.h>
#include <srec/record.h>


srec_output_file_c::srec_output_file_c() :
	srec_output_file("-"),
	prefix("no_name_given"),
	taddr(0),
	header_done(false),
	column(0),
	current_address(0),
	line_length(75),
	address_length(4)
{
}


srec_output_file_c::srec_output_file_c(const char *filename, const char *a2) :
	srec_output_file(filename),
	prefix(a2),
	taddr(0),
	header_done(false),
	column(0),
	current_address(0),
	line_length(75),
	address_length(4)
{
}


void
srec_output_file_c::emit_header()
{
	if (header_done)
		return;
	put_stringf("unsigned char %s[] =\n{\n", prefix.c_str());
	header_done = true;
}


void
srec_output_file_c::emit_byte(int n)
{
	if (column >= line_length)
	{
		put_char('\n');
		column = 0;
	}
	else if (column)
	{
		put_char(' ');
		++column;
	}
	put_stringf("0x%02X,", (unsigned char)n);
	column += 5;
}


srec_output_file_c::~srec_output_file_c()
{
	emit_header();
	if (range.empty())
		emit_byte(0xFF);
	if (column)
		put_char('\n');
	put_string("};\n");

	int width = 2 * address_length;
	if (!data_only_flag)
	{
		put_stringf("unsigned long %s_termination = 0x%0*lX;\n",
			prefix.c_str(), width, taddr);
		put_stringf("unsigned long %s_start       = 0x%0*lX;\n",
			prefix.c_str(), width, range.get_lowest());
		put_stringf("unsigned long %s_finish      = 0x%0*lX;\n",
			prefix.c_str(), width, range.get_highest());
	}
	put_stringf("unsigned long %s_length      = 0x%0*lX;\n",
		prefix.c_str(),
		width,
		range.get_highest() - range.get_lowest());
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
			int c = *cp++;
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
		if (range.empty())
			current_address = record.get_address();
		range +=
			interval
			(
				record.get_address(),
				record.get_address() + record.get_length()
			);
		emit_header();
		while (current_address < record.get_address())
		{
			emit_byte(0xFF);
			++current_address;
		}
		for (int j = 0; j < record.get_length(); ++j)
		{
			if (record.get_address() + j < current_address)
				continue;
			emit_byte(record.get_data(j));
			++current_address;
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
	n = (n + 1) / 6;
	if (n < 1)
		n = 1;
	n = n * 6 - 1;
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
	}
}


int
srec_output_file_c::preferred_block_size_get()
	const
{
	//
	// Irrelevant.  Use the largest we can get.
	//
	return srec_record::max_data_length;
}

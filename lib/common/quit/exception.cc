//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002 Peter Miller;
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
// MANIFEST: functions to impliment the quit_exception class
//

#pragma implementation "quit_exception"

#include <quit/exception.h>


quit_exception::quit_exception()
{
}


quit_exception::quit_exception(const quit_exception &arg)
{
}


quit_exception &
quit_exception::operator=(const quit_exception &arg)
{
	return *this;
}


quit_exception::~quit_exception()
{
}


void
quit_exception::exit(int n)
{
	throw vomit();
}


void
quit_exception::message_v(const char *, va_list)
{
	// don't say anything
}

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
// MANIFEST: interface definition for include/quit/prefix.cc
//

#ifndef INCLUDE_QUIT_PREFIX_H
#define INCLUDE_QUIT_PREFIX_H

#pragma interface "quit_prefix"

#include <string>
using namespace std;
#include <quit.h>

/**
  * The quit_prefix class is used to filter the messages issued to include
  * a line prefix.  The message is then passed ot a deeper quit handler.
  */
class quit_prefix:
	public quit
{
public:
	/**
	  * The destructor.
	  */
	virtual ~quit_prefix();

	/**
	  * A constructor.  The given handler is used to process the
	  * messages, once prefixed.  The given string is used as the
	  * line prefix.
	  */
	quit_prefix(quit &deeper, const char *prefix);

	/**
	  * A constructor.  The given handler is used to process the
	  * messages, once prefixed.  The given string is used as the
	  * line prefix.
	  */
	quit_prefix(quit &deeper, const string &prefix);

	// See base class for documentation.
	virtual void exit(int);

	// See base class for documentation.
	virtual void message_v(const char *, va_list);

private:
	/**
	  * The prefix instance variable is used to remember the line
	  * prefix to use.
	  */
	string prefix;

	/**
	  * The deepr instance variable is used to remember which handler
	  * is to be used to process the error messages once the refix
	  * has been added.
	  */
	quit &deeper;

	/**
	  * The default constructor.  Do not use.
	  */
	quit_prefix();

	/**
	  * The copy constructor.  Do not use.
	  */
	quit_prefix(const quit_prefix &);

	/**
	  * The assignment operator.  Do not use.
	  */
	quit_prefix &operator=(const quit_prefix &);
};

#endif // INCLUDE_QUIT_PREFIX_H

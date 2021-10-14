//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/quit/exception.cc
//

#ifndef INCLUDE_QUIT_EXCEPTION_H
#define INCLUDE_QUIT_EXCEPTION_H


#include <lib/quit.h>

/**
  * The quit_exception class is use to represent a quit handler which
  * throws an exception, rather than call ::exit.
  */
class quit_exception:
    public quit
{
public:
    /**
      * The destructor.
      */
    virtual ~quit_exception();

    /**
      * The default constructor.
      */
    quit_exception();

    /**
      * the vomit class is used for the throw.
      */
    class vomit { };

protected:
    // see base class for documentation
    virtual void exit(int n);

    // see base class for documentation
    virtual void message_v(const char *fmt, va_list);

private:
    /**
      * The copy constructor.  Do not use.
      */
    quit_exception(const quit_exception &);

    /**
      * The assignment operator.  Do not use.
      */
    quit_exception &operator=(const quit_exception &);
};

#endif // INCLUDE_QUIT_EXCEPTION_H

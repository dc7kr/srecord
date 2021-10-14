//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2006-2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILE_WILSON_H
#define LIB_SREC_INPUT_FILE_WILSON_H

#include <lib/srec/input/file.h>

/**
  * The srec_input_file_wilson clas sis used to represent the parse state
  * of an input file in Wilson format.  (Named after the person who wasked
  * for it; I still don't have an official definition of this format,
  * but it is very much like a binary(ish) form of the Motorola format.)
  */
class srec_input_file_wilson:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_wilson();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_wilson(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    const char *mode() const;

protected:
    /**
      * The get_byte method is used to get a byte of input.  We override
      * because the byte encoding is one character of input sometimes,
      * and two characters of input at other times.
      */
    virtual int get_byte();

private:
    /**
      * The read_inner method is used to read a record from the input.
      * The raead() method is a wrapper around this method.
      */
    int read_inner(srec_record &);

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning about garbage input has been issued yet.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * this file contains any data so far.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether the termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_wilson();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_wilson(const srec_input_file_wilson &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_wilson &operator=(const srec_input_file_wilson &);
};

#endif // LIB_SREC_INPUT_FILE_WILSON_H
